#pragma once
#ifndef CPUINFO_H
#define CPUINFO_H

#ifndef __cplusplus
	#include <stdbool.h>
#endif

#include <stdint.h>

/* Identify architecture and define corresponding macro */

#if defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(_M_IX86)
	#define CPUINFO_ARCH_X86 1
#endif

#if defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
	#define CPUINFO_ARCH_X86_64 1
#endif

#if defined(__arm__) || defined(_M_ARM)
	#define CPUINFO_ARCH_ARM 1
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
	#define CPUINFO_ARCH_ARM64 1
#endif

#if defined(__PPC64__) || defined(__powerpc64__) || defined(_ARCH_PPC64)
	#define CPUINFO_ARCH_PPC64 1
#endif

#if defined(__pnacl__)
	#define CPUINFO_ARCH_PNACL 1
#endif

#if defined(EMSCRIPTEN)
	#define CPUINFO_ARCH_ASMJS 1
#endif

#if CPUINFO_ARCH_X86 && defined(_MSC_VER)
	#define CPUINFO_ABI __cdecl
#elif CPUINFO_ARCH_X86 && defined(__GNUC__)
	#define CPUINFO_ABI __attribute__((__cdecl__))
#else
	#define CPUINFO_ABI
#endif

/* Define other architecture-specific macros as 0 */

#ifndef CPUINFO_ARCH_X86
	#define CPUINFO_ARCH_X86 0
#endif

#ifndef CPUINFO_ARCH_X86_64
	#define CPUINFO_ARCH_X86_64 0
#endif

#ifndef CPUINFO_ARCH_ARM
	#define CPUINFO_ARCH_ARM 0
#endif

#ifndef CPUINFO_ARCH_ARM64
	#define CPUINFO_ARCH_ARM64 0
#endif

#ifndef CPUINFO_ARCH_PPC64
	#define CPUINFO_ARCH_PPC64 0
#endif

#ifndef CPUINFO_ARCH_PNACL
	#define CPUINFO_ARCH_PNACL 0
#endif

#ifndef CPUINFO_ARCH_ASMJS
	#define CPUINFO_ARCH_ASMJS 0
#endif

#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	struct cpuinfo_x86_isa {
		#if CPUINFO_ARCH_X86
			bool rdtsc;
		#endif
		bool sysenter;
		#if CPUINFO_ARCH_X86
			bool syscall;
		#endif
		bool msr;
		bool clzero;
		bool clflush;
		bool clflushopt;
		bool mwait;
		bool mwaitx;
		#if CPUINFO_ARCH_X86
			bool emmx;
		#endif
		bool fxsave;
		bool xsave;
		#if CPUINFO_ARCH_X86
			bool fpu;
			bool mmx;
			bool mmx_plus;
		#endif
		bool three_d_now;
		bool three_d_now_plus;
		#if CPUINFO_ARCH_X86
			bool three_d_now_geode;
		#endif
		bool prefetch;
		bool prefetchw;
		bool prefetchwt1;
		#if CPUINFO_ARCH_X86
			bool daz;
			bool sse;
			bool sse2;
		#endif
		bool sse3;
		bool ssse3;
		bool sse4_1;
		bool sse4_2;
		bool sse4a;
		bool misaligned_sse;
		bool avx;
		bool fma3;
		bool fma4;
		bool xop;
		bool f16c;
		bool avx2;
		bool avx512f;
		bool avx512pf;
		bool avx512er;
		bool avx512cd;
		bool avx512dq;
		bool avx512bw;
		bool avx512vl;
		bool avx512ifma;
		bool avx512vbmi;
		bool avx512vpopcntdq;
		bool avx512_4vnniw;
		bool avx512_4fmaps;
		bool hle;
		bool rtm;
		bool xtest;
		bool mpx;
		#if CPUINFO_ARCH_X86
			bool cmov;
			bool cmpxchg8b;
		#endif
		bool cmpxchg16b;
		bool clwb;
		bool movbe;
		#if CPUINFO_ARCH_X86_64
			bool lahf_sahf;
		#endif
		bool fs_gs_base;
		bool lzcnt;
		bool popcnt;
		bool tbm;
		bool bmi;
		bool bmi2;
		bool adx;
		bool aes;
		bool pclmulqdq;
		bool rdrand;
		bool rdseed;
		bool sha;
		bool rng;
		bool ace;
		bool ace2;
		bool phe;
		bool pmm;
		bool lwp;
		bool rdtscp;
		bool rdpid;
	};

	struct cpuinfo_x86_model_info {
		uint32_t model;
		uint32_t family;

		uint32_t base_model;
		uint32_t base_family;
		uint32_t stepping;
		uint32_t extended_model;
		uint32_t extended_family;
		uint32_t processor_type;
	};
