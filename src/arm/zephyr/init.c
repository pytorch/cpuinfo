/*
 * Copyright (c) 2026 BayLibre SAS
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * cpuinfo initialization for AArch64 Zephyr targets.
 *
 * Reads ARM64 ID registers directly to detect ISA features instead of
 * parsing /proc/cpuinfo or Linux HWCAP bits.  This is more accurate
 * than the Linux path since we get the raw hardware capability bits.
 *
 * Topology is simplified to a single package/cluster with the number
 * of cores reported by Zephyr's CONFIG_MP_MAX_NUM_CPUS.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <zephyr/kernel.h>

#include <arm/api.h>
#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>

struct cpuinfo_arm_isa cpuinfo_isa = {0};

static struct cpuinfo_package static_package = {{0}};

/*
 * ID_AA64ISAR0_EL1 field definitions (4-bit fields).
 */
#define ISAR0_AES_SHIFT 4
#define ISAR0_SHA1_SHIFT 8
#define ISAR0_SHA2_SHIFT 12
#define ISAR0_CRC32_SHIFT 16
#define ISAR0_ATOMIC_SHIFT 20
#define ISAR0_RDM_SHIFT 28
#define ISAR0_DP_SHIFT 44 /* DotProduct (UDOT/SDOT) */
#define ISAR0_FHM_SHIFT 48

/*
 * ID_AA64ISAR1_EL1 field definitions.
 */
#define ISAR1_JSCVT_SHIFT 12
#define ISAR1_FCMA_SHIFT 16
#define ISAR1_I8MM_SHIFT 52
#define ISAR1_BF16_SHIFT 44

/*
 * ID_AA64PFR0_EL1 field definitions.
 */
#define PFR0_FP_SHIFT 16
#define PFR0_ADVSIMD_SHIFT 20
#define PFR0_SVE_SHIFT 32

/*
 * ID_AA64PFR1_EL1 field definitions.
 */
#define PFR1_SME_SHIFT 24

/*
 * ID_AA64SMFR0_EL1 field definitions (for SME sub-features).
 */
#define SMFR0_I16I32_BIT (1ULL << 52)
#define SMFR0_BI32I32_BIT (1ULL << 48)
#define SMFR0_B16B16_BIT (1ULL << 60)
#define SMFR0_F16F16_BIT (1ULL << 42)

static inline uint64_t read_id_aa64isar0(void) {
	uint64_t val;
	__asm__ volatile("mrs %0, id_aa64isar0_el1" : "=r"(val));
	return val;
}

static inline uint64_t read_id_aa64isar1(void) {
	uint64_t val;
	__asm__ volatile("mrs %0, id_aa64isar1_el1" : "=r"(val));
	return val;
}

static inline uint64_t read_id_aa64pfr0(void) {
	uint64_t val;
	__asm__ volatile("mrs %0, id_aa64pfr0_el1" : "=r"(val));
	return val;
}

static inline uint64_t read_id_aa64pfr1(void) {
	uint64_t val;
	__asm__ volatile("mrs %0, id_aa64pfr1_el1" : "=r"(val));
	return val;
}

static inline uint64_t read_midr(void) {
	uint64_t val;
	__asm__ volatile("mrs %0, midr_el1" : "=r"(val));
	return val;
}

#define FIELD(reg, shift) (((reg) >> (shift)) & 0xf)

