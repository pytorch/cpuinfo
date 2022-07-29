#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/machine.h>

#include <cpuinfo.h>
#include <mach/api.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>

#define SAFE_FREE_NULL(x) 	\
	do { 					\
		if((x)) free((x));	\
		(x) = NULL;			\
	} while(0);

/* Polyfill recent CPUFAMILY_ARM_* values for older SDKs */
#ifndef CPUFAMILY_ARM_MONSOON_MISTRAL
	#define CPUFAMILY_ARM_MONSOON_MISTRAL   0xE81E7EF6
#endif
#ifndef CPUFAMILY_ARM_VORTEX_TEMPEST
	#define CPUFAMILY_ARM_VORTEX_TEMPEST    0x07D34B9F
#endif
#ifndef CPUFAMILY_ARM_LIGHTNING_THUNDER
	#define CPUFAMILY_ARM_LIGHTNING_THUNDER 0x462504D2
#endif
#ifndef CPUFAMILY_ARM_FIRESTORM_ICESTORM
	#define CPUFAMILY_ARM_FIRESTORM_ICESTORM 0x1B588BB3
#endif

struct cpuinfo_arm_isa cpuinfo_isa = {
#if CPUINFO_ARCH_ARM
	.thumb = true,
	.thumb2 = true,
	.thumbee = false,
	.jazelle = false,
	.armv5e = true,
	.armv6 = true,
	.armv6k = true,
	.armv7 = true,
	.vfpv2 = false,
	.vfpv3 = true,
	.d32 = true,
	.wmmx = false,
	.wmmx2 = false,
	.neon = true,
#endif
#if CPUINFO_ARCH_ARM64
	.aes = true,
	.sha1 = true,
	.sha2 = true,
	.pmull = true,
	.crc32 = true,
#endif
};

struct cache_array {
	struct cpuinfo_cache *caches;
	uint32_t count;
};

/*
 * iOS 15 and macOS Monterey 12 added sysctls to describe configuration information
 * where not all cores are the same (number of cores, cache sizes).
 * 
 * Each perflevel sysctl has a prefix of `hw.perflevel??.` where ?? is the
 * perflevel index, starting at zero.  The total number of perflevels are
 * exposed via the `hw.nperflevels` sysctl.  Higher performance perflevels
 * have lower indexes.
 *
 * sysctls:
 * - hw.nperflevels     - number of different types of cores / cache configs (perflevels)
 * - hw.perflevel?? 
 *   - .physicalcpu     - number of enabled physical cores for perflevel ??
 *   - .physicalcpu_max - number of physical cores for perflevel ??
 *   - .logicalcpu      - number of enabled logical cores for perflevel ??
 *   - .logicalcpu_max  - number of logical cores for perflevel ??
 *   - .l1icachesize    - size in bytes of L1 instruction cache for cores in perflevel ??
 *   - .l1dcachesize    - size in bytes of L1 data cache for cores in perflevel ??
 *   - .l2cachesize     - size in bytes of L2 data cache for cores in perflevel ??
 *   - .cpusperl2       - number of cores that share an L2 cache in perflevel ??
 *   - .l3cachesize     - size in bytes of L3 data cache for cores in perflevel ??
 *   - .cpusperl3       - number of cores that share an L3 cache in perflevel ??
 *
 * Technically, these perflevels could be in src/mach/api.h since they are supported
 * across architectures (x86_64 and arm64).  x86_64 doesn't currently have multiple
 * perflevels, which means there's not much benefit there.
 */
struct mach_perflevel {
	uint32_t physicalcpu;
	uint32_t physicalcpu_max;
	uint32_t logicalcpu;
	uint32_t logicalcpu_max;
	uint32_t l1icachesize;
	uint32_t l1dcachesize;
	uint32_t l2cachesize;
	uint32_t cpusperl2;
	uint32_t l3cachesize;
	uint32_t cpusperl3;

	uint32_t core_start;		/* first core index this perflevel describes */
	uint32_t processor_start;	/* first processor index this perflevel describes */
};

static uint32_t get_sys_info(int type_specifier, const char* name) {
	size_t size = 0;
	uint32_t result = 0;
	int mib[2] = { CTL_HW, type_specifier };
	if (sysctl(mib, 2, NULL, &size, NULL, 0) != 0) {
		cpuinfo_log_info("sysctl(\"%s\") failed: %s", name, strerror(errno));
	} else if (size == sizeof(uint32_t)) {
		sysctl(mib, 2, &result, &size, NULL, 0);
		cpuinfo_log_debug("%s: %"PRIu32 ", size = %lu", name, result, size);
	} else {
		cpuinfo_log_info("sysctl does not support non-integer lookup for (\"%s\")", name);
	}
	return result;
}

static uint32_t get_sys_info_by_name(const char* type_specifier) {
	size_t size = 0;
	uint32_t result = 0;
	if (sysctlbyname(type_specifier, NULL, &size, NULL, 0) != 0) {
		cpuinfo_log_info("sysctlbyname(\"%s\") failed: %s", type_specifier, strerror(errno));
	} else if (size == sizeof(uint32_t)) {
		sysctlbyname(type_specifier, &result, &size, NULL, 0);
		cpuinfo_log_debug("%s: %"PRIu32 ", size = %lu", type_specifier, result, size);
	} else {
		cpuinfo_log_info("sysctl does not support non-integer lookup for (\"%s\")", type_specifier);
	}
	return result;
}