#endif

#if CPUINFO_ARCH_ARM
	struct cpuinfo_arm_isa {
		bool thumb;
		bool thumb2;
		bool thumbee;
		bool jazelle;
		bool armv5e;
		bool armv6;
		bool armv6k;
		bool armv7;
		bool armv7mp;
		bool idiv;

		bool vfpv2;
		bool vfpv3;
		bool d32;
		bool fp16;
		bool fma;

		bool wmmx;
		bool wmmx2;
		bool neon;

		bool aes;
		bool sha1;
		bool sha2;
		bool pmull;
		bool crc32;
	};

	struct cpuinfo_arm_model_info {
		uint16_t implementer;
		uint16_t variant;
		uint16_t part;
		uint16_t revision;
	};
#endif

#define CPUINFO_CACHE_UNIFIED          0x00000001
#define CPUINFO_CACHE_INCLUSIVE        0x00000002
#define CPUINFO_CACHE_COMPLEX_INDEXING 0x00000004

enum cpuinfo_cache_level {
	cpuinfo_cache_level_1i  = 0,
	cpuinfo_cache_level_1d  = 1,
	cpuinfo_cache_level_2   = 2,
	cpuinfo_cache_level_3   = 3,
	cpuinfo_cache_level_4   = 4,
	cpuinfo_cache_level_max = 5,
};

struct cpuinfo_cache {
	/** Cache size in bytes */
	uint32_t size;
	/** Number of ways of associativity */
	uint32_t associativity;
	/** Number of sets */
	uint32_t sets;
	/** Number of partitions */
	uint32_t partitions;
	/** Line size in bytes */
	uint32_t line_size;
	/**
	 * Binary characteristics of the cache (unified cache, inclusive cache, cache with complex indexing).
	 *
	 * @see CPUINFO_CACHE_UNIFIED, CPUINFO_CACHE_INCLUSIVE, CPUINFO_CACHE_COMPLEX_INDEXING
	 */
	uint32_t flags;
	/** Index of the first logical processor that shares this cache */
	uint32_t thread_start;
	/** Number of logical processors that share this cache */
	uint32_t thread_count;
};

struct cpuinfo_caches {
	uint32_t count;
	const struct cpuinfo_cache* instances;
};

struct cpuinfo_trace_cache {
	uint32_t uops;
	uint32_t associativity;
};

#define CPUINFO_PAGE_SIZE_4KB  0x1000
#define CPUINFO_PAGE_SIZE_1MB  0x100000
#define CPUINFO_PAGE_SIZE_2MB  0x200000
#define CPUINFO_PAGE_SIZE_4MB  0x400000
#define CPUINFO_PAGE_SIZE_16MB 0x1000000
#define CPUINFO_PAGE_SIZE_1GB  0x40000000

struct cpuinfo_tlb {
	uint32_t entries;
	uint32_t associativity;
	uint64_t pages;
};

/** Vendor of processor core design */
enum cpuinfo_vendor {
	/** Processor vendor is not known to the library, or the library failed to get vendor information from the OS. */
	cpuinfo_vendor_unknown = 0,
	