static void detect_isa_features(void) {
	const uint64_t isar0 = read_id_aa64isar0();
	const uint64_t isar1 = read_id_aa64isar1();
	const uint64_t pfr0 = read_id_aa64pfr0();
	const uint64_t pfr1 = read_id_aa64pfr1();

	/* ID_AA64ISAR0_EL1 */
	cpuinfo_isa.aes = FIELD(isar0, ISAR0_AES_SHIFT) >= 1;
	cpuinfo_isa.pmull = FIELD(isar0, ISAR0_AES_SHIFT) >= 2;
	cpuinfo_isa.sha1 = FIELD(isar0, ISAR0_SHA1_SHIFT) >= 1;
	cpuinfo_isa.sha2 = FIELD(isar0, ISAR0_SHA2_SHIFT) >= 1;
	cpuinfo_isa.crc32 = FIELD(isar0, ISAR0_CRC32_SHIFT) >= 1;
	cpuinfo_isa.atomics = FIELD(isar0, ISAR0_ATOMIC_SHIFT) >= 2;
	cpuinfo_isa.rdm = FIELD(isar0, ISAR0_RDM_SHIFT) >= 1;
	cpuinfo_isa.dot = FIELD(isar0, ISAR0_DP_SHIFT) >= 1;
	cpuinfo_isa.fhm = FIELD(isar0, ISAR0_FHM_SHIFT) >= 1;

	/* ID_AA64ISAR1_EL1 */
	cpuinfo_isa.jscvt = FIELD(isar1, ISAR1_JSCVT_SHIFT) >= 1;
	cpuinfo_isa.fcma = FIELD(isar1, ISAR1_FCMA_SHIFT) >= 1;
	cpuinfo_isa.i8mm = FIELD(isar1, ISAR1_I8MM_SHIFT) >= 1;
	cpuinfo_isa.bf16 = FIELD(isar1, ISAR1_BF16_SHIFT) >= 1;

	/* ID_AA64PFR0_EL1: FP16 arithmetic requires both FP and AdvSIMD
	 * fields to report half-precision support (value >= 1). */
	const uint32_t fp_field = FIELD(pfr0, PFR0_FP_SHIFT);
	const uint32_t advsimd_field = FIELD(pfr0, PFR0_ADVSIMD_SHIFT);
	cpuinfo_isa.fp16arith = (fp_field >= 1 && fp_field != 0xf) && (advsimd_field >= 1 && advsimd_field != 0xf);

	/* SVE */
	cpuinfo_isa.sve = FIELD(pfr0, PFR0_SVE_SHIFT) >= 1;
	if (cpuinfo_isa.sve) {
		/* SVE2: indicated by ID_AA64ZFR0_EL1.SVEver >= 1 */
		uint64_t zfr0;
		__asm__ volatile("mrs %0, s3_0_c0_c4_4" : "=r"(zfr0));
		cpuinfo_isa.sve2 = (zfr0 & 0xf) >= 1;

		/* SVE vector length in bytes. */
		uint64_t vl_bytes;
		__asm__ volatile("rdvl %0, #1" : "=r"(vl_bytes));
		cpuinfo_isa.svelen = (uint32_t)vl_bytes;
	}

	/* ID_AA64PFR1_EL1: SME */
	const uint32_t sme_field = FIELD(pfr1, PFR1_SME_SHIFT);
	cpuinfo_isa.sme = sme_field >= 1;
	cpuinfo_isa.sme2 = sme_field >= 2;
	cpuinfo_isa.sme2p1 = sme_field >= 3;

	if (cpuinfo_isa.sme) {
		uint64_t smfr0;
		__asm__ volatile("mrs %0, s3_0_c0_c4_5" : "=r"(smfr0));
		cpuinfo_isa.sme_i16i32 = !!(smfr0 & SMFR0_I16I32_BIT);
		cpuinfo_isa.sme_bi32i32 = !!(smfr0 & SMFR0_BI32I32_BIT);
		cpuinfo_isa.sme_b16b16 = !!(smfr0 & SMFR0_B16B16_BIT);
		cpuinfo_isa.sme_f16f16 = !!(smfr0 & SMFR0_F16F16_BIT);

		/*
		 * SME streaming vector length may differ from SVE vector
		 * length but reading it requires entering streaming mode
		 * (SMSTART/SMSTOP) which has side effects.  Default to
		 * SVE length as a reasonable approximation.
		 */
		cpuinfo_isa.smelen = cpuinfo_isa.svelen;
	}
}