static enum cpuinfo_uarch decode_uarch(uint32_t cpu_family, uint32_t cpu_subtype, uint32_t core_index, uint32_t core_count) {
	switch (cpu_family) {
		case CPUFAMILY_ARM_SWIFT:
			return cpuinfo_uarch_swift;
		case CPUFAMILY_ARM_CYCLONE:
			return cpuinfo_uarch_cyclone;
		case CPUFAMILY_ARM_TYPHOON:
			return cpuinfo_uarch_typhoon;
		case CPUFAMILY_ARM_TWISTER:
			return cpuinfo_uarch_twister;
		case CPUFAMILY_ARM_HURRICANE:
			return cpuinfo_uarch_hurricane;
		case CPUFAMILY_ARM_MONSOON_MISTRAL:
			/* 2x Monsoon + 4x Mistral cores */
			return core_index < 2 ? cpuinfo_uarch_monsoon : cpuinfo_uarch_mistral;
		case CPUFAMILY_ARM_VORTEX_TEMPEST:
			/* Hexa-core: 2x Vortex + 4x Tempest; Octa-core: 4x Cortex + 4x Tempest */
			return core_index + 4 < core_count ? cpuinfo_uarch_vortex : cpuinfo_uarch_tempest;
		case CPUFAMILY_ARM_LIGHTNING_THUNDER:
			/* Hexa-core: 2x Lightning + 4x Thunder; Octa-core (presumed): 4x Lightning + 4x Thunder */
			return core_index + 4 < core_count ? cpuinfo_uarch_lightning : cpuinfo_uarch_thunder;
		case CPUFAMILY_ARM_FIRESTORM_ICESTORM:
			/* Hexa-core: 2x Firestorm + 4x Icestorm; Octa-core: 4x Firestorm + 4x Icestorm */
			return core_index + 4 < core_count ? cpuinfo_uarch_firestorm : cpuinfo_uarch_icestorm;
		default:
			/* Use hw.cpusubtype for detection */
			break;
	}

	#if CPUINFO_ARCH_ARM
		switch (cpu_subtype) {
			case CPU_SUBTYPE_ARM_V7:
				return cpuinfo_uarch_cortex_a8;
			case CPU_SUBTYPE_ARM_V7F:
				return cpuinfo_uarch_cortex_a9;
			case CPU_SUBTYPE_ARM_V7K:
				return cpuinfo_uarch_cortex_a7;
			default:
				return cpuinfo_uarch_unknown;
		}
	#else
		return cpuinfo_uarch_unknown;
	#endif
}

static void decode_package_name(char* package_name) {
	size_t size;
	if (sysctlbyname("hw.machine", NULL, &size, NULL, 0) != 0) {
		cpuinfo_log_warning("sysctlbyname(\"hw.machine\") failed: %s", strerror(errno));
		return;
	}

	char *machine_name = alloca(size);
	if (sysctlbyname("hw.machine", machine_name, &size, NULL, 0) != 0) {
		cpuinfo_log_warning("sysctlbyname(\"hw.machine\") failed: %s", strerror(errno));
		return;
	}
	cpuinfo_log_debug("hw.machine: %s", machine_name);

	char name[10];
	uint32_t major = 0, minor = 0;
	if (sscanf(machine_name, "%9[^,0123456789]%"SCNu32",%"SCNu32, name, &major, &minor) != 3) {
		cpuinfo_log_warning("parsing \"hw.machine\" failed: %s", strerror(errno));
		return;
	}

	uint32_t chip_model = 0;
	char suffix = '\0';
	if (strcmp(name, "iPhone") == 0) {
		/*
		 * iPhone 4 and up are supported:
		 *  - iPhone 4       [A4]:  iPhone3,1, iPhone3,2, iPhone3,3
		 *  - iPhone 4S      [A5]:  iPhone4,1
		 *  - iPhone 5       [A6]:  iPhone5,1, iPhone5,2
		 *  - iPhone 5c      [A6]:  iPhone5,3, iPhone5,4
		 *  - iPhone 5s      [A7]:  iPhone6,1, iPhone6,2
		 *  - iPhone 6       [A8]:  iPhone7,2
		 *  - iPhone 6 Plus  [A8]:  iPhone7,1
		 *  - iPhone 6s      [A9]:  iPhone8,1
		 *  - iPhone 6s Plus [A9]:  iPhone8,2
		 *  - iPhone SE      [A9]:  iPhone8,4
		 *  - iPhone 7       [A10]: iPhone9,1, iPhone9,3
		 *  - iPhone 7 Plus  [A10]: iPhone9,2, iPhone9,4
		 *  - iPhone 8       [A11]: iPhone10,1, iPhone10,4
		 *  - iPhone 8 Plus  [A11]: iPhone10,2, iPhone10,5
		 *  - iPhone X       [A11]: iPhone10,3, iPhone10,6
		 *  - iPhone XS      [A12]: iPhone11,2,
		 *  - iPhone XS Max  [A12]: iPhone11,4, iPhone11,6
		 *  - iPhone XR      [A12]: iPhone11,8
		 */
		chip_model = major + 1;
	} else if (strcmp(name, "iPad") == 0) {
		switch (major) {
			/* iPad 2 and up are supported */
			case 2:
				/*
				 * iPad 2    [A5]: iPad2,1, iPad2,2, iPad2,3, iPad2,4
				 * iPad mini [A5]: iPad2,5, iPad2,6, iPad2,7
				 */
				chip_model = major + 3;
				break;
			case 3:
				/*
				 * iPad 3rd Gen [A5X]: iPad3,1, iPad3,2, iPad3,3
				 * iPad 4th Gen [A6X]: iPad3,4, iPad3,5, iPad3,6
				 */
				chip_model = (minor <= 3) ? 5 : 6;
				suffix = 'X';
				break;
			case 4:
				/*
				 * iPad Air         [A7]: iPad4,1, iPad4,2, iPad4,3
				 * iPad mini Retina [A7]: iPad4,4, iPad4,5, iPad4,6
				 * iPad mini 3      [A7]: iPad4,7, iPad4,8, iPad4,9
				 */
				chip_model = major + 3;
				break;
			case 5:
				/*
				 * iPad mini 4 [A8]:  iPad5,1, iPad5,2
				 * iPad Air 2  [A8X]: iPad5,3, iPad5,4
				 */
				chip_model = major + 3;
				suffix = (minor <= 2) ? '\0' : 'X';
				break;
			case 6:
				/*
				 * iPad Pro 9.7" [A9X]: iPad6,3, iPad6,4
				 * iPad Pro      [A9X]: iPad6,7, iPad6,8
				 * iPad 5th Gen  [A9]:  iPad6,11, iPad6,12
				 */
				chip_model = major + 3;
				suffix = minor <= 8 ? 'X' : '\0';
				break;
			case 7:
				/*
				 * iPad Pro 12.9" [A10X]: iPad7,1, iPad7,2
				 * iPad Pro 10.5" [A10X]: iPad7,3, iPad7,4
				 * iPad 6th Gen   [A10]:  iPad7,5, iPad7,6
				 */
				chip_model = major + 3;
				suffix = minor <= 4 ? 'X' : '\0';
				break;
			default:
				cpuinfo_log_info("unknown iPad: %s", machine_name);
				break;
		}
	} else if (strcmp(name, "iPod") == 0) {
		switch (major) {
			case 5:
				chip_model = 5;
				break;
				/* iPod touch (5th Gen) [A5]: iPod5,1 */
			case 7:
				/* iPod touch (6th Gen, 2015) [A8]: iPod7,1 */
				chip_model = 8;
				break;
			default:
				cpuinfo_log_info("unknown iPod: %s", machine_name);
				break;
		}
	} else {
		cpuinfo_log_info("unknown device: %s", machine_name);
	}
	if (chip_model != 0) {
		snprintf(package_name, CPUINFO_PACKAGE_NAME_MAX, "Apple A%"PRIu32"%c", chip_model, suffix);
	}
}

