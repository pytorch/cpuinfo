struct cpuinfo_mock_file filesystem[] = {
	{
		.path = "/proc/cpuinfo",
		.size = 292,
		.content =
			"processor\t: 0\n"
			"hart\t: 1\n"
			"isa\t: rv64imafdc\n"
			"mmu\t: sv39\n"
			"uarch\t: sifive,u74-mc\n"
			"\n"
			"processor\t: 1\n"
			"hart\t: 2\n"
			"isa\t: rv64imafdc\n"
			"mmu\t: sv39\n"
			"uarch\t: sifive,u74-mc\n"
			"\n"
			"processor\t: 2\n"
			"hart\t: 3\n"
			"isa\t: rv64imafdc\n"
			"mmu\t: sv39\n"
			"uarch\t: sifive,u74-mc\n"
			"\n"
			"processor\t: 3\n"
			"hart\t: 4\n"
			"isa\t: rv64imafdc\n"
			"mmu\t: sv39\n"
			"uarch\t: sifive,u74-mc\n"
			"\n"
	},
	{
		.path = "/sys/devices/system/cpu/kernel_max",
		.size = 3,
		.content = "31\n",
	},
	{
		.path = "/sys/devices/system/cpu/possible",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/present",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/physical_package_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_cpus_list",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_siblings_list",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/cluster_cpus_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/package_cpus_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/thread_siblings_list",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/physical_package_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_cpus_list",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_siblings_list",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/cluster_cpus_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/package_cpus_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_id",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/thread_siblings_list",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/physical_package_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_cpus_list",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_siblings_list",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/cluster_cpus_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/package_cpus_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_id",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/thread_siblings_list",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/physical_package_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_cpus_list",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_siblings_list",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/cluster_cpus_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/package_cpus_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_id",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/thread_siblings_list",
		.size = 2,
		.content = "3\n",
	},
	{ NULL },
};
