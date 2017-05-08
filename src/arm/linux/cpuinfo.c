#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>

#if CPUINFO_MOCK
	#include <cpuinfo-mock.h>
#endif
#include <arm/linux/api.h>
#include <log.h>


static const char* proc_cpuinfo_path = "/proc/cpuinfo";

#if CPUINFO_MOCK
	void cpuinfo_set_proc_cpuinfo_path(const char* path) {
		/* Note: this leaks memory */
		proc_cpuinfo_path = strdup(path);
	}
#endif


/*
 * Size, in chars, of the on-stack buffer used for parsing cpu lists.
 * This is also the limit on the length of a single entry
 * (<cpu-number> or <cpu-number-start>-<cpu-number-end>)
 * in the cpu list.
 */
#define BUFFER_SIZE 256


static uint32_t parse_processor_number(
	const char* processor_start,
	const char* processor_end,
	struct proc_cpuinfo proc_cpuinfo[restrict static 1])
{
	const size_t processor_length = (size_t) (processor_end - processor_start);

	if (processor_length == 0) {
		cpuinfo_log_warning("Processor number in /proc/cpuinfo is ignored: string is empty");
		return 0;
	}

	uint32_t processor_number = 0;
	for (const char* digit_ptr = processor_start; digit_ptr != processor_end; digit_ptr++) {
		const uint32_t digit = (uint32_t) (*digit_ptr - '0');
		if (digit > 10) {
			cpuinfo_log_warning("non-decimal suffix %.*s in /proc/cpuinfo processor number is ignored",
				(int) (processor_end - digit_ptr), digit_ptr);
			break;
		}

		processor_number = processor_number * 10 + digit;
	}

	return processor_number;
}

/*
 *	Full list of ARM features reported in /proc/cpuinfo:
 *	
 *	* swp - support for SWP instruction (deprecated in ARMv7, can be removed in future)
 *	* half - support for half-word loads and stores. These instruction are part of ARMv4,
 *	         so no need to check it on supported CPUs.
 *	* thumb - support for 16-bit Thumb instruction set. Note that BX instruction is detected
 *	          by ARMv4T architecture, not by this flag.
 *	* 26bit - old CPUs merged 26-bit PC and program status register (flags) into 32-bit PC
 *	          and had special instructions for working with packed PC. Now it is all deprecated.
 *	* fastmult - most old ARM CPUs could only compute 2 bits of multiplication result per clock
 *	             cycle, but CPUs with M suffix (e.g. ARM7TDMI) could compute 4 bits per cycle.
 *	             Of course, now it makes no sense.
 *	* fpa - floating point accelerator available. On original ARM ABI all floating-point operations
 *	        generated FPA instructions. If FPA was not available, these instructions generated
 *	        "illegal operation" interrupts, and the OS processed them by emulating the FPA instructions.
 *	        Debian used this ABI before it switched to EABI. Now FPA is deprecated.
 *	* vfp - vector floating point instructions. Available on most modern CPUs (as part of VFPv3).
 *	        Required by Android ARMv7A ABI and by Ubuntu on ARM.
 *              Note: there is no flag for VFPv2.
 *	* edsp - V5E instructions: saturating add/sub and 16-bit x 16-bit -> 32/64-bit multiplications.
 *	         Required on Android, supported by all CPUs in production.
 *	* java - Jazelle extension. Supported on most CPUs.
 *	* iwmmxt - Intel/Marvell Wireless MMX instructions. 64-bit integer SIMD.
 *	           Supported on XScale (Since PXA270) and Sheeva (PJ1, PJ4) architectures.
 *	           Note that there is no flag for WMMX2 instructions.
 *	* crunch - Maverick Crunch instructions. Junk.
 *	* thumbee - ThumbEE instructions. Almost no documentation is available.
 *	* neon - NEON instructions (aka Advanced SIMD). MVFR1 register gives more
 *	         fine-grained information on particular supported features, but
 *	         the Linux kernel exports only a single flag for all of them.
 *	         According to ARMv7A docs it also implies the availability of VFPv3
 *	         (with 32 double-precision registers d0-d31).
 *	* vfpv3 - VFPv3 instructions. Available on most modern CPUs. Augment VFPv2 by
 *	          conversion to/from integers and load constant instructions.
 *	          Required by Android ARMv7A ABI and by Ubuntu on ARM.
 *	* vfpv3d16 - VFPv3 instructions with only 16 double-precision registers (d0-d15).
 *	* tls - software thread ID registers.
 *	        Used by kernel (and likely libc) for efficient implementation of TLS.
 *	* vfpv4 - fused multiply-add instructions.
 *	* idiva - DIV instructions available in ARM mode.
 *	* idivt - DIV instructions available in Thumb mode.
 *  * vfpd32 - VFP (of any version) with 32 double-precision registers d0-d31.
 *  * lpae - Large Physical Address Extension (physical address up to 40 bits).
 *  * evtstrm - generation of Event Stream by timer.
 *  * aes - AES instructions.
 *  * pmull - Polinomial Multiplication instructions.
 *  * sha1 - SHA1 instructions.
 *  * sha2 - SHA2 instructions.
 *  * crc32 - CRC32 instructions.
 *
 *	/proc/cpuinfo on ARM is populated in file arch/arm/kernel/setup.c in Linux kernel
 *	Note that some devices may use patched Linux kernels with different feature names.
 *	However, the names above were checked on a large number of /proc/cpuinfo listings.
 */