void cpuinfo_zephyr_arm_init(void) {
	struct cpuinfo_processor* processors = NULL;
	struct cpuinfo_core* cores = NULL;
	struct cpuinfo_cluster* clusters = NULL;
	struct cpuinfo_uarch_info* uarchs = NULL;
	struct cpuinfo_cache* l1i = NULL;
	struct cpuinfo_cache* l1d = NULL;
	struct cpuinfo_cache* l2 = NULL;

	const uint32_t core_count = arch_num_cpus();
	const uint32_t cluster_count = 1;

	detect_isa_features();

	processors = calloc(core_count, sizeof(struct cpuinfo_processor));
	cores = calloc(core_count, sizeof(struct cpuinfo_core));
	clusters = calloc(cluster_count, sizeof(struct cpuinfo_cluster));
	uarchs = calloc(cluster_count, sizeof(struct cpuinfo_uarch_info));
	l1i = calloc(core_count, sizeof(struct cpuinfo_cache));
	l1d = calloc(core_count, sizeof(struct cpuinfo_cache));
	l2 = calloc(cluster_count, sizeof(struct cpuinfo_cache));

	if (!processors || !cores || !clusters || !uarchs || !l1i || !l1d || !l2) {
		cpuinfo_log_error("failed to allocate cpuinfo structures");
		goto cleanup;
	}

	const uint64_t midr = read_midr();
	enum cpuinfo_vendor vendor = cpuinfo_vendor_unknown;
	enum cpuinfo_uarch uarch = cpuinfo_uarch_unknown;
	cpuinfo_arm_decode_vendor_uarch((uint32_t)midr, &vendor, &uarch);

	strncpy(static_package.name, "ARM Cortex-A (Zephyr)", CPUINFO_PACKAGE_NAME_MAX);
	static_package.processor_count = core_count;
	static_package.core_count = core_count;
	static_package.cluster_count = cluster_count;

	clusters[0] = (struct cpuinfo_cluster){
		.processor_start = 0,
		.processor_count = core_count,
		.core_start = 0,
		.core_count = core_count,
		.cluster_id = 0,
		.package = &static_package,
		.vendor = vendor,
		.uarch = uarch,
	};

	for (uint32_t i = 0; i < core_count; i++) {
		processors[i] = (struct cpuinfo_processor){
			.smt_id = 0,
			.core = cores + i,
			.cluster = clusters,
			.package = &static_package,
			.cache.l1i = l1i + i,
			.cache.l1d = l1d + i,
			.cache.l2 = l2,
		};

		cores[i] = (struct cpuinfo_core){
			.processor_start = i,
			.processor_count = 1,
			.core_id = i,
			.cluster = clusters,
			.package = &static_package,
			.vendor = vendor,
			.uarch = uarch,
			.midr = (uint32_t)midr,
		};

		/*
		 * TODO: read CCSIDR_EL1 (via CSSELR_EL1 level select) for
		 * actual cache geometry.  These are conservative defaults
		 * typical for Cortex-A class cores.
		 */
		l1i[i] = (struct cpuinfo_cache){
			.size = 64 * 1024,
			.associativity = 4,
			.line_size = 64,
			.processor_start = i,
			.processor_count = 1,
		};

		l1d[i] = (struct cpuinfo_cache){
			.size = 64 * 1024,
			.associativity = 4,
			.line_size = 64,
			.processor_start = i,
			.processor_count = 1,
		};
	}

	l2[0] = (struct cpuinfo_cache){
		.size = 512 * 1024,
		.associativity = 8,
		.line_size = 64,
		.processor_start = 0,
		.processor_count = core_count,
	};

	/* Commit */
	cpuinfo_processors = processors;
	cpuinfo_cores = cores;
	cpuinfo_clusters = clusters;
	cpuinfo_packages = &static_package;

	cpuinfo_cache[cpuinfo_cache_level_1i] = l1i;
	cpuinfo_cache[cpuinfo_cache_level_1d] = l1d;
	cpuinfo_cache[cpuinfo_cache_level_2] = l2;

	cpuinfo_cache_count[cpuinfo_cache_level_1i] = core_count;
	cpuinfo_cache_count[cpuinfo_cache_level_1d] = core_count;
	cpuinfo_cache_count[cpuinfo_cache_level_2] = cluster_count;

	uarchs[0] = (struct cpuinfo_uarch_info){
		.uarch = uarch,
		.processor_count = core_count,
		.core_count = core_count,
	};
	cpuinfo_uarchs = uarchs;
	cpuinfo_uarchs_count = cluster_count;

	cpuinfo_processors_count = core_count;
	cpuinfo_cores_count = core_count;
	cpuinfo_clusters_count = cluster_count;
	cpuinfo_packages_count = 1;

	cpuinfo_max_cache_size = 8 * 1024 * 1024;

	cpuinfo_is_initialized = true;

	processors = NULL;
	cores = NULL;
	clusters = NULL;
	uarchs = NULL;
	l1i = l1d = l2 = NULL;

cleanup:
	free(processors);
	free(cores);
	free(clusters);
	free(uarchs);
	free(l1i);
	free(l1d);
	free(l2);
}