static void detect_isa(uint32_t cpu_family, uint32_t cpu_type, uint32_t cpu_subtype) {
	switch (cpu_type) {
		case CPU_TYPE_ARM64:
			cpuinfo_isa.aes = true;
			cpuinfo_isa.sha1 = true;
			cpuinfo_isa.sha2 = true;
			cpuinfo_isa.pmull = true;
			cpuinfo_isa.crc32 = true;
			break;
#if CPUINFO_ARCH_ARM
		case CPU_TYPE_ARM:
			switch (cpu_subtype) {
				case CPU_SUBTYPE_ARM_V8:
					cpuinfo_isa.armv8 = true;
					cpuinfo_isa.aes = true;
					cpuinfo_isa.sha1 = true;
					cpuinfo_isa.sha2 = true;
					cpuinfo_isa.pmull = true;
					cpuinfo_isa.crc32 = true;
					/* Fall-through to add ARMv7S features */
				case CPU_SUBTYPE_ARM_V7S:
				case CPU_SUBTYPE_ARM_V7K:
					cpuinfo_isa.fma = true;
					/* Fall-through to add ARMv7F features */
				case CPU_SUBTYPE_ARM_V7F:
					cpuinfo_isa.armv7mp = true;
					cpuinfo_isa.fp16 = true;
					/* Fall-through to add ARMv7 features */
				case CPU_SUBTYPE_ARM_V7:
					break;
				default:
					break;
			}
			break;
#endif
	}

	/*
	 * iOS 15 and macOS 12 added sysctls for ARM features.  Use them where
	 * possible.  Otherwise, fallback to hardcoded set of CPUs with known
	 * support.
	 */

	const uint32_t has_feat_lse = get_sys_info_by_name("hw.optional.arm.FEAT_LSE");
	if (has_feat_lse != 0) {
		cpuinfo_isa.atomics = true;
	}
	#if CPUINFO_ARCH_ARM64
		else {
			switch (cpu_family) {
				case CPUFAMILY_ARM_MONSOON_MISTRAL:
				case CPUFAMILY_ARM_VORTEX_TEMPEST:
				case CPUFAMILY_ARM_LIGHTNING_THUNDER:
				case CPUFAMILY_ARM_FIRESTORM_ICESTORM:
					cpuinfo_isa.atomics = true;
			}
		}
	#endif

	const uint32_t has_feat_rdm = get_sys_info_by_name("hw.optional.arm.FEAT_RDM");
	if (has_feat_rdm != 0) {
		cpuinfo_isa.rdm = true;
	}

	const uint32_t has_feat_fp16 = get_sys_info_by_name("hw.optional.arm.FEAT_FP16");
	if (has_feat_fp16 != 0) {
		cpuinfo_isa.fp16arith = true;
	} else {
		switch (cpu_family) {
			case CPUFAMILY_ARM_MONSOON_MISTRAL:
			case CPUFAMILY_ARM_VORTEX_TEMPEST:
			case CPUFAMILY_ARM_LIGHTNING_THUNDER:
			case CPUFAMILY_ARM_FIRESTORM_ICESTORM:
				cpuinfo_isa.fp16arith = true;
		}
	}

	const uint32_t has_feat_dotprod = get_sys_info_by_name("hw.optional.arm.FEAT_DotProd");
	if (has_feat_dotprod != 0) {
		cpuinfo_isa.dot = true;
	} else {
		switch (cpu_family) {
			case CPUFAMILY_ARM_LIGHTNING_THUNDER:
			case CPUFAMILY_ARM_FIRESTORM_ICESTORM:
				cpuinfo_isa.dot = true;
		}
	}

	const uint32_t has_feat_jscvt = get_sys_info_by_name("hw.optional.arm.FEAT_JSCVT");
	if (has_feat_jscvt != 0) {
		cpuinfo_isa.jscvt = true;
	}

	const uint32_t has_feat_fcma = get_sys_info_by_name("hw.optional.arm.FEAT_FCMA");
	if (has_feat_fcma != 0) {
		cpuinfo_isa.fcma = true;
	}

	const uint32_t has_feat_bf16 = get_sys_info_by_name("hw.optional.arm.FEAT_BF16");
	if (has_feat_bf16 != 0) {
		cpuinfo_isa.bf16 = true;
	}

	const uint32_t has_feat_i8mm = get_sys_info_by_name("hw.optional.arm.FEAT_I8MM");
	if (has_feat_i8mm != 0) {
		cpuinfo_isa.i8mm = true;
	}

	const uint32_t has_feat_fhm = get_sys_info_by_name("hw.optional.arm.FEAT_FHM");
	if (has_feat_fhm != 0) {
		cpuinfo_isa.fhm = true;
	} else {
		// Prior to iOS 15, use 'hw.optional.armv8_2_fhm'
		const uint32_t has_feat_fhm_legacy = get_sys_info_by_name("hw.optional.armv8_2_fhm");
		if (has_feat_fhm_legacy != 0) {
			cpuinfo_isa.fhm = true;
		}
	}
}

