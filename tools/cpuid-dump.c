#ifdef __linux__
#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#endif

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <x86/cpuid.h>

static void print_cpuid(struct cpuid_regs regs, uint32_t eax) {
	printf("CPUID %08" PRIX32 ": %08" PRIX32 "-%08" PRIX32 "-%08" PRIX32 "-%08" PRIX32 "\n",
	       eax,
	       regs.eax,
	       regs.ebx,
	       regs.ecx,
	       regs.edx);
}

static void print_cpuidex(struct cpuid_regs regs, uint32_t eax, uint32_t ecx) {
	printf("CPUID %08" PRIX32 ": %08" PRIX32 "-%08" PRIX32 "-%08" PRIX32 "-%08" PRIX32 " [SL %02" PRIX32 "]\n",
	       eax,
	       regs.eax,
	       regs.ebx,
	       regs.ecx,
	       regs.edx,
	       ecx);
}

enum cpuid_string_format { REG_ABCD = 1, REG_BDC = 2, REG_BCD = 3, REG_A = 4 };

static void print_cpuid_string(struct cpuid_regs regs, uint32_t eax, enum cpuid_string_format fmt) {
	char buf[16];
	memset(buf, 0, sizeof(buf));
	switch (fmt) {
		case REG_ABCD:
			memcpy(&buf[0], &regs.eax, sizeof(regs.eax));
			memcpy(&buf[4], &regs.ebx, sizeof(regs.ebx));
			memcpy(&buf[8], &regs.ecx, sizeof(regs.ecx));
			memcpy(&buf[12], &regs.edx, sizeof(regs.edx));
			break;
		case REG_BCD:
			memcpy(&buf[0], &regs.ebx, sizeof(regs.ebx));
			memcpy(&buf[4], &regs.ecx, sizeof(regs.ecx));
			memcpy(&buf[8], &regs.edx, sizeof(regs.edx));
			break;
		case REG_BDC:
			memcpy(&buf[0], &regs.ebx, sizeof(regs.ebx));
			memcpy(&buf[4], &regs.edx, sizeof(regs.ecx));
			memcpy(&buf[8], &regs.ecx, sizeof(regs.edx));
			break;
		case REG_A:
			memcpy(&buf[0], &regs.eax, sizeof(regs.eax));
			break;
	}
	if (buf[0]) {
		printf("CPUID %08" PRIX32 ": %08" PRIX32 "-%08" PRIX32 "-%08" PRIX32 "-%08" PRIX32 " [%.16s]\n",
		       eax,
		       regs.eax,
		       regs.ebx,
		       regs.ecx,
		       regs.edx,
		       buf);
	} else {
		print_cpuid(regs, eax);
	}
}

static void force_one_cpu(int cpu, int n_cpus) {
	if (n_cpus > 1) {
#ifdef __linux__
		cpu_set_t mask_one;
		CPU_ZERO(&mask_one);
		CPU_SET(cpu, &mask_one);
		if (sched_setaffinity(0, sizeof mask_one, &mask_one) != 0) {
			char buf[80];
			snprintf(buf, sizeof(buf), "sched_setaffinity to CPU %d failed", cpu);
			perror(buf);
			exit(1);
		}
#endif
	}
}

static void print_cpu_index(int cpu, int n_cpus) {
#ifdef __linux__
	if (n_cpus > 1)
		printf("cpu%.*d: ", (n_cpus < 10 ? 1 : (n_cpus < 100 ? 2 : 3)), cpu);
#endif
}

