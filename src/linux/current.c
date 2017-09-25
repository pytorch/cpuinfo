#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sched.h>

#include <cpuinfo.h>
#include <linux/api.h>


const struct cpuinfo_processor** cpuinfo_linux_cpu_to_processor_map;
const struct cpuinfo_core** cpuinfo_linux_cpu_to_core_map;


const struct cpuinfo_processor* CPUINFO_ABI cpuinfo_current_processor(void) {
	const int cpu = sched_getcpu();
	if (cpu >= 0) {
		return cpuinfo_linux_cpu_to_processor_map[cpu];
	} else {
		return &cpuinfo_processors[0];
	}
}

const struct cpuinfo_core* CPUINFO_ABI cpuinfo_current_core(void) {
	const int cpu = sched_getcpu();
	if (cpu >= 0) {
		return cpuinfo_linux_cpu_to_core_map[cpu];
	} else {
		return &cpuinfo_cores[0];
	}
}
