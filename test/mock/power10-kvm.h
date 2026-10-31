struct cpuinfo_mock_file filesystem[] = {
	{
		.path = "/proc/cpuinfo",
		.size = 1084,
		.content =
			"processor\t: 0\n"
			"cpu\t\t: POWER10 (architected), altivec supported\n"
			"clock\t\t: 2950.000000MHz\n"
			"revision\t: 2.0 (pvr 0080 0200)\n"
			"\n"
			"processor\t: 1\n"
			"cpu\t\t: POWER10 (architected), altivec supported\n"
			"clock\t\t: 2950.000000MHz\n"
			"revision\t: 2.0 (pvr 0080 0200)\n"
			"\n"
			"processor\t: 2\n"
			"cpu\t\t: POWER10 (architected), altivec supported\n"
			"clock\t\t: 2950.000000MHz\n"
			"revision\t: 2.0 (pvr 0080 0200)\n"
			"\n"
			"processor\t: 3\n"
			"cpu\t\t: POWER10 (architected), altivec supported\n"
			"clock\t\t: 2950.000000MHz\n"
			"revision\t: 2.0 (pvr 0080 0200)\n"
			"\n"
			"processor\t: 4\n"
			"cpu\t\t: POWER10 (architected), altivec supported\n"
			"clock\t\t: 2950.000000MHz\n"
			"revision\t: 2.0 (pvr 0080 0200)\n"
			"\n"
			"processor\t: 5\n"
			"cpu\t\t: POWER10 (architected), altivec supported\n"
			"clock\t\t: 2950.000000MHz\n"
			"revision\t: 2.0 (pvr 0080 0200)\n"
			"\n"
			"processor\t: 6\n"
			"cpu\t\t: POWER10 (architected), altivec supported\n"
			"clock\t\t: 2950.000000MHz\n"
			"revision\t: 2.0 (pvr 0080 0200)\n"
			"\n"
			"processor\t: 7\n"
			"cpu\t\t: POWER10 (architected), altivec supported\n"
			"clock\t\t: 2950.000000MHz\n"
			"revision\t: 2.0 (pvr 0080 0200)\n"
			"\n"
			"timebase\t: 512000000\n"
			"platform\t: pSeries\n"
			"model\t\t: IBM pSeries (emulated by qemu)\n"
			"machine\t\t: CHRP IBM pSeries (emulated by qemu)\n"
			"MMU\t\t: Radix\n",
	},
	{
		.path = "/sys/devices/system/cpu/kernel_max",
		.size = 5,
		.content = "8191\n",
	},
	{
		.path = "/sys/devices/system/cpu/possible",
		.size = 4,
		.content = "0-7\n",
	},
	{
		.path = "/sys/devices/system/cpu/present",
		.size = 4,
		.content = "0-7\n",
	},
	{
		.path = "/sys/devices/system/cpu/online",
		.size = 4,
		.content = "0-7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_cpus_list",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_siblings_list",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/package_cpus_list",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/physical_package_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/thread_siblings_list",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cache/index0/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cache/index0/shared_cpu_list",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cache/index0/size",
		.size = 4,
		.content = "32K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cache/index0/type",
		.size = 5,
		.content = "Data\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cache/index1/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cache/index1/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cache/index1/shared_cpu_list",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cache/index1/size",
		.size = 4,
		.content = "48K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cache/index1/type",
		.size = 12,
		.content = "Instruction\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_cpus_list",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_id",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_siblings_list",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/package_cpus_list",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/physical_package_id",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/thread_siblings_list",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cache/index0/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cache/index0/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cache/index0/shared_cpu_list",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cache/index0/size",
		.size = 4,
		.content = "32K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cache/index0/type",
		.size = 5,
		.content = "Data\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cache/index1/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cache/index1/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cache/index1/shared_cpu_list",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cache/index1/size",
		.size = 4,
		.content = "48K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cache/index1/type",
		.size = 12,
		.content = "Instruction\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_cpus_list",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_id",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_siblings_list",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/package_cpus_list",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/physical_package_id",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/thread_siblings_list",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cache/index0/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cache/index0/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cache/index0/shared_cpu_list",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cache/index0/size",
		.size = 4,
		.content = "32K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cache/index0/type",
		.size = 5,
		.content = "Data\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cache/index1/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cache/index1/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cache/index1/shared_cpu_list",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cache/index1/size",
		.size = 4,
		.content = "48K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cache/index1/type",
		.size = 12,
		.content = "Instruction\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_cpus_list",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_id",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_siblings_list",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/package_cpus_list",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/physical_package_id",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/thread_siblings_list",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cache/index0/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cache/index0/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cache/index0/shared_cpu_list",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cache/index0/size",
		.size = 4,
		.content = "32K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cache/index0/type",
		.size = 5,
		.content = "Data\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cache/index1/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cache/index1/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cache/index1/shared_cpu_list",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cache/index1/size",
		.size = 4,
		.content = "48K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cache/index1/type",
		.size = 12,
		.content = "Instruction\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/core_cpus_list",
		.size = 2,
		.content = "4\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/core_id",
		.size = 2,
		.content = "4\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/core_siblings_list",
		.size = 2,
		.content = "4\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/package_cpus_list",
		.size = 2,
		.content = "4\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/physical_package_id",
		.size = 2,
		.content = "4\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/thread_siblings_list",
		.size = 2,
		.content = "4\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cache/index0/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cache/index0/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cache/index0/shared_cpu_list",
		.size = 2,
		.content = "4\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cache/index0/size",
		.size = 4,
		.content = "32K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cache/index0/type",
		.size = 5,
		.content = "Data\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cache/index1/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cache/index1/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cache/index1/shared_cpu_list",
		.size = 2,
		.content = "4\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cache/index1/size",
		.size = 4,
		.content = "48K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cache/index1/type",
		.size = 12,
		.content = "Instruction\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/core_cpus_list",
		.size = 2,
		.content = "5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/core_id",
		.size = 2,
		.content = "5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/core_siblings_list",
		.size = 2,
		.content = "5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/package_cpus_list",
		.size = 2,
		.content = "5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/physical_package_id",
		.size = 2,
		.content = "5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/thread_siblings_list",
		.size = 2,
		.content = "5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cache/index0/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cache/index0/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cache/index0/shared_cpu_list",
		.size = 2,
		.content = "5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cache/index0/size",
		.size = 4,
		.content = "32K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cache/index0/type",
		.size = 5,
		.content = "Data\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cache/index1/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cache/index1/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cache/index1/shared_cpu_list",
		.size = 2,
		.content = "5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cache/index1/size",
		.size = 4,
		.content = "48K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cache/index1/type",
		.size = 12,
		.content = "Instruction\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/core_cpus_list",
		.size = 2,
		.content = "6\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/core_id",
		.size = 2,
		.content = "6\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/core_siblings_list",
		.size = 2,
		.content = "6\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/package_cpus_list",
		.size = 2,
		.content = "6\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/physical_package_id",
		.size = 2,
		.content = "6\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/thread_siblings_list",
		.size = 2,
		.content = "6\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cache/index0/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cache/index0/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cache/index0/shared_cpu_list",
		.size = 2,
		.content = "6\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cache/index0/size",
		.size = 4,
		.content = "32K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cache/index0/type",
		.size = 5,
		.content = "Data\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cache/index1/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cache/index1/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cache/index1/shared_cpu_list",
		.size = 2,
		.content = "6\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cache/index1/size",
		.size = 4,
		.content = "48K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cache/index1/type",
		.size = 12,
		.content = "Instruction\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/core_cpus_list",
		.size = 2,
		.content = "7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/core_id",
		.size = 2,
		.content = "7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/core_siblings_list",
		.size = 2,
		.content = "7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/package_cpus_list",
		.size = 2,
		.content = "7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/physical_package_id",
		.size = 2,
		.content = "7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/thread_siblings_list",
		.size = 2,
		.content = "7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cache/index0/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cache/index0/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cache/index0/shared_cpu_list",
		.size = 2,
		.content = "7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cache/index0/size",
		.size = 4,
		.content = "32K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cache/index0/type",
		.size = 5,
		.content = "Data\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cache/index1/coherency_line_size",
		.size = 4,
		.content = "128\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cache/index1/level",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cache/index1/shared_cpu_list",
		.size = 2,
		.content = "7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cache/index1/size",
		.size = 4,
		.content = "48K\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cache/index1/type",
		.size = 12,
		.content = "Instruction\n",
	},
	{NULL},
};