	/* Active vendors of modern CPUs */

	/**
	 * Intel Corporation. Vendor of x86, x86-64, IA64, and ARM processor microarchitectures.
	 *
	 * Sold its ARM design subsidiary in 2006. The last ARM processor design was released in 2004.
	 */
	cpuinfo_vendor_intel    = 1,
	/** Advanced Micro Devices, Inc. Vendor of x86 and x86-64 processor microarchitectures. */
	cpuinfo_vendor_amd      = 2,
	/** ARM Holdings plc. Vendor of ARM and ARM64 processor microarchitectures. */
	cpuinfo_vendor_arm      = 3,
	/** Qualcomm Incorporated. Vendor of ARM and ARM64 processor microarchitectures. */
	cpuinfo_vendor_qualcomm = 4,
	/** Apple Inc. Vendor of ARM and ARM64 processor microarchitectures. */
	cpuinfo_vendor_apple    = 5,
	/** Samsung Electronics Co., Ltd. Vendir if ARM64 processor microarchitectures. */
	cpuinfo_vendor_samsung  = 6,
	/** nVidia Corporation. Vendor of ARM64-compatible processor microarchitectures. */
	cpuinfo_vendor_nvidia   = 7,
	/** MIPS Technologies, Inc. Vendor of MIPS processor microarchitectures. */
	cpuinfo_vendor_mips     = 8,
	/** International Business Machines Corporation. Vendor of PowerPC processor microarchitectures. */
	cpuinfo_vendor_ibm      = 9,
	/** Ingenic Semiconductor. Vendor of MIPS processor microarchitectures. */
	cpuinfo_vendor_ingenic  = 10,
	/**
	 * VIA Technologies, Inc. Vendor of x86 and x86-64 processor microarchitectures.
	 *
	 * Processors are designed by Centaur Technology, a subsidiary of VIA Technologies.
	 */
	cpuinfo_vendor_via      = 11,

	/* Active vendors of embedded CPUs */

	/** Texas Instruments Inc. Vendor of ARM processor microarchitectures. */
	cpuinfo_vendor_texas_instruments = 30,
	/** Marvell Technology Group Ltd. Vendor of ARM processor microarchitectures. */
	cpuinfo_vendor_marvell           = 31,
	/** RDC Semiconductor Co., Ltd. Vendor of x86 processor microarchitectures. */
	cpuinfo_vendor_rdc               = 32,
	/** DM&P Electronics Inc. Vendor of x86 processor microarchitectures. */
	cpuinfo_vendor_dmp               = 33,
	/** Motorola, Inc. Vendor of PowerPC and ARM processor microarchitectures. */
	cpuinfo_vendor_motorola          = 34,

	/* Defunct CPU vendors */
		
	/**
	 * Transmeta Corporation. Vendor of x86 processor microarchitectures.
	 *
	 * Now defunct. The last processor design was released in 2004.
	 * Transmeta processors implemented VLIW ISA and used binary translation to execute x86 code.
	 */
	cpuinfo_vendor_transmeta = 50,
	/**
	 * Cyrix Corporation. Vendor of x86 processor microarchitectures.
	 *
	 * Now defunct. The last processor design was released in 1996.
	 */
	cpuinfo_vendor_cyrix     = 51,
	/**
	 * Rise Technology. Vendor of x86 processor microarchitectures.
	 *
	 * Now defunct. The last processor design was released in 1999.
	 */
	cpuinfo_vendor_rise      = 52,
	/**
	 * National Semiconductor. Vendor of x86 processor microarchitectures.
	 *
	 * Sold its x86 design subsidiary in 1999. The last processor design was released in 1998.
	 */
	cpuinfo_vendor_nsc       = 53,
	/**
	 * Silicon Integrated Systems. Vendor of x86 processor microarchitectures.
	 *
	 * Sold its x86 design subsidiary in 2001. The last processor design was released in 2001.
	 */
	cpuinfo_vendor_sis       = 54,
	/**
	 * NexGen. Vendor of x86 processor microarchitectures.
	 *
	 * Now defunct. The last processor design was released in 1994.
	 * NexGen designed the first x86 microarchitecture which decomposed x86 instructions into simple microoperations.
	 */
	cpuinfo_vendor_nexgen    = 55,
	/**
	 * United Microelectronics Corporation. Vendor of x86 processor microarchitectures.
	 *
	 * Ceased x86 in the early 1990s. The last processor design was released in 1991.
	 * Designed U5C and U5D processors. Both are 486 level.
	 */
	cpuinfo_vendor_umc       = 56,
	/**
	 * Digital Equipment Corporation. Vendor of ARM processor microarchitecture.
	 *
	 * Sold its ARM designs in 1997. The last processor design was released in 1997.
	 */
	cpuinfo_vendor_dec       = 57,
};