static char * alloc_sysctl_perflevel_string(uint32_t perflevel, const char * const perflevel_suffix) {
	char * ret = NULL;
	int err = asprintf(&ret, "hw.perflevel%u.%s", perflevel, perflevel_suffix);
	if (err == -1 || ret == NULL) {
		cpuinfo_log_error("failed to allocate memory for hw.perflevel* string");
		return NULL;
	}

	return ret;
}

static struct mach_perflevel * read_perflevels(const uint32_t nperflevels) {
	struct mach_perflevel * perflevels = NULL;

	perflevels = calloc(nperflevels, sizeof (*perflevels));
	if (!perflevels) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" perflevels",
			nperflevels * sizeof(*perflevels), nperflevels);
		return NULL;
	}

	/*
	 * Each perflevel sysctl is of the form "hw.perflevel<int>.<name>"
	 * where <int> is an integer starting at zero and incrementing for each level
	 * and <name> is the name of the sysctl.  Since they change based on the
	 * level, we need to build them dynamically.
	 */
	char * sysctl_physicalcpu = NULL;
	char * sysctl_physicalcpu_max = NULL;
	char * sysctl_logicalcpu = NULL;
	char * sysctl_logicalcpu_max = NULL;
	char * sysctl_l1icachesize = NULL;
	char * sysctl_l1dcachesize = NULL;
	char * sysctl_l2cachesize = NULL;
	char * sysctl_cpusperl2 = NULL;
	char * sysctl_l3cachesize = NULL;
	char * sysctl_cpusperl3 = NULL;

	uint32_t core_index = 0;
	uint32_t processor_index = 0;

	bool success = true;
	uint32_t i = 0;
	for (; i<nperflevels; ++i) {
		sysctl_physicalcpu = alloc_sysctl_perflevel_string(i, "physicalcpu");
		if (!sysctl_physicalcpu) {
			success = false;
			break;
		}
		perflevels[i].physicalcpu = get_sys_info_by_name(sysctl_physicalcpu);

		sysctl_physicalcpu_max = alloc_sysctl_perflevel_string(i, "physicalcpu_max");
		if (!sysctl_physicalcpu_max) {
			success = false;
			break;
		}
		perflevels[i].physicalcpu_max = get_sys_info_by_name(sysctl_physicalcpu_max);

		sysctl_logicalcpu_max = alloc_sysctl_perflevel_string(i, "logicalcpu_max");
		if (!sysctl_logicalcpu_max) {
			success = false;
			break;
		}
		perflevels[i].logicalcpu_max = get_sys_info_by_name(sysctl_logicalcpu_max);

		sysctl_l1icachesize = alloc_sysctl_perflevel_string(i, "l1icachesize");
		if (!sysctl_l1icachesize) {
			success = false;
			break;
		}
		perflevels[i].l1icachesize = get_sys_info_by_name(sysctl_l1icachesize);

		sysctl_l1dcachesize = alloc_sysctl_perflevel_string(i, "l1dcachesize");
		if (!sysctl_l1dcachesize) {
			success = false;
			break;
		}
		perflevels[i].l1dcachesize = get_sys_info_by_name(sysctl_l1dcachesize);

		sysctl_l2cachesize = alloc_sysctl_perflevel_string(i, "l2cachesize");
		if (!sysctl_l2cachesize) {
			success = false;
			break;
		}
		perflevels[i].l2cachesize = get_sys_info_by_name(sysctl_l2cachesize);

		sysctl_cpusperl2 = alloc_sysctl_perflevel_string(i, "cpusperl2");
		if (!sysctl_cpusperl2) {
			success = false;
			break;
		}
		perflevels[i].cpusperl2 = get_sys_info_by_name(sysctl_cpusperl2);

		sysctl_l3cachesize = alloc_sysctl_perflevel_string(i, "l3cachesize");
		if (!sysctl_l3cachesize) {
			/* May not have L3 */
		} else {
			perflevels[i].l3cachesize = get_sys_info_by_name(sysctl_l3cachesize);
		}

		sysctl_cpusperl3 = alloc_sysctl_perflevel_string(i, "cpusperl3");
		if (!sysctl_cpusperl3) {
			/* May not have L3 */
		} else{
			perflevels[i].cpusperl3 = get_sys_info_by_name(sysctl_cpusperl3);
		}

		perflevels[i].core_start = core_index;
		core_index += perflevels[i].physicalcpu_max;
		cpuinfo_log_debug("perflevel%"PRIu32".core_start: %"PRIu32, i, perflevels[i].core_start);

		perflevels[i].processor_start = processor_index;
		processor_index += perflevels[i].logicalcpu_max;
		cpuinfo_log_debug("perflevel%"PRIu32".processor_start: %"PRIu32, i, perflevels[i].processor_start);

		/* Cleanup */
		SAFE_FREE_NULL(sysctl_physicalcpu);
		SAFE_FREE_NULL(sysctl_physicalcpu_max);
		SAFE_FREE_NULL(sysctl_logicalcpu);
		SAFE_FREE_NULL(sysctl_logicalcpu_max);
		SAFE_FREE_NULL(sysctl_l1icachesize);
		SAFE_FREE_NULL(sysctl_l1dcachesize);
		SAFE_FREE_NULL(sysctl_l2cachesize);
		SAFE_FREE_NULL(sysctl_cpusperl2);
		SAFE_FREE_NULL(sysctl_l3cachesize);
		SAFE_FREE_NULL(sysctl_cpusperl3);
	}


	if (!success) {
		/* We exited loop early (error).  Cleanup */
		SAFE_FREE_NULL(sysctl_physicalcpu);
		SAFE_FREE_NULL(sysctl_physicalcpu_max);
		SAFE_FREE_NULL(sysctl_logicalcpu);
		SAFE_FREE_NULL(sysctl_logicalcpu_max);
		SAFE_FREE_NULL(sysctl_l1icachesize);
		SAFE_FREE_NULL(sysctl_l1dcachesize);
		SAFE_FREE_NULL(sysctl_l2cachesize);
		SAFE_FREE_NULL(sysctl_cpusperl2);
		SAFE_FREE_NULL(sysctl_l3cachesize);
		SAFE_FREE_NULL(sysctl_cpusperl3);

		/* free / NULL return value */
		SAFE_FREE_NULL(perflevels);
	}

	return perflevels;
}

