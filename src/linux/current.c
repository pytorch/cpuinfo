#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sched.h>

#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>
#include <linux/api.h>


const struct cpuinfo_processor** cpuinfo_linux_cpu_to_processor_map = NULL;
const struct cpuinfo_core** cpuinfo_linux_cpu_to_core_map = NULL;


const struct cpuinfo_processor* CPUINFO_ABI cpuinfo_get_current_processor(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "current_processor");
	}
	const int cpu = sched_getcpu();
	if (cpu >= 0) {
		return cpuinfo_linux_cpu_to_processor_map[cpu];
	} else {
		return &cpuinfo_processors[0];
	}
}

const struct cpuinfo_core* CPUINFO_ABI cpuinfo_get_current_core(void) {
	if (!cpuinfo_is_initialized) {
		cpuinfo_log_fatal("cpuinfo_get_%s called before cpuinfo is initialized", "current_core");
	}
	const int cpu = sched_getcpu();
	if (cpu >= 0) {
		return cpuinfo_linux_cpu_to_core_map[cpu];
	} else {
		return &cpuinfo_cores[0];
	}
}