static void parse_features(
	const char* features_start,
	const char* features_end,
	struct proc_cpuinfo proc_cpuinfo[restrict static 1])
{
	const char* feature_start = features_start;
	const char* feature_end;

	/* Mark the features as valid */
	proc_cpuinfo->valid_mask |= PROC_CPUINFO_VALID_FEATURES;

	do {
		feature_end = feature_start + 1;
		for (; feature_end != features_end; feature_end++) {
			if (*feature_end == ' ') {
				break;
			}
		}
		const size_t feature_length = (size_t) (feature_end - feature_start);
		
		switch (feature_length) {
			case 3:
				if (memcmp(feature_start, "swp", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_SWP;
				} else if (memcmp(feature_start, "fpa", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_FPA;
				} else if (memcmp(feature_start, "vfp", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_VFP;
				} else if (memcmp(feature_start, "tls", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_TLS;
				} else if (memcmp(feature_start, "aes", feature_length) == 0) {
					proc_cpuinfo->features2 |= PROC_CPUINFO_FEATURE2_AES;
				} else {
					goto unexpected;
				}
				break;
			case 4:
				if (memcmp(feature_start, "half", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_HALF;
				} else if (memcmp(feature_start, "edsp", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_EDSP;
				} else if (memcmp(feature_start, "java", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_JAVA;
				} else if (memcmp(feature_start, "neon", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_NEON;
				} else if (memcmp(feature_start, "lpae", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_LPAE;
				} else if (memcmp(feature_start, "sha1", feature_length) == 0) {
					proc_cpuinfo->features2 |= PROC_CPUINFO_FEATURE2_SHA1;
				} else if (memcmp(feature_start, "sha2", feature_length) == 0) {
					proc_cpuinfo->features2 |= PROC_CPUINFO_FEATURE2_SHA2;
				} else {
					goto unexpected;
				}
				break;
			case 5:
				if (memcmp(feature_start, "thumb", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_THUMB;
				} else if (memcmp(feature_start, "26bit", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_26BIT;
				} else if (memcmp(feature_start, "vfpv3", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_VFPV3;
				} else if (memcmp(feature_start, "vfpv4", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_VFPV4;
				} else if (memcmp(feature_start, "idiva", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_IDIVA;
				} else if (memcmp(feature_start, "idivt", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_IDIVT;
				} else if (memcmp(feature_start, "pmull", feature_length) == 0) {
					proc_cpuinfo->features2 |= PROC_CPUINFO_FEATURE2_PMULL;
				} else if (memcmp(feature_start, "crc32", feature_length) == 0) {
					proc_cpuinfo->features2 |= PROC_CPUINFO_FEATURE2_CRC32;
				} else {
					goto unexpected;
				}
 				break;
			case 6:
				if (memcmp(feature_start, "iwmmxt", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_IWMMXT;
				} else if (memcmp(feature_start, "crunch", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_CRUNCH;
				} else if (memcmp(feature_start, "vfpd32", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_VFPD32;
				} else {
					goto unexpected;
				}
				break;
			case 7:
				if (memcmp(feature_start, "thumbee", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_THUMBEE;
				} else if (memcmp(feature_start, "evtstrm", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_EVTSTRM;
				} else {
					goto unexpected;
				}
				break;
			case 8:
				if (memcmp(feature_start, "fastmult", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_FASTMULT;
				} else if (memcmp(feature_start, "vfpv3d16", feature_length) == 0) {
					proc_cpuinfo->features |= PROC_CPUINFO_FEATURE_VFPV3D16;
				} else {
					goto unexpected;
				}
				break;
			default:
			unexpected:
				cpuinfo_log_warning("unexpected /proc/cpuinfo features %.*s is ignored",
					(int) feature_length, feature_start);
				break;
		}
		feature_start = feature_end;
		for (; feature_start != features_end; feature_start++) {
			if (*feature_start != ' ') {
				break;
			}
		}
	} while (feature_start != feature_end);
}

static void parse_cpu_architecture(
	const char* cpu_architecture_start,
	const char* cpu_architecture_end,
	struct proc_cpuinfo proc_cpuinfo[restrict static 1])
{
	const size_t cpu_architecture_length = (size_t) (cpu_architecture_end - cpu_architecture_start);

	uint32_t architecture = 0;
	const char* cpu_architecture_ptr = cpu_architecture_start;
	for (; cpu_architecture_ptr != cpu_architecture_end; cpu_architecture_ptr++) {
		const uint32_t digit = (*cpu_architecture_ptr) - '0';

		/* Verify that CPU architecture is a decimal number */
		if (digit >= 10) {
			break;
		}

		architecture = architecture * 10 + digit;
	}

	if (architecture != 0) {
		proc_cpuinfo->architecture.version = architecture;
		proc_cpuinfo->valid_mask |= PROC_CPUINFO_VALID_ARCHITECTURE;

		for (; cpu_architecture_ptr != cpu_architecture_end; cpu_architecture_ptr++) {
			const char feature = *cpu_architecture_ptr;
			switch (feature) {
				case 'T':
					proc_cpuinfo->architecture.flags |= PROC_CPUINFO_ARCH_T;
					break;
				case 'E':
					proc_cpuinfo->architecture.flags |= PROC_CPUINFO_ARCH_E;
					break;
				case 'J':
					proc_cpuinfo->architecture.flags |= PROC_CPUINFO_ARCH_J;
					break;
				case ' ':
				case '\t':
					/* Ignore whitespace at the end */
					break;
				default:
					cpuinfo_log_warning("skipped unknown architectural feature '%c' for ARMv%"PRIu32,
						feature, architecture);
					break;
			}
		}
	}
}

static void parse_cpu_part(
	const char* cpu_part_start,
	const char* cpu_part_end,
	struct proc_cpuinfo proc_cpuinfo[restrict static 1])
{
	const size_t cpu_part_length = (size_t) (cpu_part_end - cpu_part_start);

	/*
	 * CPU part should contain hex prefix (0x) and one to three hex digits.
	 * I have never seen less than three digits as a value of this field,
	 * but I don't think it is impossible to see such values in future.
	 * Value can not contain more than three hex digits since
	 * Main ID Register (MIDR) assigns only a 12-bit value for CPU part.
	 */
	if (cpu_part_length < 3 || cpu_part_length > 5) {
		cpuinfo_log_warning("CPU part %.*s in /proc/cpuinfo is ignored due to unexpected length (%zu)",
			(int) cpu_part_length, cpu_part_start, cpu_part_length);
		return;
	}

	/* Verify the presence of hex prefix */
	if (cpu_part_start[0] != '0' || cpu_part_start[1] != 'x') {
		cpuinfo_log_warning("CPU part %.*s in /proc/cpuinfo is ignored due to lack of 0x prefix",
			(int) cpu_part_length, cpu_part_start);
		return;
	}

	/* Verify that characters after hex prefix are hexadecimal digits and decode them */
	uint32_t cpu_part = 0;
	for (const char* digit_ptr = cpu_part_start + 2; digit_ptr != cpu_part_end; digit_ptr++) {
		const char digit_char = *digit_ptr;
		uint32_t digit;
		if (digit_char >= '0' && digit_char <= '9') {
			digit = digit_char - '0';
		} else if ((uint32_t) (digit_char - 'A') < 6) {
			digit = 10 + (digit_char - 'A');
		} else if ((uint32_t) (digit_char - 'a') < 6) {
			digit = 10 + (digit_char - 'a');
		} else {
			cpuinfo_log_warning("CPU part %.*s in /proc/cpuinfo is ignored due to unexpected non-hex character %c at offset %zu",
				(int) cpu_part_length, cpu_part_start, digit_char, (size_t) (digit_ptr - cpu_part_start));
			return;
		}
		cpu_part = cpu_part * 16 + digit;
	}

	proc_cpuinfo->part = cpu_part;
	proc_cpuinfo->valid_mask |= PROC_CPUINFO_VALID_PART;
}

static void parse_cpu_implementer(
	const char* cpu_implementer_start,
	const char* cpu_implementer_end,
	struct proc_cpuinfo proc_cpuinfo[restrict static 1])
{
	const size_t cpu_implementer_length = cpu_implementer_end - cpu_implementer_start;

	/*
	 * Value should contain hex prefix (0x) and one or two hex digits.
	 * I have never seen single hex digit as a value of this field,
	 * but I don't think it is impossible in future.
	 * Value can not contain more than two hex digits since
	 * Main ID Register (MIDR) assigns only an 8-bit value for CPU implementer.
	 */
	switch (cpu_implementer_length) {
		case 3:
		case 4:
			break;
		default:
		cpuinfo_log_warning("CPU implementer %.*s in /proc/cpuinfo is ignored due to unexpected length (%zu)",
			(int) cpu_implementer_length, cpu_implementer_start, cpu_implementer_length);
		return;
	}

	/* Verify the presence of hex prefix */
	if (cpu_implementer_start[0] != '0' || cpu_implementer_start[1] != 'x') {
		cpuinfo_log_warning("CPU implementer %.*s in /proc/cpuinfo is ignored due to lack of 0x prefix",
			(int) cpu_implementer_length, cpu_implementer_start);
		return;
	}

	/* Verify that characters after hex prefix are hexadecimal digits and decode them */
	uint32_t cpu_implementer = 0;
	for (const char* digit_ptr = cpu_implementer_start + 2; digit_ptr != cpu_implementer_end; digit_ptr++) {
		const char digit_char = *digit_ptr;
		uint32_t digit;
		if (digit_char >= '0' && digit_char <= '9') {
			digit = digit_char - '0';
		} else if ((uint32_t) (digit_char - 'A') < 6) {
			digit = 10 + (digit_char - 'A');
		} else if ((uint32_t) (digit_char - 'a') < 6) {
			digit = 10 + (digit_char - 'a');
		} else {
			cpuinfo_log_warning("CPU implementer %.*s in /proc/cpuinfo is ignored due to unexpected non-hex character '%c' at offset %zu",
				(int) cpu_implementer_length, cpu_implementer_start, digit_char, (size_t) (digit_ptr - cpu_implementer_start));
			return;
		}
		cpu_implementer = cpu_implementer * 16 + digit;
	}

	proc_cpuinfo->implementer = cpu_implementer;
	proc_cpuinfo->valid_mask |= PROC_CPUINFO_VALID_IMPLEMENTER;
}

static void parse_cpu_variant(
	const char* cpu_variant_start,
	const char* cpu_variant_end,
	struct proc_cpuinfo proc_cpuinfo[restrict static 1])
{
	const size_t cpu_variant_length = cpu_variant_end - cpu_variant_start;

	/*
	 * Value should contain hex prefix (0x) and one hex digit.
	 * Value can not contain more than one hex digits since
	 * Main ID Register (MIDR) assigns only a 4-bit value for CPU variant.
	 */
	if (cpu_variant_length != 3) {
		cpuinfo_log_warning("CPU variant %.*s in /proc/cpuinfo is ignored due to unexpected length (%zu)",
			(int) cpu_variant_length, cpu_variant_start, cpu_variant_length);
		return;
	}

	/* Skip if there is no hex prefix (0x) */
	if (cpu_variant_start[0] != '0' || cpu_variant_start[1] != 'x') {
		cpuinfo_log_warning("CPU variant %.*s in /proc/cpuinfo is ignored due to lack of 0x prefix",
			(int) cpu_variant_length, cpu_variant_start);
		return;
	}

	/* Check if the value after hex prefix is indeed a hex digit and decode it. */
	const char digit_char = cpu_variant_start[2];
	if ((uint32_t) (digit_char - '0') < 10) {
		proc_cpuinfo->variant = (uint32_t) (digit_char - '0');
	} else if ((uint32_t) (digit_char - 'A') < 6) {
		proc_cpuinfo->variant = 10 + (uint32_t) (digit_char - 'A');
	} else if ((uint32_t) (digit_char - 'a') < 6) {
		proc_cpuinfo->variant = 10 + (uint32_t) (digit_char - 'a');
	} else {
		cpuinfo_log_warning("CPU variant %.*s in /proc/cpuinfo is ignored due to unexpected non-hex character '%c'",
			(int) cpu_variant_length, cpu_variant_start, digit_char);
		return;
	}

	proc_cpuinfo->valid_mask |= PROC_CPUINFO_VALID_VARIANT;
}

static void parse_cpu_revision(
	const char* cpu_revision_start,
	const char* cpu_revision_end,
	struct proc_cpuinfo proc_cpuinfo[restrict static 1])
{
	uint32_t cpu_revision = 0;
	for (const char* digit_ptr = cpu_revision_start; digit_ptr != cpu_revision_end; digit_ptr++) {
		const uint32_t digit = (uint32_t) (*digit_ptr - '0');

		/* Verify that the character in CPU revision is a decimal digit */
		if (digit >= 10) {
			cpuinfo_log_warning("CPU revision %.*s in /proc/cpuinfo is ignored due to unexpected non-digit character '%c' at offset %zu",
				(int) (cpu_revision_end - cpu_revision_start), cpu_revision_start,
				*digit_ptr, (size_t) (digit_ptr - cpu_revision_start));
			return;
		}

		cpu_revision = cpu_revision * 10 + digit;
	}

	proc_cpuinfo->revision = cpu_revision;
	proc_cpuinfo->valid_mask |= PROC_CPUINFO_VALID_REVISION;
}

/*
 * Decode one of the cache-related numbers reported by Linux kernel
 * for pre-ARMv7 architecture.
 * An example cache-related information in /proc/cpuinfo:
 *
 *      I size          : 32768
 *      I assoc         : 4
 *      I line length   : 32
 *      I sets          : 256
 *      D size          : 16384
 *      D assoc         : 4
 *      D line length   : 32
 *      D sets          : 128
 *
 */
static void parse_cache_number(
	const char* number_start,
	const char* number_end,
	const char* number_name,
	uint32_t number_ptr[restrict static 1],
	uint32_t valid_mask[restrict static 1],
	uint32_t number_mask)
{
	uint32_t number = 0;
	for (const char* digit_ptr = number_start; digit_ptr != number_end; digit_ptr++) {
		const uint32_t digit = *digit_ptr - '0';
		if (digit >= 10) {
			cpuinfo_log_warning("%s %.*s in /proc/cpuinfo is ignored due to unexpected non-digit character '%c' at offset %zu",
				number_name, (int) (number_end - number_start), number_start,
				*digit_ptr, (size_t) (digit_ptr - number_start));
			return;
		}

		number = number * 10 + digit;
	}

	if (number == 0) {
		cpuinfo_log_warning("%s %.*s in /proc/cpuinfo is ignored due to invalid value of zero reported by the kernel",
			number_name, (int) (number_end - number_start), number_start);
	}

	/* If the number specifies a cache line size, verify that is a reasonable power of 2 */
	if (number_mask & PROC_CPUINFO_VALID_CACHE_LINE) {
		switch (number) {
			case 16:
			case 32:
			case 64:
			case 128:
				break;
			default:
				cpuinfo_log_warning("invalid %s %.*s is ignored: a value of 16, 32, 64, or 128 expected",
					number_name, (int) (number_end - number_start), number_start);			
		}
	}

	*number_ptr = number;
	*valid_mask |= number_mask;
}

/*
 *	Decode a single line of /proc/cpuinfo information.
 *	Lines have format <words-with-spaces>[ ]*:[ ]<space-separated words>
 *	An example of /proc/cpuinfo (from Pandaboard-ES):
 *
 *		Processor       : ARMv7 Processor rev 10 (v7l)
 *		processor       : 0
 *		BogoMIPS        : 1392.74
 *
 *		processor       : 1
 *		BogoMIPS        : 1363.33
 *
 *		Features        : swp half thumb fastmult vfp edsp thumbee neon vfpv3
 *		CPU implementer : 0x41
 *		CPU architecture: 7
 *		CPU variant     : 0x2
 *		CPU part        : 0xc09
 *		CPU revision    : 10
 *
 *		Hardware        : OMAP4 Panda board
 *		Revision        : 0020
 *		Serial          : 0000000000000000
 */
static uint32_t parse_line(
	const char* line_start,
	const char* line_end,
	uint32_t processor_number,
	struct proc_cpuinfo* proc_cpuinfo)
{
	/* Empty line. Skip. */
	if (line_start == line_end) {
		return processor_number;
	}
	
	/* Search for ':' on the line. */
	const char* separator = line_start;
	for (; separator != line_end; separator++) {
		if (*separator == ':') {
			break;
		}
	}
	/* Skip line if no ':' separator was found. */
	if (separator == line_end) {
		cpuinfo_log_warning("Line %.*s in /proc/cpuinfo is ignored: key/value separator ':' not found",
			(int) (line_end - line_start), line_start);
		return processor_number;
	}

	/* Skip trailing spaces in key part. */
	const char* key_end = separator;
	for (; key_end != line_start; key_end--) {
		if (key_end[-1] != ' ' && key_end[-1] != '\t') {
			break;
		}
	}
	/* Skip line if key contains nothing but spaces. */
	if (key_end == line_start) {
		cpuinfo_log_warning("Line %.*s in /proc/cpuinfo is ignored: key contains only spaces",
			(int) (line_end - line_start), line_start);
		return processor_number;
	}

	/* Skip leading spaces in value part. */
	const char* value_start = separator + 1;
	for (; value_start != line_end; value_start++) {
		if (*value_start != ' ') {
			break;
		}
	}
	/* Value part contains nothing but spaces. Skip line. */
	if (value_start == line_end) {
		cpuinfo_log_warning("Line %.*s in /proc/cpuinfo is ignored: value contains only spaces",
			(int) (line_end - line_start), line_start);
		return processor_number;
	}

	/* Skip trailing spaces in value part (if any) */
	const char* value_end = line_end;
	for (; value_end != separator; value_end--) {
		if (value_end[-1] != ' ') {
			break;
		}
	}

	const size_t key_length = key_end - line_start;
	switch (key_length) {
		case 6:
			if (memcmp(line_start, "I size", key_length) == 0) {
				parse_cache_number(value_start, value_end,
					"instruction cache size", &proc_cpuinfo->cache.i_size,
					&proc_cpuinfo->valid_mask, PROC_CPUINFO_VALID_ICACHE_SIZE);
			} else if (memcmp(line_start, "I sets", key_length) == 0) {
				parse_cache_number(value_start, value_end,
					"instruction cache sets", &proc_cpuinfo->cache.i_sets,
					&proc_cpuinfo->valid_mask, PROC_CPUINFO_VALID_ICACHE_SETS);
			} else if (memcmp(line_start, "D size", key_length) == 0) {
				parse_cache_number(value_start, value_end,
					"data cache size", &proc_cpuinfo->cache.d_size,
					&proc_cpuinfo->valid_mask, PROC_CPUINFO_VALID_DCACHE_SIZE);
			} else if (memcmp(line_start, "D sets", key_length) == 0) {
				parse_cache_number(value_start, value_end,
					"data cache sets", &proc_cpuinfo->cache.d_sets,
					&proc_cpuinfo->valid_mask, PROC_CPUINFO_VALID_DCACHE_SETS);
			} else if (memcmp(line_start, "Serial", key_length) == 0) {
				/* Usually contains just zeros, useless */
			} else {
				goto unknown;
			}
			break;
		case 7:
			if (memcmp(line_start, "I assoc", key_length) == 0) {
				parse_cache_number(value_start, value_end,
					"instruction cache associativity", &proc_cpuinfo->cache.i_assoc,
					&proc_cpuinfo->valid_mask, PROC_CPUINFO_VALID_ICACHE_WAYS);
			} else if (memcmp(line_start, "D assoc", key_length) == 0) {
				parse_cache_number(value_start, value_end,
					"data cache associativity", &proc_cpuinfo->cache.d_assoc,
					&proc_cpuinfo->valid_mask, PROC_CPUINFO_VALID_DCACHE_WAYS);
			} else {
				goto unknown;
			}
			break;
		case 8:
			if (memcmp(line_start, "CPU part", key_length) == 0) {
				parse_cpu_part(value_start, value_end, proc_cpuinfo);
			} else if (memcmp(line_start, "Features", key_length) == 0) {
				parse_features(value_start, value_end, proc_cpuinfo);
			} else if (memcmp(line_start, "BogoMIPS", key_length) == 0) {
				/* BogoMIPS is useless, don't parse */
			} else if (memcmp(line_start, "Hardware", key_length) == 0) {
				/* TODO: parse to extract SoC name */
			} else if (memcmp(line_start, "Revision", key_length) == 0) {
				/* Board revision, no use for now */
			} else {
				goto unknown;
			}
			break;
		case 9:
			if (memcmp(line_start, "processor", key_length) == 0) {
				const uint32_t new_processor_number =
					parse_processor_number(value_start, value_end, proc_cpuinfo);
				const uint32_t new_processors_count = new_processor_number + 1;
				if (new_processors_count <= processor_number && processor_number != 0) {
					cpuinfo_log_warning("ignored unexpectedly low processor number %"PRIu32" following processor %"PRIu32" in /proc/cpuinfo",
						new_processor_number, processor_number);
				} else {
					if (new_processors_count > processor_number + 1) {
						cpuinfo_log_info("unexpectedly high processor number %"PRIu32" following processor %"PRIu32" in /proc/cpuinfo",
							new_processor_number, processor_number);
						return new_processors_count;
					}
					return new_processors_count;
				}
			} else if (memcmp(line_start, "Processor", key_length) == 0) {
				/* TODO: parse to fix misreported architecture, similar to Android's cpufeatures */
			} else {
				goto unknown;
			}
			break;
		case 11:
			if (memcmp(line_start, "CPU variant", key_length) == 0) {
				parse_cpu_variant(value_start, value_end, proc_cpuinfo);
			} else {
				goto unknown;
			}
			break;
		case 12:
			if (memcmp(line_start, "CPU revision", key_length) == 0) {
				parse_cpu_revision(value_start, value_end, proc_cpuinfo);
			} else {
				goto unknown;
			}
			break;
		case 13:
			if (memcmp(line_start, "I line length", key_length) == 0) {
				parse_cache_number(value_start, value_end,
					"instruction cache line size", &proc_cpuinfo->cache.i_line_length,
					&proc_cpuinfo->valid_mask, PROC_CPUINFO_VALID_ICACHE_LINE);
			} else if (memcmp(line_start, "D line length", key_length) == 0) {
				parse_cache_number(value_start, value_end,
					"data cache line size", &proc_cpuinfo->cache.d_line_length,
					&proc_cpuinfo->valid_mask, PROC_CPUINFO_VALID_DCACHE_LINE);
			} else {
				goto unknown;
			}
			break;
		case 15:
			if (memcmp(line_start, "CPU implementer", key_length) == 0) {
				parse_cpu_implementer(value_start, value_end, proc_cpuinfo);
			} else if (memcmp(line_start, "CPU implementor", key_length) == 0) {
				parse_cpu_implementer(value_start, value_end, proc_cpuinfo);
			} else {
				goto unknown;
			}
			break;
		case 16:
			if (memcmp(line_start, "CPU architecture", key_length) == 0) {
				parse_cpu_architecture(value_start, value_end, proc_cpuinfo);
			} else {
				goto unknown;
			}
			break;
		default:
		unknown:
			cpuinfo_log_debug("unknown /proc/cpuinfo key: %.*s", (int) key_length, line_start);

	}
	return processor_number;
}

struct proc_cpuinfo* cpuinfo_arm_linux_parse_proc_cpuinfo(uint32_t processors_count_ptr[restrict static 1]) {
	int file = -1;
	struct proc_cpuinfo* processors = NULL;
	struct proc_cpuinfo* result = NULL;
	uint32_t processors_capacity = 8;
	uint32_t processors_count = 1;
	char buffer[BUFFER_SIZE];

	processors = calloc(processors_capacity, sizeof(struct proc_cpuinfo));
	if (processors == NULL) {
		cpuinfo_log_error("failed to allocate %zu bytes for /proc/cpuinfo data",
			processors_capacity * sizeof(struct proc_cpuinfo));
		goto cleanup;
	}

	cpuinfo_log_debug("parsing cpu info from file %s", proc_cpuinfo_path);
	file = open(proc_cpuinfo_path, O_RDONLY);
	if (file == -1) {
		cpuinfo_log_error("failed to open %s: %s", proc_cpuinfo_path, strerror(errno));
		goto cleanup;
	}

	size_t position = 0;
	const char* buffer_end = &buffer[BUFFER_SIZE];
	char* data_start = buffer;
	ssize_t bytes_read;
	do {
		bytes_read = read(file, data_start, (size_t) (buffer_end - data_start));
		if (bytes_read < 0) {
			cpuinfo_log_error("failed to read file %s at position %zu: %s", proc_cpuinfo_path, position, strerror(errno));
			goto cleanup;
		}

		position += (size_t) bytes_read;
		const char* data_end = data_start + (size_t) bytes_read;
		const char* line_start = buffer;

		if (bytes_read == 0) {
			/* No more data in the file: process the remaining text in the buffer as a single entry */
			const char* line_end = data_end;
			const uint32_t new_processors_count =
				parse_line(line_start, line_end, processors_count, &processors[processors_count - 1]);
			if (new_processors_count > processors_capacity) {
				processors = realloc(processors, new_processors_count * sizeof(struct proc_cpuinfo));
			}
			memset(&processors[processors_count], 0, (new_processors_count - processors_count) * sizeof(struct proc_cpuinfo));
			processors_count = processors_capacity = new_processors_count;
		} else {
			const char* line_end;
			do {
				/* Find the end of the entry, as indicated by a comma (',') */
				for (line_end = line_start; line_end != data_end; line_end++) {
					if (*line_end == '\n') {
						break;
					}
				}

				/*
				 * If we located separator at the end of the entry, parse it.
				 * Otherwise, there may be more data at the end; read the file once again.
				 */
				if (line_end != data_end) {
					const uint32_t new_processors_count =
						parse_line(line_start, line_end, processors_count, &processors[processors_count - 1]);
					if (new_processors_count > processors_capacity) {
						processors = realloc(processors, new_processors_count * sizeof(struct proc_cpuinfo));
					}
					memset(&processors[processors_count], 0, (new_processors_count - processors_count) * sizeof(struct proc_cpuinfo));
					processors_count = processors_capacity = new_processors_count;
					line_start = line_end + 1;
				}
			} while (line_end != data_end);

			/* Move remaining partial line data at the end to the beginning of the buffer */
			const size_t line_length = (size_t) (line_end - line_start);
			memmove(buffer, line_start, line_length);
			data_start = &buffer[line_length];
		}

		if (processors == NULL) {
			cpuinfo_log_error("failed to allocate %zu bytes for /proc/cpuinfo data",
				processors_capacity * sizeof(struct proc_cpuinfo));
			goto cleanup;
		}
	} while (bytes_read != 0);


	uint32_t last_i = 0;
	for (uint32_t i = processors_count; i != 0; i--) {
		if ((processors[i - 1].valid_mask & PROC_CPUINFO_VALID_INFO) == PROC_CPUINFO_VALID_INFO) {
			last_i = i;
			break;
		}
	}

	if (last_i == 0) {
		cpuinfo_log_error("none of the %"PRIu32" processors reported in /proc/cpuinfo were successfully parsed",
			processors_count);
		goto cleanup;
	}

	for (uint32_t i = last_i - 1; i < processors_count; i++) {
		processors[i] = processors[last_i - 1];
	}
	for (uint32_t i = last_i; i != 0; i--) {
		if ((processors[i - 1].valid_mask & PROC_CPUINFO_VALID_INFO) == PROC_CPUINFO_VALID_INFO) {
			last_i = i;
		} else {
			processors[i - 1] = processors[last_i - 1];
		}
	}

	/* Commit */
	result = processors;
	processors = NULL;
	*processors_count_ptr = processors_count;

cleanup:
	free(processors);
	processors = NULL;
	if (file != -1) {
		close(file);
		file = -1;
	}
	return result;
}