bool detect_caches_legacy(
	const struct cpuinfo_mach_topology mach_topology,
	struct cache_array *l1i,
	struct cache_array *l1d,
	struct cache_array *l2,
	struct cache_array *l3
)
{
	if (!l1i || !l1d || !l2 || !l3) {
		cpuinfo_log_error("cannot detect caches. no place to store results.");
		return false;
	}

	const uint32_t cacheline_size = get_sys_info(HW_CACHELINE, "HW_CACHELINE");
	const uint32_t l1d_cache_size = get_sys_info(HW_L1DCACHESIZE, "HW_L1DCACHESIZE");
	const uint32_t l1i_cache_size = get_sys_info(HW_L1ICACHESIZE, "HW_L1ICACHESIZE");
	const uint32_t l2_cache_size = get_sys_info(HW_L2CACHESIZE, "HW_L2CACHESIZE");
	const uint32_t l3_cache_size = get_sys_info(HW_L3CACHESIZE, "HW_L3CACHESIZE");
	/*
	 * Cache associativity, partitions, and flags values here are copied from
	 * previous implementation.
	 */
	const uint32_t l1_cache_associativity = 4;
	const uint32_t l2_cache_associativity = 8;
	const uint32_t l3_cache_associativity = 16;
	const uint32_t cache_partitions = 1;
	const uint32_t cache_flags = 0;

	uint32_t threads_per_l1 = 0;
	if (l1i_cache_size != 0 || l1d_cache_size != 0) {
		/* Assume L1 caches are private to each core */
		threads_per_l1 = 1;
		l1i->count = mach_topology.threads / threads_per_l1;
		l1d->count = l1i->count;
		cpuinfo_log_debug("detected %"PRIu32" L1 caches", l1i->count);
	}

	uint32_t threads_per_l2 = 0;
	if (l2_cache_size != 0) {
		/* Assume L2 cache is shared between all cores */
		threads_per_l2 = mach_topology.cores;
		l2->count = 1;
		cpuinfo_log_debug("detected %"PRIu32" L2 caches", l2->count);
	}

	uint32_t threads_per_l3 = 0;
	if (l3_cache_size != 0) {
		/* Assume L3 cache is shared between all cores */
		threads_per_l3 = mach_topology.cores;
		l3->count = 1;
		cpuinfo_log_debug("detected %"PRIu32" L3 caches", l3->count);
	}

	if (l1i_cache_size != 0) {
		l1i->caches = calloc(l1i->count, sizeof(*(l1i->caches)));
		if (l1i->caches == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1I caches",
				l1i->count * sizeof(*(l1i->caches)), l1i->count);
			return false;
		}
		for (uint32_t c = 0; c < l1i->count; c++) {
			l1i->caches[c] = (struct cpuinfo_cache) {
				.size            = l1i_cache_size,
				.associativity   = l1_cache_associativity,
				.sets            = l1i_cache_size / (l1_cache_associativity * cacheline_size),
				.partitions      = cache_partitions,
				.line_size       = cacheline_size,
				.flags           = cache_flags,
				.processor_start = c * threads_per_l1,
				.processor_count = threads_per_l1,
			};
		}
	}

	if (l1d_cache_size != 0) {
		l1d->caches = calloc(l1d->count, sizeof(*(l1d->caches)));
		if (l1d->caches == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1D caches",
				l1d->count * sizeof(*(l1d->caches)), l1d->count);
			return false;
		}
		for (uint32_t c = 0; c < l1d->count; c++) {
			l1d->caches[c] = (struct cpuinfo_cache) {
				.size            = l1d_cache_size,
				.associativity   = l1_cache_associativity,
				.sets            = l1d_cache_size / (l1_cache_associativity * cacheline_size),
				.partitions      = cache_partitions,
				.line_size       = cacheline_size,
				.flags           = cache_flags,
				.processor_start = c * threads_per_l1,
				.processor_count = threads_per_l1,
			};
		}
	}

	if (l2->count != 0) {
		l2->caches = calloc(l2->count, sizeof(*(l2->caches)));
		if (l2->caches == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L2 caches",
				l2->count * sizeof(*(l2->caches)), l2->count);
			return false;
		}
		for (uint32_t c = 0; c < l2->count; c++) {
			l2->caches[c] = (struct cpuinfo_cache) {
				.size            = l2_cache_size,
				.associativity   = l2_cache_associativity,
				.sets            = l2_cache_size / (l2_cache_associativity * cacheline_size),
				.partitions      = cache_partitions,
				.line_size       = cacheline_size,
				.flags           = cache_flags,
				.processor_start = c * threads_per_l2,
				.processor_count = threads_per_l2,
			};
		}
	}

	if (l3->count != 0) {
		l3->caches = calloc(l3->count, sizeof(*(l3->caches)));
		if (l3->caches == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L3 caches",
												l3->count * sizeof(*(l3->caches)), l3->count);
			return false;
		}
		for (uint32_t c = 0; c < l3->count; c++) {
			l3->caches[c] = (struct cpuinfo_cache) {
				.size            = l3_cache_size,
				.associativity   = l3_cache_associativity,
				.sets            = l3_cache_size / (l3_cache_associativity * cacheline_size),
				.partitions      = cache_partitions,
				.line_size       = cacheline_size,
				.flags           = cache_flags,
				.processor_start = c * threads_per_l3,
				.processor_count = threads_per_l3,
			};
		}
	}

	return true;
}