/**
 * Processor microarchitecture
 *
 * Processors with different microarchitectures often have different instruction performance characteristics,
 * and may have dramatically different pipeline organization.
 */
enum cpuinfo_uarch {
	/** Microarchitecture is unknown, or the library failed to get information about the microarchitecture from OS */
	cpuinfo_uarch_unknown = 0,

	/** Pentium and Pentium MMX microarchitecture. */
	cpuinfo_uarch_p5    = 0x00100100,
	/** Intel Quark microarchitecture. */
	cpuinfo_uarch_quark = 0x00100101,

	/** Pentium Pro, Pentium II, and Pentium III. */
	cpuinfo_uarch_p6           = 0x00100200,
	/** Pentium M. */
	cpuinfo_uarch_dothan       = 0x00100201,
	/** Intel Core microarchitecture. */
	cpuinfo_uarch_yonah        = 0x00100202,
	/** Intel Core 2 microarchitecture on 65 nm process. */
	cpuinfo_uarch_conroe       = 0x00100203,
	/** Intel Core 2 microarchitecture on 45 nm process. */
	cpuinfo_uarch_penryn       = 0x00100204,
	/** Intel Nehalem and Westmere microarchitectures (Core i3/i5/i7 1st gen). */
	cpuinfo_uarch_nehalem      = 0x00100205,
	/** Intel Sandy Bridge microarchitecture (Core i3/i5/i7 2nd gen). */
	cpuinfo_uarch_sandy_bridge = 0x00100206,
	/** Intel Ivy Bridge microarchitecture (Core i3/i5/i7 3rd gen). */
	cpuinfo_uarch_ivy_bridge   = 0x00100207,
	/** Intel Haswell microarchitecture (Core i3/i5/i7 4th gen). */
	cpuinfo_uarch_haswell      = 0x00100208,
	/** Intel Broadwell microarchitecture. */
	cpuinfo_uarch_broadwell    = 0x00100209,
	/** Intel Sky Lake microarchitecture. */
	cpuinfo_uarch_sky_lake     = 0x0010020A,
	/** Intel Kaby Lake microarchitecture. */
	cpuinfo_uarch_kaby_lake    = 0x0010020B,

	/** Pentium 4 with Willamette, Northwood, or Foster cores. */
	cpuinfo_uarch_willamette = 0x00100300,
	/** Pentium 4 with Prescott and later cores. */
	cpuinfo_uarch_prescott   = 0x00100301,

	/** Intel Atom on 45 nm process. */
	cpuinfo_uarch_bonnell    = 0x00100400,
	/** Intel Atom on 32 nm process. */
	cpuinfo_uarch_saltwell   = 0x00100401,
	/** Intel Silvermont microarchitecture (22 nm out-of-order Atom). */
	cpuinfo_uarch_silvermont = 0x00100402,
	/** Intel Airmont microarchitecture (14 nm out-of-order Atom). */
	cpuinfo_uarch_airmont    = 0x00100403,

