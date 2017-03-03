#include <string.h>
#include <alloca.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/host_info.h>
#include <mach/mach_host.h>

#include <log.h>
#include <mach/api.h>


struct cpuinfo_mach_topology cpuinfo_mach_detect_topology(void) {
	host_basic_info_data_t basic_info;
	mach_msg_type_number_t count = HOST_BASIC_INFO_COUNT;
	kern_return_t host_info_result = host_info(mach_host_self(), HOST_BASIC_INFO, (host_info_t) &basic_info, &count);
	if (host_info_result != KERN_SUCCESS) {
		cpuinfo_log_error("host_info(HOST_BASIC_INFO) failed with error code %d", host_info_result);
		return (struct cpuinfo_mach_topology) { .packages = 1, .cores = 1, .threads = 1 };
	}
	integer_t cores = basic_info.physical_cpu_max;
	if (cores <= 0) {
		cpuinfo_log_error("host_info(HOST_BASIC_INFO) returned invalid physical_cpu_max value %d", (int) cores);
		cores = 1;
	}
	integer_t threads = basic_info.logical_cpu_max;
	if (threads < cores) {
		cpuinfo_log_error("host_info(HOST_BASIC_INFO) returned invalid logical_cpu_max value %d "
			"physical_cpu_max = %d", (int) threads, (int) cores);
		threads = cores;
	}

	int packages = 1;
	size_t sizeof_packages = sizeof(packages);
	if (sysctlbyname("hw.packages", &packages, &sizeof_packages, NULL, 0) != 0) {
		cpuinfo_log_error("sysctlbyname(\"hw.packages\") failed: %s", strerror(errno));
	} else if (packages <= 0) {
		cpuinfo_log_error("sysctlbyname(\"hw.packages\") returned invalid value %d", packages);
		packages = 1;
	}

	cpuinfo_log_info("mach topology: packages = %d, cores = %d, threads = %d", packages, (int) cores, (int) threads);
	struct cpuinfo_mach_topology topology = {
		.packages = (uint32_t) packages,
		.cores = (uint32_t) cores,
		.threads = (uint32_t) threads
	};

	size_t cacheconfig_size = 0;
	if (sysctlbyname("hw.cacheconfig", NULL, &cacheconfig_size, NULL, 0) != 0) {
		cpuinfo_log_error("sysctlbyname(\"hw.cacheconfig\") failed: %s", strerror(errno));
	} else {
		uint64_t* cacheconfig = alloca(cacheconfig_size);
		if (sysctlbyname("hw.cacheconfig", cacheconfig, &cacheconfig_size, NULL, 0) != 0) {
			cpuinfo_log_error("sysctlbyname(\"hw.cacheconfig\") failed: %s", strerror(errno));
		} else {
			size_t cache_configs = cacheconfig_size / sizeof(uint64_t);
			cpuinfo_log_debug("mach hw.cacheconfig count: %zu", cache_configs);
			if (cache_configs > CPUINFO_MACH_MAX_CACHE_LEVELS) {
				cache_configs = CPUINFO_MACH_MAX_CACHE_LEVELS;
			}
			for (size_t i = 0; i < cache_configs; i++) {
				cpuinfo_log_debug("mach hw.cacheconfig[%zu]: %"PRIu64, i, cacheconfig[i]);
				topology.threads_per_cache[i] = cacheconfig[i];
			}
		}
	}
	return topology;
}