bool detect_caches_using_perflevels(
	const struct cpuinfo_mach_topology mach_topology,
	const struct mach_perflevel * const perflevels,
	const uint32_t nperflevels,
	struct cache_array *l1i,
	struct cache_array *l1d,
	struct cache_array *l2,
	struct cache_array *l3
)
{
	if (!l1i || !l1d || !l2 || !l3) {
		cpuinfo_log_error("cannot detect caches. no place to store results.");
		return false;
	}

	const uint32_t cacheline_size = get_sys_info(HW_CACHELINE, "HW_CACHELINE");
	/*
	 * Cache associativity, partitions, and flags values here are copied from
	 * previous implementation.
	 */
	const uint32_t l1_cache_associativity = 4;
	const uint32_t l2_cache_associativity = 8;
	const uint32_t l3_cache_associativity = 16;
	const uint32_t cache_partitions = 1;
	const uint32_t cache_flags = 0;

	l1i->count = 0;
	l1d->count = 0;
	l2->count = 0;
	l3->count = 0;
	for (uint32_t pl=0; pl<nperflevels; ++pl) {
		if (perflevels[pl].l1icachesize != 0) {
			/* One l1i cache per core */
			l1i->count += perflevels[pl].physicalcpu_max;
		}

		if (perflevels[pl].l1dcachesize != 0) {
			/* One l1d cache per core */
			l1d->count += perflevels[pl].physicalcpu_max;
		}

		if (perflevels[pl].cpusperl2 != 0) {
			l2->count += perflevels[pl].physicalcpu_max / perflevels[pl].cpusperl2;
		}

		if (perflevels[pl].cpusperl3 != 0) {
			l3->count += perflevels[pl].physicalcpu_max / perflevels[pl].cpusperl3;
		}
	}

	if (l1i->count != 0) {
		l1i->caches = calloc(l1i->count, sizeof(*(l1i->caches)));
		if (l1i->caches == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1I caches",
				l1i->count * sizeof(*(l1i->caches)), l1i->count);
			return false;
		}
		for (uint32_t pl=0; pl<nperflevels; ++pl) {
			if (perflevels[pl].l1icachesize != 0) {
				uint32_t threads_per_l1 = perflevels[pl].logicalcpu_max / perflevels[pl].physicalcpu_max;

				/* One l1i cache per core */
				uint32_t cache_start = perflevels[pl].core_start;
				uint32_t cache_end = cache_start + perflevels[pl].physicalcpu_max;
				for (uint32_t c=cache_start; c<cache_end; ++c) {
					l1i->caches[c] = (struct cpuinfo_cache) {
						.size            = perflevels[pl].l1icachesize,
						.associativity   = l1_cache_associativity,
						.sets            = perflevels[pl].l1icachesize / (l1_cache_associativity * cacheline_size),
						.partitions      = cache_partitions,
						.line_size       = cacheline_size,
						.flags           = cache_flags,
						.processor_start = c * threads_per_l1,
						.processor_count = threads_per_l1,
					};
				}
			}
		}
	}

	if (l1d->count != 0) {
		l1d->caches = calloc(l1d->count, sizeof(*(l1d->caches)));
		if (l1d->caches == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L1D caches",
				l1d->count * sizeof(*(l1d->caches)), l1d->count);
			return false;
		}
		for (uint32_t pl=0; pl<nperflevels; ++pl) {
			if (perflevels[pl].l1dcachesize != 0) {
				uint32_t threads_per_l1 = perflevels[pl].logicalcpu_max / perflevels[pl].physicalcpu_max;

				/* One l1d cache per core */
				uint32_t cache_start = perflevels[pl].core_start;
				uint32_t cache_end = cache_start + perflevels[pl].physicalcpu_max;
				for (uint32_t c=cache_start; c<cache_end; ++c) {
					l1d->caches[c] = (struct cpuinfo_cache) {
						.size            = perflevels[pl].l1dcachesize,
						.associativity   = l1_cache_associativity,
						.sets            = perflevels[pl].l1dcachesize / (l1_cache_associativity * cacheline_size),
						.partitions      = cache_partitions,
						.line_size       = cacheline_size,
						.flags           = cache_flags,
						.processor_start = c * threads_per_l1,
						.processor_count = threads_per_l1,
					};
				}
			}
		}
	}

	if (l2->count != 0) {
		l2->caches = calloc(l2->count, sizeof(*(l2->caches)));
		if (l2->caches == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L2 caches",
				l2->count * sizeof(*(l2->caches)), l2->count);
			return false;
		}
		uint32_t cache_index = 0;
		for (uint32_t pl=0; pl<nperflevels; ++pl) {
			if (perflevels[pl].l2cachesize != 0) {
				uint32_t l2_cache_count = perflevels[pl].physicalcpu_max / perflevels[pl].cpusperl2;
				uint32_t threads_per_core = perflevels[pl].logicalcpu_max / perflevels[pl].physicalcpu_max;
				uint32_t threads_per_l2 = threads_per_core * perflevels[pl].cpusperl2;
				uint32_t cache_end = cache_index + l2_cache_count;
				for (; cache_index<cache_end; ++cache_index) {
					l2->caches[cache_index] = (struct cpuinfo_cache) {
						.size            = perflevels[pl].l2cachesize,
						.associativity   = l2_cache_associativity,
						.sets            = perflevels[pl].l2cachesize / (l2_cache_associativity * cacheline_size),
						.partitions      = cache_partitions,
						.line_size       = cacheline_size,
						.flags           = cache_flags,
						.processor_start = cache_index * threads_per_l2,
						.processor_count = threads_per_l2,
					};
				}
			}
		}
	}

	if (l3->count != 0) {
		l3->caches = calloc(l3->count, sizeof(*(l3->caches)));
		if (l3->caches == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" L3 caches",
				l3->count * sizeof(*(l3->caches)), l3->count);
			return false;
		}
		uint32_t cache_index = 0;
		for (uint32_t pl=0; pl<nperflevels; ++pl) {
			if (perflevels[pl].l3cachesize != 0) {
				uint32_t l3_cache_count = perflevels[pl].physicalcpu_max / perflevels[pl].cpusperl3;
				uint32_t threads_per_core = perflevels[pl].logicalcpu_max / perflevels[pl].physicalcpu_max;
				uint32_t threads_per_l3 = threads_per_core * perflevels[pl].cpusperl3;
				uint32_t cache_end = cache_index + l3_cache_count;
				for (; cache_index<cache_end; ++cache_index) {
					l3->caches[cache_index] = (struct cpuinfo_cache) {
						.size            = perflevels[pl].l3cachesize,
						.associativity   = l3_cache_associativity,
						.sets            = perflevels[pl].l3cachesize / (l3_cache_associativity * cacheline_size),
						.partitions      = cache_partitions,
						.line_size       = cacheline_size,
						.flags           = cache_flags,
						.processor_start = cache_index * threads_per_l3,
						.processor_count = threads_per_l3,
					};
				}
			}
		}
	}

	return true;
}