	/** Intel Knights Ferry HPC boards. */
	cpuinfo_uarch_knights_ferry   = 0x00100500,
	/** Intel Knights Corner HPC boards (aka Xeon Phi). */
	cpuinfo_uarch_knights_corner  = 0x00100501,
	/** Intel Knights Landing microarchitecture (second-gen MIC). */
	cpuinfo_uarch_knights_landing = 0x00100502,
	/** Intel Knights Hill microarchitecture (third-gen MIC). */
	cpuinfo_uarch_knights_hill    = 0x00100503,
	/** Intel Knights Mill Xeon Phi. */
	cpuinfo_uarch_knights_mill    = 0x00100504,

	/** Intel/Marvell XScale series. */
	cpuinfo_uarch_xscale = 0x00100600,

	/** AMD K5. */
	cpuinfo_uarch_k5        = 0x00200100,
	/** AMD K6 and alike. */
	cpuinfo_uarch_k6        = 0x00200101,
	/** AMD Athlon and Duron. */
	cpuinfo_uarch_k7        = 0x00200102,
	/** AMD Athlon 64, Opteron 64. */
	cpuinfo_uarch_k8        = 0x00200103,
	/** AMD Family 10h (Barcelona, Istambul, Magny-Cours). */
	cpuinfo_uarch_k10       = 0x00200104,
	/**
	 * AMD Bulldozer microarchitecture
	 * Zambezi FX-series CPUs, Zurich, Valencia and Interlagos Opteron CPUs.
	 */
	cpuinfo_uarch_bulldozer = 0x00200105,
	/**
	 * AMD Piledriver microarchitecture
	 * Vishera FX-series CPUs, Trinity and Richland APUs, Delhi, Seoul, Abu Dhabi Opteron CPUs.
	 */
	cpuinfo_uarch_piledriver  = 0x00200106,
	/** AMD Steamroller microarchitecture (Kaveri APUs). */
	cpuinfo_uarch_steamroller = 0x00200107,
	/** AMD Excavator microarchitecture (Carizzo APUs). */
	cpuinfo_uarch_excavator   = 0x00200108,
	/** AMD Zen microarchitecture (Ryzen CPUs). */
	cpuinfo_uarch_zen         = 0x00200109,

	/** NSC Geode and AMD Geode GX and LX. */
	cpuinfo_uarch_geode  = 0x00200200,
	/** AMD Bobcat mobile microarchitecture. */
	cpuinfo_uarch_bobcat = 0x00200201,
	/** AMD Jaguar mobile microarchitecture. */
	cpuinfo_uarch_jaguar = 0x00200202,
	
	/** ARM7 series. */
	cpuinfo_uarch_arm7  = 0x00300100,
	/** ARM9 series. */
	cpuinfo_uarch_arm9  = 0x00300101,
	/** ARM 1136, ARM 1156, ARM 1176, or ARM 11MPCore. */
	cpuinfo_uarch_arm11 = 0x00300102,

	/** ARM Cortex-A5. */
	cpuinfo_uarch_cortex_a5  = 0x00300205,
	/** ARM Cortex-A7. */
	cpuinfo_uarch_cortex_a7  = 0x00300207,
	/** ARM Cortex-A8. */
	cpuinfo_uarch_cortex_a8  = 0x00300208,
	/** ARM Cortex-A9. */
	cpuinfo_uarch_cortex_a9  = 0x00300209,
	/** ARM Cortex-A12. */
	cpuinfo_uarch_cortex_a12 = 0x00300212,
	/** ARM Cortex-A15. */
	cpuinfo_uarch_cortex_a15 = 0x00300215,
	/** ARM Cortex-A17. */
	cpuinfo_uarch_cortex_a17 = 0x00300217,