int main(int argc, char** argv) {
	const uint32_t max_base_index = cpuid(0).eax;
	uint32_t max_structured_index = 0, max_trace_index = 0, max_socid_index = 0, n_log_proc = 1;
	bool has_sgx = false, is_hybrid = false;

#ifdef __linux__
	// TODO: handle case of >CPU_SETSIZE logical CPUs, or non-contiguity
	cpu_set_t mask_default;
	if (sched_getaffinity(0, sizeof mask_default, &mask_default) != 0) {
		perror("sched_getaffinity failed");
		exit(1);
	}
	n_log_proc = CPU_COUNT(&mask_default);
#endif
	if (max_base_index >= 1) {
		// TODO: handle case of >256 logical CPUs, or multiple packages
		const struct cpuid_regs regs = cpuid(1);
		uint32_t n_apic_id;
		if (regs.edx & UINT32_C(0x10000000)) { // Number of logical CPUs field is valid
			n_apic_id = ((regs.ebx & UINT32_C(0x00ff0000)) >> 16);

#ifdef __linux__
			if (n_apic_id != n_log_proc)
				fprintf(stderr,
					"WARNING: %d logical CPUs per CPUID.01h.EBX[23:16] != %d per sched_getaffinity()\n",
					n_apic_id,
					n_log_proc);
#else
			if (n_apic_id > 1)
				fprintf(stderr,
					"WARNING: %d logical CPUs per CPUID.01h.EBX, results may vary by logical CPU\n",
					n_apic_id);
#endif
		}
	}

	for (uint32_t lp = 0; lp < n_log_proc; lp++) {
		force_one_cpu(lp, n_log_proc);

		for (uint32_t eax = 0; eax <= max_base_index; eax++) {
			switch (eax) {
				case UINT32_C(0x00000000):
					print_cpu_index(lp, n_log_proc);
					print_cpuid_string(cpuid(eax), eax, REG_BDC);
					break;
				case UINT32_C(0x00000004):
					for (uint32_t ecx = 0;; ecx++) {
						const struct cpuid_regs regs = cpuidex(eax, ecx);
						if ((regs.eax & UINT32_C(0x1F)) == 0) {
							break;
						}
						print_cpu_index(lp, n_log_proc);
						print_cpuidex(regs, eax, ecx);
					}
					break;
				case UINT32_C(0x00000007):
					for (uint32_t ecx = 0; ecx <= max_structured_index; ecx++) {
						const struct cpuid_regs regs = cpuidex(eax, ecx);
						if (ecx == 0) {
							max_structured_index = regs.eax;
							has_sgx = !!(regs.ebx & UINT32_C(0x00000004));
						}
						print_cpu_index(lp, n_log_proc);
						print_cpuidex(regs, eax, ecx);
					}
					break;
				case UINT32_C(0x0000000B):
				case UINT32_C(0x0000001F): // Extended/V2
					for (uint32_t ecx = 0;; ecx++) {
						const struct cpuid_regs regs = cpuidex(eax, ecx);
						if ((regs.ecx & UINT32_C(0x0000FF00)) == 0) {
							break;
						}
						print_cpu_index(lp, n_log_proc);
						print_cpuidex(regs, eax, ecx);
					}
					break;
				case UINT32_C(0x00000012):
					if (has_sgx) {
						for (uint32_t ecx = 0;; ecx++) {
							const struct cpuid_regs regs = cpuidex(eax, ecx);
							if (ecx >= 2 && (regs.eax & UINT32_C(0x0000000F)) == 0) {
								break;
							}
							print_cpu_index(lp, n_log_proc);
							print_cpuidex(regs, eax, ecx);
						}
					}
					break;
				case UINT32_C(0x00000014):
					for (uint32_t ecx = 0; ecx <= max_trace_index; ecx++) {
						const struct cpuid_regs regs = cpuidex(eax, ecx);
						if (ecx == 0) {
							max_trace_index = regs.eax;
						}
						print_cpu_index(lp, n_log_proc);
						print_cpuidex(regs, eax, ecx);
					}
					break;
				case UINT32_C(0x00000017):
					for (uint32_t ecx = 0; ecx <= max_socid_index; ecx++) {
						const struct cpuid_regs regs = cpuidex(eax, ecx);
						if (ecx == 0) {
							max_socid_index = regs.eax;
						}
						print_cpu_index(lp, n_log_proc);
						print_cpuidex(regs, eax, ecx);
					}
					break;
				case UINT32_C(0x00000024):
					for (uint32_t ecx = 0; ecx <= max_socid_index; ecx++) {
						const struct cpuid_regs regs = cpuidex(eax, ecx);
						if (ecx == 0) {
							max_socid_index = regs.eax;
						}
						print_cpu_index(lp, n_log_proc);
						print_cpuidex(regs, eax, ecx);
					}
					break;
				default:
					print_cpu_index(lp, n_log_proc);
					print_cpuid(cpuidex(eax, 0), eax);
					break;
			}
		}

		/**
		 * CPUID[1].ECX bit 31 is supposed to indicate whether or not
		 * a hypervisor is running, but not all hypervisors set it.
		 */
		const uint32_t max_hypervisor_index = cpuid(UINT32_C(0x40000000)).eax;
		for (uint32_t eax = UINT32_C(0x40000000); eax <= max_hypervisor_index; eax++) {
			switch (eax) {
				case UINT32_C(0x40000000):
					print_cpu_index(lp, n_log_proc);
					print_cpuid_string(cpuid(eax), eax, REG_BCD);
					break;
				case UINT32_C(0x40000001):
					print_cpu_index(lp, n_log_proc);
					print_cpuid_string(cpuid(eax), eax, REG_A);
					break;
				default:
					print_cpu_index(lp, n_log_proc);
					print_cpuid(cpuidex(eax, 0), eax);
			}
		}

		const uint32_t max_extended_index = cpuid(UINT32_C(0x80000000)).eax;
		for (uint32_t eax = UINT32_C(0x80000000); eax <= max_extended_index; eax++) {
			switch (eax) {
				case UINT32_C(0x80000000):
					print_cpu_index(lp, n_log_proc);
					print_cpuid_string(cpuid(eax), eax, REG_BDC);
					break;
				case UINT32_C(0x80000002):
				case UINT32_C(0x80000003):
				case UINT32_C(0x80000004):
					print_cpu_index(lp, n_log_proc);
					print_cpuid_string(cpuid(eax), eax, REG_ABCD);
					break;
				default:
					print_cpu_index(lp, n_log_proc);
					print_cpuid(cpuidex(eax, 0), eax);
			}
		}
	}

	if (n_log_proc > 1) {
#ifdef __linux__
		if (sched_setaffinity(0, sizeof mask_default, &mask_default) != 0) {
			perror("sched_setaffinity to restore process defaults failed");
			exit(1);
		}
#endif
	}
}