void cpuinfo_arm_mach_init(void) {
	const uint32_t cpu_family = get_sys_info_by_name("hw.cpufamily");
	const uint32_t cpu_type = get_sys_info_by_name("hw.cputype");
	const uint32_t cpu_subtype = get_sys_info_by_name("hw.cpusubtype");

	detect_isa(cpu_family, cpu_type, cpu_subtype);

	struct cpuinfo_processor* processors = NULL;
	struct cpuinfo_core* cores = NULL;
	struct cpuinfo_cluster* clusters = NULL;
	struct cpuinfo_package* packages = NULL;
	struct cpuinfo_uarch_info* uarchs = NULL;
	struct cache_array l1i = {0};
	struct cache_array l1d = {0};
	struct cache_array l2 = {0};
	struct cache_array l3 = {0};

	struct cpuinfo_mach_topology mach_topology = cpuinfo_mach_detect_topology();

	/* 
	 * iOS 15 and macOS Monterey 12 added sysctls for specifying different performance
	 * levels.  Probe `hw.nperflevels` to see if they're present.  If so,
	 * read and validate them.
	 */
	struct mach_perflevel * perflevels = NULL;
	const uint32_t nperflevels = get_sys_info_by_name("hw.nperflevels");
	if (nperflevels > 1) {
		perflevels = read_perflevels(nperflevels);

		if (!perflevels) {
			cpuinfo_log_error("failed to initialize perflevels");
			goto cleanup;
		}

		/* Double-check topology counts */
		uint32_t cores = 0;
		uint32_t threads = 0;
		for (uint32_t i=0; i<nperflevels; ++i) {
			cores += perflevels[i].physicalcpu_max;
			threads += perflevels[i].logicalcpu_max;
		}

		if (mach_topology.cores != cores) {
			cpuinfo_log_error("mismatch topology core count (%"PRIu32" != %"PRIu32").",
				mach_topology.cores, cores);
			goto cleanup;
		}

		if (mach_topology.threads != threads) {
			cpuinfo_log_error("mismatch topology thread count (%"PRIu32" != %"PRIu32").",
				mach_topology.threads, threads);
			goto cleanup;
		}
	}

	/* Allocate working set of arrays that we'll populate */

	processors = calloc(mach_topology.threads, sizeof(*processors));
	if (processors == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" logical processors",
			mach_topology.threads * sizeof(*processors), mach_topology.threads);
		goto cleanup;
	}
	cores = calloc(mach_topology.cores, sizeof(*cores));
	if (cores == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" cores",
			mach_topology.cores * sizeof(*cores), mach_topology.cores);
		goto cleanup;
	}
	packages = calloc(mach_topology.packages, sizeof(*packages));
	if (packages == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for descriptions of %"PRIu32" packages",
			mach_topology.packages * sizeof(*packages), mach_topology.packages);
		goto cleanup;
	}

	/* Populate packages, cores, clusters, and partially processors */

	const uint32_t threads_per_core = mach_topology.threads / mach_topology.cores;
	const uint32_t threads_per_package = mach_topology.threads / mach_topology.packages;
	const uint32_t cores_per_package = mach_topology.cores / mach_topology.packages;

	char package_name[CPUINFO_PACKAGE_NAME_MAX] = {0};
	decode_package_name(package_name);

	for (uint32_t i = 0; i < mach_topology.packages; i++) {
		packages[i] = (struct cpuinfo_package) {
			.processor_start = i * threads_per_package,
			.processor_count = threads_per_package,
			.core_start = i * cores_per_package,
			.core_count = cores_per_package,
		};
		memmove(packages[i].name, package_name, CPUINFO_PACKAGE_NAME_MAX);
	}

	uint32_t num_clusters = 1;
	for (uint32_t i = 0; i < mach_topology.cores; i++) {
		cores[i] = (struct cpuinfo_core) {
			.processor_start = i * threads_per_core,
			.processor_count = threads_per_core,
			.core_id = i % cores_per_package,
			.package = packages + i / cores_per_package,
			.vendor = cpuinfo_vendor_apple,
			.uarch = decode_uarch(cpu_family, cpu_subtype, i, mach_topology.cores),
		};
		if (i != 0 && cores[i].uarch != cores[i - 1].uarch) {
			num_clusters++;
		}
	}
	for (uint32_t i = 0; i < mach_topology.threads; i++) {
		const uint32_t smt_id = i % threads_per_core;
		const uint32_t core_id = i / threads_per_core;
		const uint32_t package_id = i / threads_per_package;

		processors[i].smt_id = smt_id;
		processors[i].core = &cores[core_id];
		processors[i].package = &packages[package_id];
	}

	/*
	 * If we're running on a newer OS that supports perflevels, verify
	 * number of perflevels == number of clusters.
	 */
	if (nperflevels > 0 && perflevels) {
		if (nperflevels != num_clusters) {
			cpuinfo_log_error("mismatch topology cluster count (%"PRIu32" != %"PRIu32").",
				nperflevels, num_clusters);
			goto cleanup;
		}
	}

	clusters = calloc(num_clusters, sizeof(*clusters));
	if (clusters == NULL) {
		cpuinfo_log_error(
			"failed to allocate %zu bytes for descriptions of %"PRIu32" clusters",
			num_clusters * sizeof(*clusters), num_clusters);
		goto cleanup;
	}
	uarchs = calloc(num_clusters, sizeof(*uarchs));
	if (uarchs == NULL) {
		cpuinfo_log_error(
			"failed to allocate %zu bytes for descriptions of %"PRIu32" uarchs",
			num_clusters * sizeof(*uarchs), num_clusters);
		goto cleanup;
	}
	uint32_t cluster_idx = UINT32_MAX;
	for (uint32_t i = 0; i < mach_topology.cores; i++) {
		if (i == 0 || cores[i].uarch != cores[i - 1].uarch) {
			cluster_idx++;
			uarchs[cluster_idx] = (struct cpuinfo_uarch_info) {
				.uarch = cores[i].uarch,
				.processor_count = 1,
				.core_count = 1,
			};
			clusters[cluster_idx] = (struct cpuinfo_cluster) {
				.processor_start = i * threads_per_core,
				.processor_count = 1,
				.core_start = i,
				.core_count = 1,
				.cluster_id = cluster_idx,
				.package = cores[i].package,
				.vendor = cores[i].vendor,
				.uarch = cores[i].uarch,
			};
		} else {
			uarchs[cluster_idx].processor_count++;
			uarchs[cluster_idx].core_count++;
			clusters[cluster_idx].processor_count++;
			clusters[cluster_idx].core_count++;
		}
		cores[i].cluster = &clusters[cluster_idx];
	}

	for (uint32_t i = 0; i < mach_topology.threads; i++) {
		const uint32_t core_id = i / threads_per_core;
		processors[i].cluster = cores[core_id].cluster;
	}

	for (uint32_t i = 0; i < mach_topology.packages; i++) {
		packages[i].cluster_start = 0;
		packages[i].cluster_count = num_clusters;
	}

	/* Detect and populate caches */

	/*
	 * Prefer perflevels to detect caches.  Fallback on error or if
	 * perflevels are not available.
	 */
	bool cachesDetected = false;
	if (nperflevels > 0 && perflevels) {
		cachesDetected = detect_caches_using_perflevels(mach_topology, perflevels, nperflevels, &l1i, &l1d, &l2, &l3);
	}

	if (!cachesDetected) {
		cachesDetected = detect_caches_legacy(mach_topology, &l1i, &l1d, &l2, &l3);
		if (!cachesDetected) {
			goto cleanup;
		}
	}

	/* Associate processors with caches */

	if (l1i.caches && l1i.count > 0) {
		for (uint32_t c=0; c<l1i.count; ++c) {
			if (l1i.caches[c].processor_count < 1) {
				cpuinfo_log_error("invalid L1I cache (processor_count < 1)");
				goto cleanup;
			}

			uint32_t processor_start = l1i.caches[c].processor_start;
			uint32_t processor_end = processor_start + l1i.caches[c].processor_count;
			for (uint32_t p=processor_start; p<processor_end; ++p) {
				processors[p].cache.l1i = &l1i.caches[c];
			}
		}
	}

	if (l1d.caches && l1d.count > 0) {
		for (uint32_t c=0; c<l1d.count; ++c) {
			if (l1d.caches[c].processor_count < 1) {
				cpuinfo_log_error("invalid L1D cache (processor_count < 1)");
				goto cleanup;
			}

			uint32_t processor_start = l1d.caches[c].processor_start;
			uint32_t processor_end = processor_start + l1d.caches[c].processor_count;
			for (uint32_t p=processor_start; p<processor_end; ++p) {
				processors[p].cache.l1d = &l1d.caches[c];
			}
		}
	}

	if (l2.caches && l2.count > 0) {
		for (uint32_t c=0; c<l2.count; ++c) {
			if (l2.caches[c].processor_count < 1) {
				cpuinfo_log_error("invalid L2 cache (processor_count < 1)");
				goto cleanup;
			}

			uint32_t processor_start = l2.caches[c].processor_start;
			uint32_t processor_end = processor_start + l2.caches[c].processor_count;
			for (uint32_t p=processor_start; p<processor_end; ++p) {
				processors[p].cache.l2 = &l2.caches[c];
			}
		}
	}

	if (l3.caches && l3.count > 0) {
		for (uint32_t c=0; c<l3.count; ++c) {
			if (l3.caches[c].processor_count < 1) {
				cpuinfo_log_error("invalid L3 cache (processor_count < 1)");
				goto cleanup;
			}

			uint32_t processor_start = l3.caches[c].processor_start;
			uint32_t processor_end = processor_start + l3.caches[c].processor_count;
			for (uint32_t p=processor_start; p<processor_end; ++p) {
				processors[p].cache.l3 = &l3.caches[c];
			}
		}
	}


	/* Commit changes */
	cpuinfo_processors = processors;
	cpuinfo_cores = cores;
	cpuinfo_clusters = clusters;
	cpuinfo_packages = packages;
	cpuinfo_uarchs = uarchs;
	cpuinfo_cache[cpuinfo_cache_level_1i] = l1i.caches;
	cpuinfo_cache[cpuinfo_cache_level_1d] = l1d.caches;
	cpuinfo_cache[cpuinfo_cache_level_2]  = l2.caches;
	cpuinfo_cache[cpuinfo_cache_level_3]  = l3.caches;

	cpuinfo_processors_count = mach_topology.threads;
	cpuinfo_cores_count = mach_topology.cores;
	cpuinfo_clusters_count = num_clusters;
	cpuinfo_packages_count = mach_topology.packages;
	cpuinfo_uarchs_count = num_clusters;
	cpuinfo_cache_count[cpuinfo_cache_level_1i] = l1i.count;
	cpuinfo_cache_count[cpuinfo_cache_level_1d] = l1d.count;
	cpuinfo_cache_count[cpuinfo_cache_level_2]  = l2.count;
	cpuinfo_cache_count[cpuinfo_cache_level_3]  = l3.count;
	cpuinfo_max_cache_size = cpuinfo_compute_max_cache_size(&processors[0]);

	__sync_synchronize();

	cpuinfo_is_initialized = true;

	processors = NULL;
	cores = NULL;
	clusters = NULL;
	packages = NULL;
	uarchs = NULL;
	l1i.caches = l1d.caches = l2.caches = l3.caches = NULL;

cleanup:
	free(perflevels);
	free(processors);
	free(cores);
	free(clusters);
	free(packages);
	free(uarchs);
	free(l1i.caches);
	free(l1d.caches);
	free(l2.caches);
	free(l3.caches);
}
