#include <stdint.h>

#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>
#include <loongarch/api.h>
#include <loongarch/cpucfg.h>


void cpuinfo_loongarch_decode_cache(
	enum cpuinfo_uarch uarch,
	uint32_t cluster_cores,
	uint32_t arch_version,
	struct cpuinfo_cache l1i[restrict static 1],
	struct cpuinfo_cache l1d[restrict static 1],
	struct cpuinfo_cache l2[restrict static 1],
	struct cpuinfo_cache l3[restrict static 1])
{
	switch (uarch) {
	/*
		* Loongarch 3A5000 Core Technical Reference Manual
		* Loongarch 3A5000. About the L1 memory system
		*   The L1 memory system consists of separate instruction and data caches. Both have a fixed size of 64KB.
		*
		* Loongarch 3A5000 L1 instruction-side memory system
		*   The L1 instruction memory system has the following key features:
		*    - Virtually Indexed, Physically Tagged (VIPT), four-way set-associative instruction cache.
		*    - Fixed cache line length of 64 bytes.
		*
		* Loongarch 3A5000 L1 data-side memory system
		*   The L1 data memory system has the following features:
		*    - Physically Indexed, Physically Tagged (PIPT), 4-way set-associative L1 data cache.
		*    - Fixed cache line length of 64 bytes.
		*    - Pseudo-random cache replacement policy.
		*
		* Loongarch 3A5000 About the L2 memory system
		*   The L2 memory subsystem consist of:
		*    - An 16-way set associative L2 cache with a configurable size of 256KB.
		*      Cache lines have a fixed length of 64 bytes.
		*
		*  +--------------------+-------+-----------+-----------+-----------+----------+
		*  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | L3 cache |
		*  +--------------------+-------+-----------+-----------+-----------+----------+
		*  | Loongarch 3A5000   | 4(+4) |    64K    |    64K    |    256K   |    16    |
		*  +--------------------+-------+-----------+-----------+-----------+----------+
		*
		*/
		case cpuinfo_uarch_LA464:
			*l1i = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 256 * 1024,
				.associativity = 16,
				.line_size = 64
			};
			*l3 = (struct cpuinfo_cache) {
				.size = 16 * 1024 * 1024,
				.associativity = 16,
				.line_size = 64 
			};
		default:
			cpuinfo_log_warning("loongarch uarch not recognized; using generic cache parameters");
			/* Follow OpenBLAS */
			if (arch_version >= 8) {
				*l1i = (struct cpuinfo_cache) {
					.size = 32 * 1024,
					.associativity = 4,
					.line_size = 64
				};
				*l1d = (struct cpuinfo_cache) {
					.size = 32 * 1024,
					.associativity = 4,
					.line_size = 64
				};
				*l2 = (struct cpuinfo_cache) {
					.size = cluster_cores * 256 * 1024,
					.associativity = 8,
					.line_size = 64
				};
			} else {
				*l1i = (struct cpuinfo_cache) {
					.size = 16 * 1024,
					.associativity = 4,
					.line_size = 32
				};
				*l1d = (struct cpuinfo_cache) {
					.size = 16 * 1024,
					.associativity = 4,
					.line_size = 32
				};
				if (arch_version >= 7) {
					*l2 = (struct cpuinfo_cache) {
						.size = cluster_cores * 128 * 1024,
						.associativity = 8,
						.line_size = 32
					};
				}
			}
			break;
	}
	l1i->sets = l1i->size / (l1i->associativity * l1i->line_size);
	l1i->partitions = 1;
	l1d->sets = l1d->size / (l1d->associativity * l1d->line_size);
	l1d->partitions = 1;
	if (l2->size != 0) {
		l2->sets = l2->size / (l2->associativity * l2->line_size);
		l2->partitions = 1;
		if (l3->size != 0) {
			l3->sets = l3->size / (l3->associativity * l3->line_size);
			l3->partitions = 1;
		}
	}
}

uint32_t cpuinfo_loongarch_compute_max_cache_size(const struct cpuinfo_processor* processor) {
	/*
	 * There is no precise way to detect cache size on LOONGARCH64, and cache size reported by cpuinfo
	 * may underestimate the actual cache size. Thus, we use microarchitecture-specific maximum.
	 */
	switch (processor->core->uarch) {
		
		case cpuinfo_uarch_LA464:
			return 16 * 1024 * 1024;
		default:
			return 4 * 1024 * 1024;
	}
}