	/** ARM Cortex-A32. */
	cpuinfo_uarch_cortex_a32 = 0x00300332,
	/** ARM Cortex-A35. */
	cpuinfo_uarch_cortex_a35 = 0x00300335,
	/** ARM Cortex-A53. */
	cpuinfo_uarch_cortex_a53 = 0x00300353,
	/** ARM Cortex-A57. */
	cpuinfo_uarch_cortex_a57 = 0x00300357,
	/** ARM Cortex-A72. */
	cpuinfo_uarch_cortex_a72 = 0x00300372,
	/** ARM Cortex-A73. */
	cpuinfo_uarch_cortex_a73 = 0x00300373,

	/** Qualcomm Scorpion. */
	cpuinfo_uarch_scorpion = 0x00400100,
	/** Qualcomm Krait. */
	cpuinfo_uarch_krait    = 0x00400101,
	/** Qualcomm Kryo. */
	cpuinfo_uarch_kryo     = 0x00400102,

	/** nVidia Denver. */
	cpuinfo_uarch_denver   = 0x00500100,

	/** Samsung Mongoose. */
	cpuinfo_uarch_mongoose = 0x00600100,

	/** Apple A6 and A6X processors. */
	cpuinfo_uarch_swift     = 0x00700100,
	/** Apple A7 processor. */
	cpuinfo_uarch_cyclone   = 0x00700101,
	/** Apple A8 processor. */
	cpuinfo_uarch_typhoon   = 0x00700102,
	/** Apple A9 processor. */
	cpuinfo_uarch_twister   = 0x00700103,
	/** Apple A10 processor. */
	cpuinfo_uarch_hurricane = 0x00700104,
};

struct cpuinfo_topology {
	/** Thread (hyperthread, or SMT) ID within a core */
	uint32_t thread_id;
	/** Core ID within a package */
	uint32_t core_id;
	/* Package (socket) ID */
	uint32_t package_id;
	#if defined(__linux__)
		int linux_id;
	#endif
	#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
		uint32_t apic_id;
	#endif
};

struct cpuinfo_processor {
	enum cpuinfo_vendor vendor;
	enum cpuinfo_uarch uarch;
	struct cpuinfo_topology topology;
	struct {
		const struct cpuinfo_trace_cache* trace;
		const struct cpuinfo_cache* l1i;
		const struct cpuinfo_cache* l1d;
		const struct cpuinfo_cache* l2;
		const struct cpuinfo_cache* l3;
		const struct cpuinfo_cache* l4;
	} cache;
};

struct cpuinfo_core {
	uint32_t thread_start;
	uint32_t thread_count;
};

struct cpuinfo_package {
	uint32_t thread_start;
	uint32_t thread_count;
	uint32_t core_count;
	#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
		struct cpuinfo_x86_model_info model_info;
	#endif
};

#ifdef __cplusplus
extern "C" {
#endif

void CPUINFO_ABI cpuinfo_initialize(void);

void CPUINFO_ABI cpuinfo_deinitialize(void);

#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	extern struct cpuinfo_x86_isa cpuinfo_isa;
#endif

#if CPUINFO_ARCH_ARM
	extern struct cpuinfo_arm_isa cpuinfo_isa;
#endif

struct cpuinfo_caches CPUINFO_ABI cpuinfo_get_l1i_cache(void);
struct cpuinfo_caches CPUINFO_ABI cpuinfo_get_l1d_cache(void);
struct cpuinfo_caches CPUINFO_ABI cpuinfo_get_l2_cache(void);
struct cpuinfo_caches CPUINFO_ABI cpuinfo_get_l3_cache(void);
struct cpuinfo_caches CPUINFO_ABI cpuinfo_get_l4_cache(void);

extern struct cpuinfo_processor* cpuinfo_processors;
extern struct cpuinfo_cores* cpuinfo_cores;
extern struct cpuinfo_package* cpuinfo_packages;


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CPUINFO_H */
