struct cpuinfo_mock_file filesystem[] = {
#if CPUINFO_ARCH_ARM64
	{
		.path = "/proc/cpuinfo",
		.size = 1486,
		.content = "Processor\t: AArch64 Processor rev 1 (aarch64)\n"
			   "processor\t: 0\n"
			   "BogoMIPS\t: 3.84\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd03\n"
			   "CPU revision\t: 4\n"
			   "\n"
			   "processor\t: 1\n"
			   "BogoMIPS\t: 3.84\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd03\n"
			   "CPU revision\t: 4\n"
			   "\n"
			   "processor\t: 2\n"
			   "BogoMIPS\t: 3.84\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd03\n"
			   "CPU revision\t: 4\n"
			   "\n"
			   "processor\t: 3\n"
			   "BogoMIPS\t: 3.84\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd03\n"
			   "CPU revision\t: 4\n"
			   "\n"
			   "processor\t: 4\n"
			   "BogoMIPS\t: 3.84\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd09\n"
			   "CPU revision\t: 1\n"
			   "\n"
			   "processor\t: 5\n"
			   "BogoMIPS\t: 3.84\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd09\n"
			   "CPU revision\t: 1\n"
			   "\n"
			   "processor\t: 6\n"
			   "BogoMIPS\t: 3.84\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd09\n"
			   "CPU revision\t: 1\n"
			   "\n"
			   "processor\t: 7\n"
			   "BogoMIPS\t: 3.84\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd09\n"
			   "CPU revision\t: 1\n"
			   "\n",
	},
#elif CPUINFO_ARCH_ARM
	{
		.path = "/proc/cpuinfo",
		.size = 1950,
		.content =
			"Processor\t: AArch64 Processor rev 1 (aarch64)\n"
			"processor\t: 0\n"
			"BogoMIPS\t: 3.84\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd03\n"
			"CPU revision\t: 4\n"
			"\n"
			"processor\t: 1\n"
			"BogoMIPS\t: 3.84\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd03\n"
			"CPU revision\t: 4\n"
			"\n"
			"processor\t: 2\n"
			"BogoMIPS\t: 3.84\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd03\n"
			"CPU revision\t: 4\n"
			"\n"
			"processor\t: 3\n"
			"BogoMIPS\t: 3.84\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd03\n"
			"CPU revision\t: 4\n"
			"\n"
			"processor\t: 4\n"
			"BogoMIPS\t: 3.84\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd09\n"
			"CPU revision\t: 1\n"
			"\n"
			"processor\t: 5\n"
			"BogoMIPS\t: 3.84\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd09\n"
			"CPU revision\t: 1\n"
			"\n"
			"processor\t: 6\n"
			"BogoMIPS\t: 3.84\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd09\n"
			"CPU revision\t: 1\n"
			"\n"
			"processor\t: 7\n"
			"BogoMIPS\t: 3.84\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd09\n"
			"CPU revision\t: 1\n"
			"\n",
	},
#endif
	{
		.path = "/system/build.prop",
		.size = 2117,
		.content =
			"\n"
			"# begin build properties\n"
			"# autogenerated by buildinfo.sh\n"
			"ro.build.id=NRD90M\n"
			"ro.build.display.id=NRD90M test-keys\n"
			"ro.build.version.incremental=eng.jslave.20161103.091030\n"
			"ro.build.version.sdk=24\n"
			"ro.build.version.preview_sdk=0\n"
			"ro.build.version.codename=REL\n"
			"ro.build.version.all_codenames=REL\n"
			"ro.build.version.release=7.0\n"
			"ro.build.version.security_patch=2016-08-05\n"
			"ro.build.version.base_os=\n"
			"ro.build.date=Thu Nov  3 09:10:30 CST 2016\n"
			"ro.build.date.utc=1478135430\n"
			"ro.build.type=user\n"
			"ro.build.user=jslave2\n"
			"ro.build.host=szvjk107cna\n"
			"ro.build.tags=test-keys\n"
			"ro.build.flavor=generic_a15-user\n"
			"ro.product.model=generic_a15\n"
			"ro.product.brand=Huawei\n"
			"ro.product.name=generic_a15\n"
			"ro.product.device=generic_a15\n"
			"ro.product.board=\n"
			"# ro.product.cpu.abi and ro.product.cpu.abi2 are obsolete,\n"
			"# use ro.product.cpu.abilist instead.\n"
			"ro.product.cpu.abi=arm64-v8a\n"
			"ro.product.cpu.abilist=arm64-v8a,armeabi-v7a,armeabi\n"
			"ro.product.cpu.abilist32=armeabi-v7a,armeabi\n"
			"ro.product.cpu.abilist64=arm64-v8a\n"
			"ro.product.manufacturer=unknown\n"
			"ro.wifi.channels=\n"
			"ro.board.platform=\n"
			"# ro.build.product is obsolete; use ro.product.device\n"
			"ro.build.product=generic_a15\n"
			"# Do not try to parse description, fingerprint, or thumbprint\n"
			"ro.build.description=generic_a15-user 7.0 NRD90M eng.jslave.20161103.091030 test-keys\n"
			"ro.build.fingerprint=Huawei/generic_a15/generic_a15:7.0/NRD90M/jslave11030910:user/test-keys\n"
			"ro.build.characteristics=default\n"
			"# end build properties\n"
			"\n"
			"# begin huawei emui properties\n"
			"# autogenerated by build_emui_info.sh\n"
			"ro.build.hw_emui_api_level=11\n"
			"# end huawei emui properties\n"
			"\n"
			"\n"
			"#\n"
			"# ADDITIONAL_BUILD_PROPERTIES\n"
			"#\n"
			"ro.config.ringtone=Ring_Synth_04.ogg\n"
			"ro.config.notification_sound=pixiedust.ogg\n"
			"ro.carrier=unknown\n"
			"ro.setupwizard.require_network=any\n"
			"ro.setupwizard.mode=OPTIONAL\n"
			"ro.com.google.gmsversion=7.0_r1\n"
			"persist.sys.dalvik.vm.lib.2=libart.so\n"
			"dalvik.vm.isa.arm64.variant=generic\n"
			"dalvik.vm.isa.arm64.features=default\n"
			"dalvik.vm.isa.arm.variant=cortex-a15\n"
			"dalvik.vm.isa.arm.features=default\n"
			"net.bt.name=Android\n"
			"dalvik.vm.stack-trace-file=/data/anr/traces.txt\n"
			"\n"
			"ro.comp.sys_support_vndk=VA7.0-E5.0-B10005\n"
			"\n"
			"ro.comp.sys_need_sndk=SA7.0-E5.0-B10005\n",
	},
	{
		.path = "/sys/devices/system/cpu/kernel_max",
		.size = 2,
		.content = "7\n",
	},
	{
		.path = "/sys/devices/system/cpu/modalias",
		.size = 66,
		.content = "cpu:type:aarch64:feature:,0000,0001,0002,0003,0004,0005,0006,0007\n",
	},
	{
		.path = "/sys/devices/system/cpu/offline",
		.size = 1,
		.content = "\n",
	},
	{
		.path = "/sys/devices/system/cpu/online",
		.size = 4,
		.content = "0-7\n",
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
		.path = "/sys/devices/system/cpu/cpuidle/current_driver",
		.size = 22,
		.content = "hisi_big_cluster_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpuidle/current_governor_ro",
		.size = 5,
		.content = "menu\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpuidle/driver/name",
		.size = 25,
		.content = "hisi_little_cluster_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/affected_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1844000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "533000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_transition_latency",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/related_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies",
		.size = 39,
		.content = "533000 999000 1402000 1709000 1844000 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors",
		.size = 67,
		.content = "interactive conservative ondemand userspace powersave performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "533000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_driver",
		.size = 15,
		.content = "arm-big-little\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "533000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/stats/time_in_state",
		.size = 60,
		.content = "533000 1865\n"
			   "999000 78\n"
			   "1402000 1860\n"
			   "1709000 231\n"
			   "1844000 7168\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/stats/total_trans",
		.size = 4,
		.content = "188\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/stats/trans_table",
		.size = 389,
		.content = "   From  :    To\n"
			   "         :    533000    999000   1402000   1709000   1844000 \n"
			   "   533000:         0         0        47         0         0 \n"
			   "   999000:         4         0         3         0         0 \n"
			   "  1402000:        29         3         0        32         3 \n"
			   "  1709000:         6         2        11         0        22 \n"
			   "  1844000:         9         2         6         9         0 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_siblings",
		.size = 3,
		.content = "0f\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_siblings_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/physical_package_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/thread_siblings",
		.size = 3,
		.content = "01\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/thread_siblings_list",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpuidle/driver/name",
		.size = 25,
		.content = "hisi_little_cluster_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/affected_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1844000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "533000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_transition_latency",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/related_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_available_frequencies",
		.size = 39,
		.content = "533000 999000 1402000 1709000 1844000 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_available_governors",
		.size = 67,
		.content = "interactive conservative ondemand userspace powersave performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "533000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_driver",
		.size = 15,
		.content = "arm-big-little\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "533000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/stats/time_in_state",
		.size = 60,
		.content = "533000 2045\n"
			   "999000 78\n"
			   "1402000 1898\n"
			   "1709000 231\n"
			   "1844000 7168\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/stats/total_trans",
		.size = 4,
		.content = "190\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/stats/trans_table",
		.size = 389,
		.content = "   From  :    To\n"
			   "         :    533000    999000   1402000   1709000   1844000 \n"
			   "   533000:         0         0        48         0         0 \n"
			   "   999000:         4         0         3         0         0 \n"
			   "  1402000:        30         3         0        32         3 \n"
			   "  1709000:         6         2        11         0        22 \n"
			   "  1844000:         9         2         6         9         0 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_id",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_siblings",
		.size = 3,
		.content = "0f\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_siblings_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/physical_package_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/thread_siblings",
		.size = 3,
		.content = "02\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/thread_siblings_list",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpuidle/driver/name",
		.size = 25,
		.content = "hisi_little_cluster_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/affected_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1844000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "533000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_transition_latency",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/related_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_available_frequencies",
		.size = 39,
		.content = "533000 999000 1402000 1709000 1844000 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_available_governors",
		.size = 67,
		.content = "interactive conservative ondemand userspace powersave performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1402000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_driver",
		.size = 15,
		.content = "arm-big-little\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "533000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/stats/time_in_state",
		.size = 60,
		.content = "533000 2218\n"
			   "999000 78\n"
			   "1402000 1932\n"
			   "1709000 234\n"
			   "1844000 7179\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/stats/total_trans",
		.size = 4,
		.content = "195\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/stats/trans_table",
		.size = 389,
		.content = "   From  :    To\n"
			   "         :    533000    999000   1402000   1709000   1844000 \n"
			   "   533000:         0         0        50         0         0 \n"
			   "   999000:         4         0         3         0         0 \n"
			   "  1402000:        31         3         0        33         3 \n"
			   "  1709000:         6         2        11         0        23 \n"
			   "  1844000:        10         2         6         9         0 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_id",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_siblings",
		.size = 3,
		.content = "0f\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_siblings_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/physical_package_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/thread_siblings",
		.size = 3,
		.content = "04\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/thread_siblings_list",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpuidle/driver/name",
		.size = 25,
		.content = "hisi_little_cluster_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/affected_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1844000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "533000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_transition_latency",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/related_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_available_frequencies",
		.size = 39,
		.content = "533000 999000 1402000 1709000 1844000 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_available_governors",
		.size = 67,
		.content = "interactive conservative ondemand userspace powersave performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "533000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_driver",
		.size = 15,
		.content = "arm-big-little\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "533000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/stats/time_in_state",
		.size = 60,
		.content = "533000 2442\n"
			   "999000 78\n"
			   "1402000 1937\n"
			   "1709000 234\n"
			   "1844000 7179\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/stats/total_trans",
		.size = 4,
		.content = "196\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/stats/trans_table",
		.size = 389,
		.content = "   From  :    To\n"
			   "         :    533000    999000   1402000   1709000   1844000 \n"
			   "   533000:         0         0        50         0         0 \n"
			   "   999000:         4         0         3         0         0 \n"
			   "  1402000:        31         3         0        33         3 \n"
			   "  1709000:         6         2        11         0        23 \n"
			   "  1844000:        10         2         6         9         0 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_id",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_siblings",
		.size = 3,
		.content = "0f\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_siblings_list",
		.size = 4,
		.content = "0-3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/physical_package_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/thread_siblings",
		.size = 3,
		.content = "08\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/thread_siblings_list",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpuidle/driver/name",
		.size = 22,
		.content = "hisi_big_cluster_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/affected_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "2362000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_transition_latency",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/related_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_available_frequencies",
		.size = 40,
		.content = "903000 1421000 1805000 2112000 2362000 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_available_governors",
		.size = 67,
		.content = "interactive conservative ondemand userspace powersave performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_driver",
		.size = 15,
		.content = "arm-big-little\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq",
		.size = 8,
		.content = "2362000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/stats/time_in_state",
		.size = 60,
		.content = "903000 6470\n"
			   "1421000 63\n"
			   "1805000 522\n"
			   "2112000 264\n"
			   "2362000 4777\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/stats/total_trans",
		.size = 4,
		.content = "218\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/stats/trans_table",
		.size = 389,
		.content = "   From  :    To\n"
			   "         :    903000   1421000   1805000   2112000   2362000 \n"
			   "   903000:         0         0        59         2         3 \n"
			   "  1421000:         3         0         6         0         2 \n"
			   "  1805000:        15         5         0         2        49 \n"
			   "  2112000:         2         0         0         0         8 \n"
			   "  2362000:        45         5         6         6         0 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/core_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/core_siblings",
		.size = 3,
		.content = "f0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/core_siblings_list",
		.size = 4,
		.content = "4-7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/physical_package_id",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/thread_siblings",
		.size = 3,
		.content = "10\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/thread_siblings_list",
		.size = 2,
		.content = "4\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpuidle/driver/name",
		.size = 22,
		.content = "hisi_big_cluster_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/affected_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "2362000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/cpuinfo_transition_latency",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/related_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_available_frequencies",
		.size = 40,
		.content = "903000 1421000 1805000 2112000 2362000 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_available_governors",
		.size = 67,
		.content = "interactive conservative ondemand userspace powersave performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_driver",
		.size = 15,
		.content = "arm-big-little\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_max_freq",
		.size = 8,
		.content = "2362000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/stats/time_in_state",
		.size = 60,
		.content = "903000 6703\n"
			   "1421000 63\n"
			   "1805000 522\n"
			   "2112000 264\n"
			   "2362000 4777\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/stats/total_trans",
		.size = 4,
		.content = "218\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/stats/trans_table",
		.size = 389,
		.content = "   From  :    To\n"
			   "         :    903000   1421000   1805000   2112000   2362000 \n"
			   "   903000:         0         0        59         2         3 \n"
			   "  1421000:         3         0         6         0         2 \n"
			   "  1805000:        15         5         0         2        49 \n"
			   "  2112000:         2         0         0         0         8 \n"
			   "  2362000:        45         5         6         6         0 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/core_id",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/core_siblings",
		.size = 3,
		.content = "f0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/core_siblings_list",
		.size = 4,
		.content = "4-7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/physical_package_id",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/thread_siblings",
		.size = 3,
		.content = "20\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/thread_siblings_list",
		.size = 2,
		.content = "5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpuidle/driver/name",
		.size = 22,
		.content = "hisi_big_cluster_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/affected_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "2362000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/cpuinfo_transition_latency",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/related_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_available_frequencies",
		.size = 40,
		.content = "903000 1421000 1805000 2112000 2362000 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_available_governors",
		.size = 67,
		.content = "interactive conservative ondemand userspace powersave performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_driver",
		.size = 15,
		.content = "arm-big-little\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_max_freq",
		.size = 8,
		.content = "2362000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/stats/time_in_state",
		.size = 60,
		.content = "903000 6909\n"
			   "1421000 63\n"
			   "1805000 526\n"
			   "2112000 266\n"
			   "2362000 4804\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/stats/total_trans",
		.size = 4,
		.content = "224\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/stats/trans_table",
		.size = 389,
		.content = "   From  :    To\n"
			   "         :    903000   1421000   1805000   2112000   2362000 \n"
			   "   903000:         0         0        61         2         3 \n"
			   "  1421000:         3         0         6         0         2 \n"
			   "  1805000:        15         5         0         3        50 \n"
			   "  2112000:         2         0         0         0         9 \n"
			   "  2362000:        46         5         6         6         0 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/core_id",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/core_siblings",
		.size = 3,
		.content = "f0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/core_siblings_list",
		.size = 4,
		.content = "4-7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/physical_package_id",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/thread_siblings",
		.size = 3,
		.content = "40\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/thread_siblings_list",
		.size = 2,
		.content = "6\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpuidle/driver/name",
		.size = 22,
		.content = "hisi_big_cluster_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/affected_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "2362000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_transition_latency",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/related_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_available_frequencies",
		.size = 40,
		.content = "903000 1421000 1805000 2112000 2362000 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_available_governors",
		.size = 67,
		.content = "interactive conservative ondemand userspace powersave performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_driver",
		.size = 15,
		.content = "arm-big-little\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_max_freq",
		.size = 8,
		.content = "2362000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "903000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/stats/time_in_state",
		.size = 60,
		.content = "903000 7079\n"
			   "1421000 63\n"
			   "1805000 526\n"
			   "2112000 266\n"
			   "2362000 4863\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/stats/total_trans",
		.size = 4,
		.content = "225\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/stats/trans_table",
		.size = 389,
		.content = "   From  :    To\n"
			   "         :    903000   1421000   1805000   2112000   2362000 \n"
			   "   903000:         0         0        61         2         3 \n"
			   "  1421000:         3         0         6         0         2 \n"
			   "  1805000:        15         5         0         3        50 \n"
			   "  2112000:         2         0         0         0         9 \n"
			   "  2362000:        47         5         6         6         0 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/core_id",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/core_siblings",
		.size = 3,
		.content = "f0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/core_siblings_list",
		.size = 4,
		.content = "4-7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/physical_package_id",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/thread_siblings",
		.size = 3,
		.content = "80\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/thread_siblings_list",
		.size = 2,
		.content = "7\n",
	},
	{NULL},
};
#ifdef __ANDROID__
struct cpuinfo_mock_property properties[] = {
	{
		.key = "audioril.lib",
		.value = "libhuawei-audio-ril.so",
	},
	{
		.key = "bastet.service.enable",
		.value = "true",
	},
	{
		.key = "build.hisi_ioloader_opt",
		.value = "true",
	},
	{
		.key = "build.hisi_perf_opt",
		.value = "true",
	},
	{
		.key = "build.hisi_rot_ani_opt",
		.value = "true",
	},
	{
		.key = "camera.dis.flag",
		.value = "2",
	},
	{
		.key = "camera.tnr.flag",
		.value = "1",
	},
	{
		.key = "dalvik.vm.appimageformat",
		.value = "lz4",
	},
	{
		.key = "dalvik.vm.boot-dex2oat-threads",
		.value = "4",
	},
	{
		.key = "dalvik.vm.checkjni",
		.value = "false",
	},
	{
		.key = "dalvik.vm.dex2oat-Xms",
		.value = "64m",
	},
	{
		.key = "dalvik.vm.dex2oat-Xmx",
		.value = "512m",
	},
	{
		.key = "dalvik.vm.dex2oat-threads",
		.value = "4",
	},
	{
		.key = "dalvik.vm.heapgrowthlimit",
		.value = "384m",
	},
	{
		.key = "dalvik.vm.heapmaxfree",
		.value = "8m",
	},
	{
		.key = "dalvik.vm.heapminfree",
		.value = "512k",
	},
	{
		.key = "dalvik.vm.heapsize",
		.value = "512m",
	},
	{
		.key = "dalvik.vm.heapstartsize",
		.value = "8m",
	},
	{
		.key = "dalvik.vm.heaptargetutilization",
		.value = "0.75",
	},
	{
		.key = "dalvik.vm.image-dex2oat-Xms",
		.value = "64m",
	},
	{
		.key = "dalvik.vm.image-dex2oat-Xmx",
		.value = "64m",
	},
	{
		.key = "dalvik.vm.image-dex2oat-threads",
		.value = "4",
	},
	{
		.key = "dalvik.vm.isa.arm.features",
		.value = "default",
	},
	{
		.key = "dalvik.vm.isa.arm.variant",
		.value = "cortex-a15",
	},
	{
		.key = "dalvik.vm.isa.arm64.features",
		.value = "default",
	},
	{
		.key = "dalvik.vm.isa.arm64.variant",
		.value = "generic",
	},
	{
		.key = "dalvik.vm.stack-trace-file",
		.value = "/data/anr/traces.txt",
	},
	{
		.key = "dalvik.vm.usejit",
		.value = "true",
	},
	{
		.key = "dalvik.vm.usejitprofiles",
		.value = "true",
	},
	{
		.key = "debug.aps.current_battery",
		.value = "19",
	},
	{
		.key = "debug.aps.enable",
		.value = "0",
	},
	{
		.key = "debug.aps.process.name",
		.value = "",
	},
	{
		.key = "debug.aps.scene_num",
		.value = "5",
	},
	{
		.key = "debug.atrace.tags.enableflags",
		.value = "0",
	},
	{
		.key = "debug.force_rtl",
		.value = "0",
	},
	{
		.key = "debug.hwui.render_dirty_regions",
		.value = "false",
	},
	{
		.key = "dev.bootcomplete",
		.value = "1",
	},
	{
		.key = "drm.service.enabled",
		.value = "true",
	},
	{
		.key = "dumptool.run",
		.value = "1",
	},
	{
		.key = "fw.max_users",
		.value = "4",
	},
	{
		.key = "fw.show_multiuserui",
		.value = "1",
	},
	{
		.key = "gsm.check_is_single_pdp_sub1",
		.value = "false",
	},
	{
		.key = "gsm.current.phone-type",
		.value = "1,1",
	},
	{
		.key = "gsm.dualcards.switch",
		.value = "false",
	},
	{
		.key = "gsm.fastdormancy.mode",
		.value = "1",
	},
	{
		.key = "gsm.hw.fdn.activated1",
		.value = "false",
	},
	{
		.key = "gsm.hw.fdn.activated2",
		.value = "false",
	},
	{
		.key = "gsm.hw.operator.iso-country",
		.value = "",
	},
	{
		.key = "gsm.hw.operator.isroaming",
		.value = "false",
	},
	{
		.key = "gsm.hw.operator.numeric",
		.value = "",
	},
	{
		.key = "gsm.hw.sim.operator.iso-country",
		.value = "",
	},
	{
		.key = "gsm.network.type",
		.value = "UMTS,Unknown",
	},
	{
		.key = "gsm.operator.alpha",
		.value = "",
	},
	{
		.key = "gsm.operator.iso-country",
		.value = "",
	},
	{
		.key = "gsm.operator.isroaming",
		.value = "false",
	},
	{
		.key = "gsm.operator.numeric",
		.value = "",
	},
	{
		.key = "gsm.sigcust.configured",
		.value = "true",
	},
	{
		.key = "gsm.sim.c_card.plmn",
		.value = "",
	},
	{
		.key = "gsm.sim.hw_atr",
		.value = "null",
	},
	{
		.key = "gsm.sim.hw_atr1",
		.value = "null",
	},
	{
		.key = "gsm.sim.operator.alpha",
		.value = ",",
	},
	{
		.key = "gsm.sim.operator.iso-country",
		.value = ",",
	},
	{
		.key = "gsm.sim.operator.numeric",
		.value = ",",
	},
	{
		.key = "gsm.sim.state",
		.value = "ABSENT,ABSENT",
	},
	{
		.key = "gsm.sim1.type",
		.value = "-1",
	},
	{
		.key = "gsm.sim2.type",
		.value = "-1",
	},
	{
		.key = "gsm.version.baseband",
		.value = "21.500.00.00.031,21.500.00.00.031",
	},
	{
		.key = "gsm.version.ril-impl",
		.value = "android infineon balong-ril 1.0",
	},
	{
		.key = "huawei.check_root.hotapermit",
		.value = "safe",
	},
	{
		.key = "huawei.check_root.oldstate",
		.value = "safe",
	},
	{
		.key = "huawei.check_root.prop",
		.value = "safe",
	},
	{
		.key = "huawei.check_root.setids",
		.value = "safe",
	},
	{
		.key = "huawei.check_root.verifyboot",
		.value = "safe",
	},
	{
		.key = "hw.lcd.density",
		.value = "480",
	},
	{
		.key = "hw.wifi.dns_stat",
		.value = "86,29,1325,1,16202",
	},
	{
		.key = "hw.wifipro.dns_fail_count",
		.value = "4",
	},
	{
		.key = "hw_chargemonitor.log",
		.value = "start",
	},
	{
		.key = "init.svc.HwCamCfgSvr",
		.value = "running",
	},
	{
		.key = "init.svc.accproxy",
		.value = "running",
	},
	{
		.key = "init.svc.adbd",
		.value = "stopping",
	},
	{
		.key = "init.svc.applogcat",
		.value = "stopped",
	},
	{
		.key = "init.svc.audioserver",
		.value = "running",
	},
	{
		.key = "init.svc.bastetd",
		.value = "running",
	},
	{
		.key = "init.svc.bootanim",
		.value = "stopped",
	},
	{
		.key = "init.svc.cameraserver",
		.value = "running",
	},
	{
		.key = "init.svc.chargelogcat",
		.value = "stopped",
	},
	{
		.key = "init.svc.chargemonitor",
		.value = "running",
	},
	{
		.key = "init.svc.check_hota",
		.value = "stopped",
	},
	{
		.key = "init.svc.check_root",
		.value = "stopped",
	},
	{
		.key = "init.svc.clear-bcb",
		.value = "stopped",
	},
	{
		.key = "init.svc.cust_from_init",
		.value = "stopped",
	},
	{
		.key = "init.svc.data_cleanner",
		.value = "stopped",
	},
	{
		.key = "init.svc.debuggerd",
		.value = "running",
	},
	{
		.key = "init.svc.debuggerd64",
		.value = "running",
	},
	{
		.key = "init.svc.defragd",
		.value = "running",
	},
	{
		.key = "init.svc.drm",
		.value = "running",
	},
	{
		.key = "init.svc.eventslogcat",
		.value = "stopped",
	},
	{
		.key = "init.svc.fingerprintd",
		.value = "running",
	},
	{
		.key = "init.svc.gatekeeperd",
		.value = "running",
	},
	{
		.key = "init.svc.gpsd_4774",
		.value = "running",
	},
	{
		.key = "init.svc.gpsdaemon",
		.value = "stopped",
	},
	{
		.key = "init.svc.hdbd",
		.value = "stopped",
	},
	{
		.key = "init.svc.healthd",
		.value = "running",
	},
	{
		.key = "init.svc.hilogcat",
		.value = "stopped",
	},
	{
		.key = "init.svc.hiscoutmanager",
		.value = "stopped",
	},
	{
		.key = "init.svc.hivwserver",
		.value = "running",
	},
	{
		.key = "init.svc.hw_ueventd",
		.value = "running",
	},
	{
		.key = "init.svc.hwnffserver",
		.value = "running",
	},
	{
		.key = "init.svc.hwpged",
		.value = "running",
	},
	{
		.key = "init.svc.imonitor",
		.value = "running",
	},
	{
		.key = "init.svc.inputlogcat",
		.value = "stopped",
	},
	{
		.key = "init.svc.installd",
		.value = "running",
	},
	{
		.key = "init.svc.ipld",
		.value = "running",
	},
	{
		.key = "init.svc.isplogcat",
		.value = "stopped",
	},
	{
		.key = "init.svc.keystore",
		.value = "running",
	},
	{
		.key = "init.svc.kmsglogcat",
		.value = "stopped",
	},
	{
		.key = "init.svc.lhd_4774",
		.value = "running",
	},
	{
		.key = "init.svc.lmkd",
		.value = "running",
	},
	{
		.key = "init.svc.logcat_service",
		.value = "stopped",
	},
	{
		.key = "init.svc.logctl_service",
		.value = "stopped",
	},
	{
		.key = "init.svc.logd",
		.value = "running",
	},
	{
		.key = "init.svc.logd-reinit",
		.value = "stopped",
	},
	{
		.key = "init.svc.macaddr",
		.value = "stopped",
	},
	{
		.key = "init.svc.media",
		.value = "running",
	},
	{
		.key = "init.svc.mediacodec",
		.value = "running",
	},
	{
		.key = "init.svc.mediadrm",
		.value = "running",
	},
	{
		.key = "init.svc.mediaextractor",
		.value = "running",
	},
	{
		.key = "init.svc.netd",
		.value = "running",
	},
	{
		.key = "init.svc.oeminfo_nvm",
		.value = "running",
	},
	{
		.key = "init.svc.p2p_supplicant",
		.value = "running",
	},
	{
		.key = "init.svc.perfhub",
		.value = "running",
	},
	{
		.key = "init.svc.powerlogd",
		.value = "running",
	},
	{
		.key = "init.svc.ril-daemon",
		.value = "running",
	},
	{
		.key = "init.svc.rillogcat",
		.value = "stopped",
	},
	{
		.key = "init.svc.servicemanager",
		.value = "running",
	},
	{
		.key = "init.svc.shlogd",
		.value = "stopped",
	},
	{
		.key = "init.svc.sleeplogcat",
		.value = "stopped",
	},
	{
		.key = "init.svc.surfaceflinger",
		.value = "running",
	},
	{
		.key = "init.svc.teecd",
		.value = "running",
	},
	{
		.key = "init.svc.teelogcat",
		.value = "stopped",
	},
	{
		.key = "init.svc.thermal-daemon",
		.value = "running",
	},
	{
		.key = "init.svc.tui-daemon",
		.value = "running",
	},
	{
		.key = "init.svc.ueventd",
		.value = "running",
	},
	{
		.key = "init.svc.vold",
		.value = "running",
	},
	{
		.key = "init.svc.vr_daemon",
		.value = "running",
	},
	{
		.key = "init.svc.zygote",
		.value = "running",
	},
	{
		.key = "init.svc.zygote_secondary",
		.value = "running",
	},
	{
		.key = "keyguard.no_require_sim",
		.value = "true",
	},
	{
		.key = "modify_ram_show",
		.value = "true",
	},
	{
		.key = "net.bt.name",
		.value = "Android",
	},
	{
		.key = "net.change",
		.value = "net.ntp.timereference",
	},
	{
		.key = "net.dns1",
		.value = "208.67.222.222",
	},
	{
		.key = "net.dns2",
		.value = "208.67.220.220",
	},
	{
		.key = "net.hostname",
		.value = "HUAWEI_Mate_9",
	},
	{
		.key = "net.ntp.time",
		.value = "1524186154191",
	},
	{
		.key = "net.ntp.timereference",
		.value = "22290",
	},
	{
		.key = "net.portal.background",
		.value = "false",
	},
	{
		.key = "net.qtaguid_enabled",
		.value = "1",
	},
	{
		.key = "net.tcp.default_init_rwnd",
		.value = "60",
	},
	{
		.key = "net.tethering.noprovisioning",
		.value = "true",
	},
	{
		.key = "partition.cust.verified",
		.value = "2",
	},
	{
		.key = "partition.product.verified",
		.value = "2",
	},
	{
		.key = "partition.system.verified",
		.value = "2",
	},
	{
		.key = "partition.vendor.verified",
		.value = "2",
	},
	{
		.key = "partition.version.verified",
		.value = "2",
	},
	{
		.key = "persist.app_auto_start",
		.value = "true",
	},
	{
		.key = "persist.config.hw.lca.region",
		.value = "inland",
	},
	{
		.key = "persist.dbg.volte_avail_ovr",
		.value = "1",
	},
	{
		.key = "persist.decode_bitmap_opt",
		.value = "1",
	},
	{
		.key = "persist.decodebitmap_async",
		.value = "1",
	},
	{
		.key = "persist.dsds.enabled",
		.value = "true",
	},
	{
		.key = "persist.egl.support_vr",
		.value = "1",
	},
	{
		.key = "persist.fb_auto_alloc",
		.value = "1",
	},
	{
		.key = "persist.fw.force_adoptable",
		.value = "true",
	},
	{
		.key = "persist.media.offload.enable",
		.value = "true",
	},
	{
		.key = "persist.media.usbvoice.enable",
		.value = "true",
	},
	{
		.key = "persist.media.usbvoice.name",
		.value = "USB-Audio - HUAWEI GLASS",
	},
	{
		.key = "persist.partial_update_support",
		.value = "1",
	},
	{
		.key = "persist.radio.activemodem",
		.value = "1",
	},
	{
		.key = "persist.radio.airmode_sim0",
		.value = "false",
	},
	{
		.key = "persist.radio.airmode_sim1",
		.value = "true",
	},
	{
		.key = "persist.radio.apm_sim_not_pwdn",
		.value = "1",
	},
	{
		.key = "persist.radio.ca_rel_ver",
		.value = "11",
	},
	{
		.key = "persist.radio.commril_mode",
		.value = "HISI_CGUL_MODE",
	},
	{
		.key = "persist.radio.lte_enabled",
		.value = "true",
	},
	{
		.key = "persist.radio.lte_rel_ver",
		.value = "11",
	},
	{
		.key = "persist.radio.modem.cap",
		.value = "09B9D",
	},
	{
		.key = "persist.radio.multisim.config",
		.value = "dsds",
	},
	{
		.key = "persist.radio.overseas_mode",
		.value = "true",
	},
	{
		.key = "persist.radio.prefer_data_modem",
		.value = "0",
	},
	{
		.key = "persist.radio.prefer_nw",
		.value = "030201",
	},
	{
		.key = "persist.radio.prefer_nw_modem1",
		.value = "0201",
	},
	{
		.key = "persist.radio.procsetdata",
		.value = "com.huawei.hwstartupguide",
	},
	{
		.key = "persist.radio.sub_state_cfg",
		.value = "1,1,1",
	},
	{
		.key = "persist.radio.usersetdata",
		.value = "true",
	},
	{
		.key = "persist.rog_feature",
		.value = "1",
	},
	{
		.key = "persist.service.hdb.enable",
		.value = "true",
	},
	{
		.key = "persist.service.tm2.tofile",
		.value = "false",
	},
	{
		.key = "persist.smart_pool",
		.value = "1",
	},
	{
		.key = "persist.sys.boost.durationms",
		.value = "1000",
	},
	{
		.key = "persist.sys.boost.freqmin.b",
		.value = "1805000",
	},
	{
		.key = "persist.sys.cpuset.bg_off",
		.value = "0-3",
	},
	{
		.key = "persist.sys.cpuset.bg_on",
		.value = "2-3",
	},
	{
		.key = "persist.sys.cpuset.boost_off",
		.value = "0-7",
	},
	{
		.key = "persist.sys.cpuset.boost_on",
		.value = "4-7",
	},
	{
		.key = "persist.sys.cpuset.enable",
		.value = "1",
	},
	{
		.key = "persist.sys.cpuset.fg_off",
		.value = "0-7",
	},
	{
		.key = "persist.sys.cpuset.fg_on",
		.value = "0-7",
	},
	{
		.key = "persist.sys.cpuset.keybg_off",
		.value = "0-3",
	},
	{
		.key = "persist.sys.cpuset.keybg_on",
		.value = "2-3,7",
	},
	{
		.key = "persist.sys.cpuset.subswitch",
		.value = "251",
	},
	{
		.key = "persist.sys.cpuset.sysbg_off",
		.value = "0-3",
	},
	{
		.key = "persist.sys.cpuset.sysbg_on",
		.value = "0-3",
	},
	{
		.key = "persist.sys.cpuset.ta_boost_off",
		.value = "0-7",
	},
	{
		.key = "persist.sys.cpuset.ta_boost_on",
		.value = "4-7",
	},
	{
		.key = "persist.sys.cpuset.topapp_off",
		.value = "0-7",
	},
	{
		.key = "persist.sys.cpuset.topapp_on",
		.value = "0-7",
	},
	{
		.key = "persist.sys.dalvik.vm.lib.2",
		.value = "libart.so",
	},
	{
		.key = "persist.sys.dualcards",
		.value = "true",
	},
	{
		.key = "persist.sys.enable_iaware",
		.value = "true",
	},
	{
		.key = "persist.sys.fingerpressnavi",
		.value = "0",
	},
	{
		.key = "persist.sys.fingersense",
		.value = "1",
	},
	{
		.key = "persist.sys.huawei.debug.on",
		.value = "0",
	},
	{
		.key = "persist.sys.hwairplanestate",
		.value = "error",
	},
	{
		.key = "persist.sys.iaware.cpuenable",
		.value = "true",
	},
	{
		.key = "persist.sys.iaware_config_ver",
		.value = "MHA-L29_US_iaware_config_1.0_rev.xml",
	},
	{
		.key = "persist.sys.io.f2fs.defrag",
		.value = "1",
	},
	{
		.key = "persist.sys.jankdb",
		.value = "<10><400><400><400><400><400>",
	},
	{
		.key = "persist.sys.jankenable",
		.value = "true",
	},
	{
		.key = "persist.sys.kmemleak.debug",
		.value = "0",
	},
	{
		.key = "persist.sys.locale",
		.value = "en-US",
	},
	{
		.key = "persist.sys.logsystem.coredump",
		.value = "off",
	},
	{
		.key = "persist.sys.logsystem.modem",
		.value = "0",
	},
	{
		.key = "persist.sys.logsystem.protohint",
		.value = "0",
	},
	{
		.key = "persist.sys.media.video.encodec",
		.value = "video/avc",
	},
	{
		.key = "persist.sys.nosave.cpu.load.b",
		.value = "90",
	},
	{
		.key = "persist.sys.nosave.cpu.load.l",
		.value = "95",
	},
	{
		.key = "persist.sys.nosave.hispeed.b",
		.value = "1421000",
	},
	{
		.key = "persist.sys.nosave.hispeed.l",
		.value = "1402000",
	},
	{
		.key = "persist.sys.nosave.targets.b",
		.value = "85:1421000:90:1805000:95",
	},
	{
		.key = "persist.sys.nosave.targets.l",
		.value = "75:999000:85:1402000:90:1709000:95",
	},
	{
		.key = "persist.sys.performance",
		.value = "true",
	},
	{
		.key = "persist.sys.powerup_reason",
		.value = "NORMAL",
	},
	{
		.key = "persist.sys.profiler_ms",
		.value = "0",
	},
	{
		.key = "persist.sys.root.status",
		.value = "0",
	},
	{
		.key = "persist.sys.save.cpu.load.b",
		.value = "90",
	},
	{
		.key = "persist.sys.save.cpu.load.l",
		.value = "95",
	},
	{
		.key = "persist.sys.save.hispeed.b",
		.value = "1421000",
	},
	{
		.key = "persist.sys.save.hispeed.l",
		.value = "1402000",
	},
	{
		.key = "persist.sys.save.targets.b",
		.value = "85:1421000:90:1805000:95",
	},
	{
		.key = "persist.sys.save.targets.l",
		.value = "75:999000:85:1402000:90:1709000:95",
	},
	{
		.key = "persist.sys.set.hispeed.b",
		.value = "1805000",
	},
	{
		.key = "persist.sys.set.hispeed.l",
		.value = "1402000",
	},
	{
		.key = "persist.sys.set.load.b",
		.value = "85",
	},
	{
		.key = "persist.sys.set.load.l",
		.value = "90",
	},
	{
		.key = "persist.sys.set.targets.b",
		.value = "85:1805000:90:2112000:80",
	},
	{
		.key = "persist.sys.set.targets.l",
		.value = "90:1402000:85",
	},
	{
		.key = "persist.sys.shut_alarm",
		.value = "none",
	},
	{
		.key = "persist.sys.spsave.cpu.load.b",
		.value = "90",
	},
	{
		.key = "persist.sys.spsave.cpu.load.l",
		.value = "95",
	},
	{
		.key = "persist.sys.spsave.hispeed.b",
		.value = "1421000",
	},
	{
		.key = "persist.sys.spsave.hispeed.l",
		.value = "1402000",
	},
	{
		.key = "persist.sys.spsave.targets.b",
		.value = "85:1421000:90:1805000:95",
	},
	{
		.key = "persist.sys.spsave.targets.l",
		.value = "75:999000:85:1402000:90:1709000:95",
	},
	{
		.key = "persist.sys.timezone",
		.value = "America/Los_Angeles",
	},
	{
		.key = "persist.sys.usb.config",
		.value = "hisuite,mtp,mass_storage,adb",
	},
	{
		.key = "persist.sys.webview.vmsize",
		.value = "118450112",
	},
	{
		.key = "persist.touch_move_opt",
		.value = "1",
	},
	{
		.key = "persist.touch_vsync_opt",
		.value = "1",
	},
	{
		.key = "pm.dexopt.ab-ota",
		.value = "speed-profile",
	},
	{
		.key = "pm.dexopt.bg-dexopt",
		.value = "speed-profile",
	},
	{
		.key = "pm.dexopt.boot",
		.value = "verify-profile",
	},
	{
		.key = "pm.dexopt.core-app",
		.value = "speed",
	},
	{
		.key = "pm.dexopt.first-boot",
		.value = "interpret-only",
	},
	{
		.key = "pm.dexopt.forced-dexopt",
		.value = "speed",
	},
	{
		.key = "pm.dexopt.install",
		.value = "interpret-only",
	},
	{
		.key = "pm.dexopt.nsys-library",
		.value = "speed",
	},
	{
		.key = "pm.dexopt.shared-apk",
		.value = "speed",
	},
	{
		.key = "ril.balong_cid",
		.value = "0",
	},
	{
		.key = "ril.ecclist",
		.value = "112,911,000,08,110,118,119,999,911,112",
	},
	{
		.key = "ril.ecclist1",
		.value = "112,911,000,08,110,118,119,999",
	},
	{
		.key = "ril.force_to_set_ecc",
		.value = "invalid",
	},
	{
		.key = "ril.hw_modem0.rssi",
		.value = "-1",
	},
	{
		.key = "ril.hw_modem1.rssi",
		.value = "-1",
	},
	{
		.key = "ril.hw_modem2.rssi",
		.value = "-1",
	},
	{
		.key = "ril.modem.balong_nvm_server",
		.value = "true",
	},
	{
		.key = "ril.operator.numeric",
		.value = "310260",
	},
	{
		.key = "rild.libargs",
		.value = "-m modem0",
	},
	{
		.key = "rild.libargs1",
		.value = "-m modem1",
	},
	{
		.key = "rild.libargs2",
		.value = "-m modem2",
	},
	{
		.key = "rild.libpath",
		.value = "/vendor/lib64/libbalong-ril.so",
	},
	{
		.key = "rild.libpath1",
		.value = "/vendor/lib64/libbalong-ril-1.so",
	},
	{
		.key = "rild.libpath2",
		.value = "/vendor/lib64/libbalong-ril-2.so",
	},
	{
		.key = "rild.rild1_ready_to_start",
		.value = "false",
	},
	{
		.key = "ro.adb.secure",
		.value = "1",
	},
	{
		.key = "ro.allow.mock.location",
		.value = "0",
	},
	{
		.key = "ro.baseband",
		.value = "unknown",
	},
	{
		.key = "ro.board.boardid",
		.value = "5509",
	},
	{
		.key = "ro.board.boardname",
		.value = "MHA_TUGL_VN2",
	},
	{
		.key = "ro.board.modemid",
		.value = "3a001400",
	},
	{
		.key = "ro.board.platform",
		.value = "hi3660",
	},
	{
		.key = "ro.boardid.product",
		.value = "3524",
	},
	{
		.key = "ro.boot.ddrsize",
		.value = "4",
	},
	{
		.key = "ro.boot.hardware",
		.value = "hi3660",
	},
	{
		.key = "ro.boot.mode",
		.value = "normal",
	},
	{
		.key = "ro.boot.selinux",
		.value = "enforcing",
	},
	{
		.key = "ro.boot.serialno",
		.value = "AHK0216C29000389",
	},
	{
		.key = "ro.boot.verifiedbootstate",
		.value = "GREEN",
	},
	{
		.key = "ro.boot.veritymode",
		.value = "enforcing",
	},
	{
		.key = "ro.bootimage.build.date",
		.value = "Thu Nov 3 12:20:37 CST 2016",
	},
	{
		.key = "ro.bootimage.build.date.utc",
		.value = "1478146837",
	},
	{
		.key = "ro.bootimage.build.fingerprint",
		.value = "hi3660/hi3660/hi3660:7.0/NRD90M/jenkin11031219:user/test-keys",
	},
	{
		.key = "ro.bootloader",
		.value = "unknown",
	},
	{
		.key = "ro.bootmode",
		.value = "normal",
	},
	{
		.key = "ro.bt.bdaddr_path",
		.value = "/data/misc/bluedroid/macbt",
	},
	{
		.key = "ro.build.beta",
		.value = "false",
	},
	{
		.key = "ro.build.characteristics",
		.value = "default",
	},
	{
		.key = "ro.build.date",
		.value = "Thu Nov  3 09:10:30 CST 2016",
	},
	{
		.key = "ro.build.date.utc",
		.value = "1478135430",
	},
	{
		.key = "ro.build.description",
		.value = "MHA-L29-user 7.0 HUAWEIMHA-L29 C636B112 release-keys",
	},
	{
		.key = "ro.build.diaplay.id",
		.value = "System 5.0.0.48(03AE)",
	},
	{
		.key = "ro.build.display.id",
		.value = "MHA-L29C636B112",
	},
	{
		.key = "ro.build.fingerprint",
		.value = "HUAWEI/MHA-L29/HWMHA:7.0/HUAWEIMHA-L29/C636B112:user/release-keys",
	},
	{
		.key = "ro.build.flavor",
		.value = "hi3660-user",
	},
	{
		.key = "ro.build.hide",
		.value = "false",
	},
	{
		.key = "ro.build.hide.matchers",
		.value = "MHA;mha;hi3660;huawei;mali-t880",
	},
	{
		.key = "ro.build.hide.replacements",
		.value = "MHA;MHA;unknown;unknown;unknown;",
	},
	{
		.key = "ro.build.hide.settings",
		.value = "8;1.8 GHz;2.0GB;11.00 GB;16.00 GB;1920 x 1080;5.1;3.10.30;3.1",
	},
	{
		.key = "ro.build.host",
		.value = "android-01of24",
	},
	{
		.key = "ro.build.hw_emui_api_level",
		.value = "11",
	},
	{
		.key = "ro.build.id",
		.value = "HUAWEIMHA-L29",
	},
	{
		.key = "ro.build.product",
		.value = "MHA",
	},
	{
		.key = "ro.build.tags",
		.value = "release-keys",
	},
	{
		.key = "ro.build.type",
		.value = "user",
	},
	{
		.key = "ro.build.update_version",
		.value = "V1_2",
	},
	{
		.key = "ro.build.user",
		.value = "test",
	},
	{
		.key = "ro.build.version.all_codenames",
		.value = "REL",
	},
	{
		.key = "ro.build.version.base_os",
		.value = "",
	},
	{
		.key = "ro.build.version.codename",
		.value = "REL",
	},
	{
		.key = "ro.build.version.emui",
		.value = "EmotionUI_5.0",
	},
	{
		.key = "ro.build.version.incremental",
		.value = "C636B112",
	},
	{
		.key = "ro.build.version.preview_sdk",
		.value = "0",
	},
	{
		.key = "ro.build.version.release",
		.value = "7.0",
	},
	{
		.key = "ro.build.version.sdk",
		.value = "24",
	},
	{
		.key = "ro.build.version.security_patch",
		.value = "2016-10-01",
	},
	{
		.key = "ro.carrier",
		.value = "unknown",
	},
	{
		.key = "ro.cdma.home.operator.numeric",
		.value = "46003",
	},
	{
		.key = "ro.cellbroadcast.emergencyids",
		.value = "0-65534",
	},
	{
		.key = "ro.check.modem_network",
		.value = "true",
	},
	{
		.key = "ro.com.google.clientidbase",
		.value = "android-huawei",
	},
	{
		.key = "ro.com.google.clientidbase.am",
		.value = "android-huawei",
	},
	{
		.key = "ro.com.google.clientidbase.gmm",
		.value = "android-huawei",
	},
	{
		.key = "ro.com.google.clientidbase.ms",
		.value = "android-huawei",
	},
	{
		.key = "ro.com.google.clientidbase.yt",
		.value = "android-huawei",
	},
	{
		.key = "ro.com.google.gmsversion",
		.value = "7.0_r1",
	},
	{
		.key = "ro.comp.chip_need_vndk",
		.value = "VA7.0-E5.0-B10005",
	},
	{
		.key = "ro.comp.chip_support_sndk",
		.value = "SA7.0-E5.0-B10005",
	},
	{
		.key = "ro.comp.chipset_version",
		.value = "Chipset-chicago 5.0.0.101SP01(005B)",
	},
	{
		.key = "ro.comp.sys_need_sndk",
		.value = "SA7.0-E5.0-B10005",
	},
	{
		.key = "ro.comp.sys_support_vndk",
		.value = "VA7.0-E5.0-B10005",
	},
	{
		.key = "ro.comp.system_version",
		.value = "System 5.0.0.48(03AE)",
	},
	{
		.key = "ro.confg.hw_bootversion",
		.value = "Chipset-chicago 5.0.0.101SP01(005B)_BOOT",
	},
	{
		.key = "ro.confg.hw_fastbootversion",
		.value = "Chipset-chicago 5.0.0.101SP01(005B)_FASTBOOT",
	},
	{
		.key = "ro.confg.hw_systemversion",
		.value = "System 5.0.0.48(03AE)",
	},
	{
		.key = "ro.confg.hw_userdataversion",
		.value = "MHA-L29C636B112_DATA_hw_spcseas",
	},
	{
		.key = "ro.config.alarm_alert",
		.value = "Aegean_Sea.ogg",
	},
	{
		.key = "ro.config.app_big_icon_size",
		.value = "161",
	},
	{
		.key = "ro.config.attach_apn_enabled",
		.value = "true",
	},
	{
		.key = "ro.config.attach_ip_type",
		.value = "IPV4V6PCSCF",
	},
	{
		.key = "ro.config.backcolor",
		.value = "white",
	},
	{
		.key = "ro.config.beta_sec_ctrl",
		.value = "false",
	},
	{
		.key = "ro.config.both_m0_m2_gsm_cap",
		.value = "true",
	},
	{
		.key = "ro.config.c2dmdelay",
		.value = "30000",
	},
	{
		.key = "ro.config.ca_withoutcat",
		.value = "true",
	},
	{
		.key = "ro.config.calendar_event_order",
		.value = "true",
	},
	{
		.key = "ro.config.callinwifi",
		.value = "200,6",
	},
	{
		.key = "ro.config.carkitmodenotif",
		.value = "true",
	},
	{
		.key = "ro.config.client_number",
		.value = "5",
	},
	{
		.key = "ro.config.colorTemperature_3d",
		.value = "true",
	},
	{
		.key = "ro.config.colorTemperature_K3",
		.value = "true",
	},
	{
		.key = "ro.config.compatibility_enable",
		.value = "1",
	},
	{
		.key = "ro.config.data_preinstalled",
		.value = "true",
	},
	{
		.key = "ro.config.default_commril_mode",
		.value = "HISI_CGUL_MODE",
	},
	{
		.key = "ro.config.devicecolor",
		.value = "white",
	},
	{
		.key = "ro.config.dist_nosim_airplane",
		.value = "true",
	},
	{
		.key = "ro.config.dsds_mode",
		.value = "cdma_gsm",
	},
	{
		.key = "ro.config.dualsim_display",
		.value = "true",
	},
	{
		.key = "ro.config.emergencycall_show",
		.value = "true",
	},
	{
		.key = "ro.config.enable_c2dm_delay",
		.value = "true",
	},
	{
		.key = "ro.config.enable_iaware",
		.value = "true",
	},
	{
		.key = "ro.config.enable_perfhub_fling",
		.value = "false",
	},
	{
		.key = "ro.config.finger_joint",
		.value = "true",
	},
	{
		.key = "ro.config.fix_commril_mode",
		.value = "false",
	},
	{
		.key = "ro.config.fp_navigation",
		.value = "true",
	},
	{
		.key = "ro.config.full_network_support",
		.value = "true",
	},
	{
		.key = "ro.config.gcm_hb_speed_factor",
		.value = "4",
	},
	{
		.key = "ro.config.gcm_hb_speedup",
		.value = "true",
	},
	{
		.key = "ro.config.google_smart_lock",
		.value = "true",
	},
	{
		.key = "ro.config.hebrew_dialpad_show",
		.value = "true",
	},
	{
		.key = "ro.config.helix_enable",
		.value = "true",
	},
	{
		.key = "ro.config.hideContactWithoutNum",
		.value = "true",
	},
	{
		.key = "ro.config.hide_ecc_no_net",
		.value = "true",
	},
	{
		.key = "ro.config.hisi_cdma_supported",
		.value = "true",
	},
	{
		.key = "ro.config.hk_3gpp_notdownload",
		.value = "true",
	},
	{
		.key = "ro.config.hspap_hsdpa_open",
		.value = "1",
	},
	{
		.key = "ro.config.huawei_smallwindow",
		.value = "0,0,1080,1920",
	},
	{
		.key = "ro.config.hw.imeisv",
		.value = "true",
	},
	{
		.key = "ro.config.hw.security_volume",
		.value = "8",
	},
	{
		.key = "ro.config.hw_3G_support",
		.value = "true",
	},
	{
		.key = "ro.config.hw_ReduceSAR",
		.value = "true",
	},
	{
		.key = "ro.config.hw_RemindWifiToPdp",
		.value = "true",
	},
	{
		.key = "ro.config.hw_TW_emergencyNum",
		.value = "true",
	},
	{
		.key = "ro.config.hw_allow_rs_mms",
		.value = "true",
	},
	{
		.key = "ro.config.hw_always_allow_mms",
		.value = "0",
	},
	{
		.key = "ro.config.hw_channel_info",
		.value = "0,0,0,0",
	},
	{
		.key = "ro.config.hw_displayeffect_en",
		.value = "true",
	},
	{
		.key = "ro.config.hw_dsda",
		.value = "false",
	},
	{
		.key = "ro.config.hw_dsdspowerup",
		.value = "true",
	},
	{
		.key = "ro.config.hw_dts",
		.value = "true",
	},
	{
		.key = "ro.config.hw_eapsim",
		.value = "true",
	},
	{
		.key = "ro.config.hw_ecc_with_sim_card",
		.value = "true",
	},
	{
		.key = "ro.config.hw_ecclist_nocard",
		.value = "1+110,6+119,8+118",
	},
	{
		.key = "ro.config.hw_ecclist_withcard",
		.value = "1+110,6+119,8+118",
	},
	{
		.key = "ro.config.hw_em_solution_ver",
		.value = "B054",
	},
	{
		.key = "ro.config.hw_enable_merge",
		.value = "true",
	},
	{
		.key = "ro.config.hw_flp_mode",
		.value = "GPS",
	},
	{
		.key = "ro.config.hw_globalEcc",
		.value = "true",
	},
	{
		.key = "ro.config.hw_glovemode_enabled",
		.value = "1",
	},
	{
		.key = "ro.config.hw_gps_power_track",
		.value = "true",
	},
	{
		.key = "ro.config.hw_hideSimIcon",
		.value = "false",
	},
	{
		.key = "ro.config.hw_hidecallforward",
		.value = "false",
	},
	{
		.key = "ro.config.hw_hotswap_on",
		.value = "true",
	},
	{
		.key = "ro.config.hw_hungtasklist",
		.value = "whitelist,system_server,SurfaceFlinger",
	},
	{
		.key = "ro.config.hw_imei_sv_enable",
		.value = "true",
	},
	{
		.key = "ro.config.hw_imei_sv_show_two",
		.value = "true",
	},
	{
		.key = "ro.config.hw_ims_as_normal",
		.value = "true",
	},
	{
		.key = "ro.config.hw_ipv6_support",
		.value = "true",
	},
	{
		.key = "ro.config.hw_lock_emergency",
		.value = "true",
	},
	{
		.key = "ro.config.hw_low_ram",
		.value = "false",
	},
	{
		.key = "ro.config.hw_lte_support",
		.value = "true",
	},
	{
		.key = "ro.config.hw_media_flags",
		.value = "1",
	},
	{
		.key = "ro.config.hw_multiscreen",
		.value = "false",
	},
	{
		.key = "ro.config.hw_music_lp",
		.value = "true",
	},
	{
		.key = "ro.config.hw_navigationbar",
		.value = "true",
	},
	{
		.key = "ro.config.hw_nfc_def_route",
		.value = "0",
	},
	{
		.key = "ro.config.hw_nfc_on",
		.value = "true",
	},
	{
		.key = "ro.config.hw_not_modify_wifi",
		.value = "Singtel WIFI",
	},
	{
		.key = "ro.config.hw_omacp",
		.value = "1",
	},
	{
		.key = "ro.config.hw_opta",
		.value = "636",
	},
	{
		.key = "ro.config.hw_optb",
		.value = "999",
	},
	{
		.key = "ro.config.hw_perfhub",
		.value = "true",
	},
	{
		.key = "ro.config.hw_power_saving",
		.value = "true",
	},
	{
		.key = "ro.config.hw_rcm_cert",
		.value = "true",
	},
	{
		.key = "ro.config.hw_save_pin",
		.value = "true",
	},
	{
		.key = "ro.config.hw_sensorhub",
		.value = "true",
	},
	{
		.key = "ro.config.hw_showSimName",
		.value = "true",
	},
	{
		.key = "ro.config.hw_showTestInfo",
		.value = "false",
	},
	{
		.key = "ro.config.hw_show_4G_Plus_icon",
		.value = "true",
	},
	{
		.key = "ro.config.hw_show_network_icon",
		.value = "true",
	},
	{
		.key = "ro.config.hw_sim2airplane",
		.value = "true",
	},
	{
		.key = "ro.config.hw_simlock_retries",
		.value = "true",
	},
	{
		.key = "ro.config.hw_simpleui_enable",
		.value = "1",
	},
	{
		.key = "ro.config.hw_singlehand",
		.value = "1",
	},
	{
		.key = "ro.config.hw_smartcardservice",
		.value = "true",
	},
	{
		.key = "ro.config.hw_southeast_asia",
		.value = "true",
	},
	{
		.key = "ro.config.hw_srlte",
		.value = "true",
	},
	{
		.key = "ro.config.hw_support_clone_app",
		.value = "true",
	},
	{
		.key = "ro.config.hw_support_geofence",
		.value = "true",
	},
	{
		.key = "ro.config.hw_support_ipcall",
		.value = "false",
	},
	{
		.key = "ro.config.hw_support_vm_ecc",
		.value = "true",
	},
	{
		.key = "ro.config.hw_switchdata_4G",
		.value = "true",
	},
	{
		.key = "ro.config.hw_touchplus_enabled",
		.value = "false",
	},
	{
		.key = "ro.config.hw_useCtrlSocket",
		.value = "true",
	},
	{
		.key = "ro.config.hw_use_browser_ua",
		.value = "http://wap1.huawei.com/uaprof/HUAWEI_%s_UAProfile.xml",
	},
	{
		.key = "ro.config.hw_vcardBase64",
		.value = "true",
	},
	{
		.key = "ro.config.hw_voicemail_sim",
		.value = "true",
	},
	{
		.key = "ro.config.hw_volte_dyn",
		.value = "true",
	},
	{
		.key = "ro.config.hw_volte_on",
		.value = "true",
	},
	{
		.key = "ro.config.hw_watermark",
		.value = "false",
	},
	{
		.key = "ro.config.hw_wifipro_enable",
		.value = "true",
	},
	{
		.key = "ro.config.hwtheme",
		.value = "1",
	},
	{
		.key = "ro.config.ipv4.mtu",
		.value = "1400",
	},
	{
		.key = "ro.config.is_start_commril",
		.value = "true",
	},
	{
		.key = "ro.config.keyguard_unusedata",
		.value = "false",
	},
	{
		.key = "ro.config.keypasstouser",
		.value = "true",
	},
	{
		.key = "ro.config.launcher_matchcard",
		.value = "0",
	},
	{
		.key = "ro.config.linkplus.liveupdate",
		.value = "true",
	},
	{
		.key = "ro.config.linkplus.roaming",
		.value = "true",
	},
	{
		.key = "ro.config.lte_show_northamerica",
		.value = "false",
	},
	{
		.key = "ro.config.marketing_name",
		.value = "HUAWEI Mate 9",
	},
	{
		.key = "ro.config.mm_socket_ctrl",
		.value = "1",
	},
	{
		.key = "ro.config.mmu_en",
		.value = "1",
	},
	{
		.key = "ro.config.modem_number",
		.value = "3",
	},
	{
		.key = "ro.config.nfc_ce_transevt",
		.value = "true",
	},
	{
		.key = "ro.config.not_show_net_connect",
		.value = "true",
	},
	{
		.key = "ro.config.notification_sound",
		.value = "Bongo.ogg",
	},
	{
		.key = "ro.config.pg_camera_cabc",
		.value = "true",
	},
	{
		.key = "ro.config.plmn_to_settings",
		.value = "true",
	},
	{
		.key = "ro.config.reduce_sar_type1",
		.value = "001",
	},
	{
		.key = "ro.config.report_cell_info_list",
		.value = "true",
	},
	{
		.key = "ro.config.ringtone",
		.value = "Huawei_Tune_Living.ogg",
	},
	{
		.key = "ro.config.ringtone2",
		.value = "Huawei_Tune_Clean.ogg",
	},
	{
		.key = "ro.config.safety_certification",
		.value = "IC,FCC,CE",
	},
	{
		.key = "ro.config.scr_timeout_10sec",
		.value = "true",
	},
	{
		.key = "ro.config.show_epg_menu",
		.value = "false",
	},
	{
		.key = "ro.config.show_full_month",
		.value = "true",
	},
	{
		.key = "ro.config.show_nw_mode",
		.value = "true",
	},
	{
		.key = "ro.config.show_vmail_number",
		.value = "true",
	},
	{
		.key = "ro.config.signalplus.tas",
		.value = "false",
	},
	{
		.key = "ro.config.signalplus.xpass",
		.value = "false",
	},
	{
		.key = "ro.config.small.previewpos",
		.value = "right",
	},
	{
		.key = "ro.config.spare_ntp_server",
		.value = "ntp.sjtu.edu.cn,time.windows.com,time.nist.gov,1.cn.pool.ntp.org",
	},
	{
		.key = "ro.config.support_ca",
		.value = "true",
	},
	{
		.key = "ro.config.support_internetCall",
		.value = "false",
	},
	{
		.key = "ro.config.support_one_time_hota",
		.value = "true",
	},
	{
		.key = "ro.config.support_wcdma_modem1",
		.value = "true",
	},
	{
		.key = "ro.config.switchPrimaryVolume",
		.value = "true",
	},
	{
		.key = "ro.config.theme_display_tw",
		.value = "true",
	},
	{
		.key = "ro.config.tw_dialpad_show",
		.value = "true",
	},
	{
		.key = "ro.config.updatelocation",
		.value = "true",
	},
	{
		.key = "ro.config.utkShowIcon",
		.value = "true",
	},
	{
		.key = "ro.config.vm_prioritymode",
		.value = "2",
	},
	{
		.key = "ro.config.widevine_level3",
		.value = "true",
	},
	{
		.key = "ro.config.wifi_country_code",
		.value = "true",
	},
	{
		.key = "ro.config.wifi_reduce_power",
		.value = "true,16,12,12",
	},
	{
		.key = "ro.connectivity.chiptype",
		.value = "bcm43xx",
	},
	{
		.key = "ro.connectivity.sub_chiptype",
		.value = "bcm43455",
	},
	{
		.key = "ro.crypto.fuse_sdcard",
		.value = "true",
	},
	{
		.key = "ro.crypto.state",
		.value = "encrypted",
	},
	{
		.key = "ro.crypto.type",
		.value = "file",
	},
	{
		.key = "ro.cust.cdrom",
		.value = "/data/hw_init/version/region_comm/oversea/cdrom/autorun.iso",
	},
	{
		.key = "ro.dalvik.vm.native.bridge",
		.value = "0",
	},
	{
		.key = "ro.debuggable",
		.value = "0",
	},
	{
		.key = "ro.dropbox.partnerpath",
		.value = "data/cust/xml/huawei_dropboxpartner.txt",
	},
	{
		.key = "ro.dual.sim.phone",
		.value = "true",
	},
	{
		.key = "ro.email.inline_as_att",
		.value = "true",
	},
	{
		.key = "ro.enable_boot_charger_mode",
		.value = "0",
	},
	{
		.key = "ro.frp.pst",
		.value = "/dev/block/bootdevice/by-name/frp",
	},
	{
		.key = "ro.hardware",
		.value = "hi3660",
	},
	{
		.key = "ro.hdmi.service",
		.value = "false",
	},
	{
		.key = "ro.huawei.cust.oma",
		.value = "false",
	},
	{
		.key = "ro.huawei.cust.oma_drm",
		.value = "false",
	},
	{
		.key = "ro.hw.country",
		.value = "spcseas",
	},
	{
		.key = "ro.hw.custPath",
		.value = "/cust/hw/spcseas",
	},
	{
		.key = "ro.hw.oemName",
		.value = "MHA-L29",
	},
	{
		.key = "ro.hw.specialCustPath",
		.value = "/cust_spec",
	},
	{
		.key = "ro.hw.vendor",
		.value = "hw",
	},
	{
		.key = "ro.hwui.drop_shadow_cache_size",
		.value = "6",
	},
	{
		.key = "ro.hwui.gradient_cache_size",
		.value = "1",
	},
	{
		.key = "ro.hwui.layer_cache_size",
		.value = "48",
	},
	{
		.key = "ro.hwui.path_cache_size",
		.value = "32",
	},
	{
		.key = "ro.hwui.r_buffer_cache_size",
		.value = "8",
	},
	{
		.key = "ro.hwui.shape_cache_size",
		.value = "2",
	},
	{
		.key = "ro.hwui.text_large_cache_height",
		.value = "1024",
	},
	{
		.key = "ro.hwui.text_large_cache_width",
		.value = "2048",
	},
	{
		.key = "ro.hwui.text_small_cache_height",
		.value = "1024",
	},
	{
		.key = "ro.hwui.text_small_cache_width",
		.value = "1024",
	},
	{
		.key = "ro.hwui.texture_cache_flushrate",
		.value = "0.4",
	},
	{
		.key = "ro.hwui.texture_cache_size",
		.value = "72",
	},
	{
		.key = "ro.image",
		.value = "bootimage",
	},
	{
		.key = "ro.logsystem.usertype",
		.value = "6",
	},
	{
		.key = "ro.magic.api.version",
		.value = "0.1",
	},
	{
		.key = "ro.multi.rild",
		.value = "false",
	},
	{
		.key = "ro.opengles.version",
		.value = "196610",
	},
	{
		.key = "ro.product.CustCVersion",
		.value = "C636",
	},
	{
		.key = "ro.product.CustDVersion",
		.value = "D112",
	},
	{
		.key = "ro.product.board",
		.value = "MHA",
	},
	{
		.key = "ro.product.brand",
		.value = "HUAWEI",
	},
	{
		.key = "ro.product.cpu.abi",
		.value = "arm64-v8a",
	},
	{
		.key = "ro.product.cpu.abilist",
		.value = "arm64-v8a,armeabi-v7a,armeabi",
	},
	{
		.key = "ro.product.cpu.abilist32",
		.value = "armeabi-v7a,armeabi",
	},
	{
		.key = "ro.product.cpu.abilist64",
		.value = "arm64-v8a",
	},
	{
		.key = "ro.product.cucc.rtsplive",
		.value = "true",
	},
	{
		.key = "ro.product.device",
		.value = "HWMHA",
	},
	{
		.key = "ro.product.fingerprintName",
		.value = "PANDA-1",
	},
	{
		.key = "ro.product.hardwareversion",
		.value = "HLIAMHAM",
	},
	{
		.key = "ro.product.locale",
		.value = "en-GB",
	},
	{
		.key = "ro.product.locale.language",
		.value = "en",
	},
	{
		.key = "ro.product.locale.region",
		.value = "US",
	},
	{
		.key = "ro.product.manufacturer",
		.value = "HUAWEI",
	},
	{
		.key = "ro.product.model",
		.value = "MHA-L29",
	},
	{
		.key = "ro.product.name",
		.value = "MHA-L29",
	},
	{
		.key = "ro.product.platform",
		.value = "hi3660",
	},
	{
		.key = "ro.product.platform.pseudonym",
		.value = "1ARB9CV",
	},
	{
		.key = "ro.pwroff_card_if_tray_out",
		.value = "true",
	},
	{
		.key = "ro.revision",
		.value = "0",
	},
	{
		.key = "ro.ril.ecclist",
		.value = "112,911,#911,*911",
	},
	{
		.key = "ro.runmode",
		.value = "normal",
	},
	{
		.key = "ro.runtime.firstboot",
		.value = "1524186581019",
	},
	{
		.key = "ro.secure",
		.value = "1",
	},
	{
		.key = "ro.serialno",
		.value = "AHK0216C29000389",
	},
	{
		.key = "ro.setupwizard.mode",
		.value = "OPTIONAL",
	},
	{
		.key = "ro.setupwizard.require_network",
		.value = "any",
	},
	{
		.key = "ro.sf.lcd_density",
		.value = "480",
	},
	{
		.key = "ro.sys.fw.bg_apps_limit",
		.value = "36",
	},
	{
		.key = "ro.sys.fw.empty_app_percent",
		.value = "67",
	},
	{
		.key = "ro.sys.fw.trim_cache_percent",
		.value = "100",
	},
	{
		.key = "ro.sys.fw.trim_empty_percent",
		.value = "100",
	},
	{
		.key = "ro.sys.fw.trim_enable_memory",
		.value = "6442450944",
	},
	{
		.key = "ro.sys.fw.use_trim_settings",
		.value = "true",
	},
	{
		.key = "ro.sys.powerup_reason",
		.value = "NORMAL",
	},
	{
		.key = "ro.sys.umsdirtyratio",
		.value = "2",
	},
	{
		.key = "ro.sys.wp",
		.value = "false",
	},
	{
		.key = "ro.sysui.signal.layout",
		.value = "",
	},
	{
		.key = "ro.telephony.default_network",
		.value = "9",
	},
	{
		.key = "ro.tui.service",
		.value = "true",
	},
	{
		.key = "ro.vendor.build.date",
		.value = "Thu Nov 3 12:34:08 CST 2016",
	},
	{
		.key = "ro.vendor.build.date.utc",
		.value = "1478147648",
	},
	{
		.key = "ro.vowifi.callinwifi",
		.value = "100,6",
	},
	{
		.key = "ro.vowifi.hw_vowifi",
		.value = "true",
	},
	{
		.key = "ro.vr_display.service",
		.value = "true",
	},
	{
		.key = "ro.wifi.channels",
		.value = "",
	},
	{
		.key = "ro.zygote",
		.value = "zygote64_32",
	},
	{
		.key = "security.perf_harden",
		.value = "1",
	},
	{
		.key = "selinux.reload_policy",
		.value = "1",
	},
	{
		.key = "service.bootanim.exit",
		.value = "1",
	},
	{
		.key = "sys.2dsdr.pkgname",
		.value = "*",
	},
	{
		.key = "sys.2dsdr.startratio",
		.value = "0.75",
	},
	{
		.key = "sys.aps.gameProcessName",
		.value = "",
	},
	{
		.key = "sys.aps.support",
		.value = "39163",
	},
	{
		.key = "sys.aps.support.tmp",
		.value = "39163",
	},
	{
		.key = "sys.aps.version",
		.value = "3.3.10-3.2.10",
	},
	{
		.key = "sys.boot_completed",
		.value = "1",
	},
	{
		.key = "sys.defaultapn.enabled",
		.value = "false",
	},
	{
		.key = "sys.fingerprint.deviceId",
		.value = "0",
	},
	{
		.key = "sys.hisi.pmom.service.enable",
		.value = "false",
	},
	{
		.key = "sys.hw_boot_success",
		.value = "1",
	},
	{
		.key = "sys.hwsholder.count",
		.value = "0",
	},
	{
		.key = "sys.iswifihotspoton",
		.value = "false",
	},
	{
		.key = "sys.oem_unlock_allowed",
		.value = "0",
	},
	{
		.key = "sys.refresh.dirty",
		.value = "1",
	},
	{
		.key = "sys.resettype",
		.value = "normal:press1s",
	},
	{
		.key = "sys.show_google_nlp",
		.value = "true",
	},
	{
		.key = "sys.super_power_save",
		.value = "false",
	},
	{
		.key = "sys.sysctl.extra_free_kbytes",
		.value = "24300",
	},
	{
		.key = "sys.sysctl.tcp_def_init_rwnd",
		.value = "60",
	},
	{
		.key = "sys.usb.config",
		.value = "hisuite,mtp,mass_storage,adb",
	},
	{
		.key = "sys.usb.configfs",
		.value = "1",
	},
	{
		.key = "sys.usb.controller",
		.value = "ff100000.dwc3",
	},
	{
		.key = "sys.usb.ffs.ready",
		.value = "1",
	},
	{
		.key = "sys.usb.ffs_hdb.ready",
		.value = "0",
	},
	{
		.key = "sys.usb.state",
		.value = "hisuite,mtp,mass_storage,adb",
	},
	{
		.key = "sys.userdata_is_ready",
		.value = "1",
	},
	{
		.key = "system_init.hwextdeviceservice",
		.value = "1",
	},
	{
		.key = "trustedcore_sfs_property",
		.value = "1",
	},
	{
		.key = "use_sensorhub_labc",
		.value = "false",
	},
	{
		.key = "viatel.device.at",
		.value = "spi.10.ttySPI",
	},
	{
		.key = "viatel.device.data",
		.value = "spi.0.ttySPI",
	},
	{
		.key = "viatel.device.fls",
		.value = "spi.2.ttySPI",
	},
	{
		.key = "viatel.device.gps",
		.value = "spi.5.ttySPI",
	},
	{
		.key = "viatel.device.pcv",
		.value = "spi.4.ttySPI",
	},
	{
		.key = "vold.crypto_unencrypt_updatedir",
		.value = "/data/update",
	},
	{
		.key = "vold.has_adoptable",
		.value = "0",
	},
	{
		.key = "vold.post_fs_data_done",
		.value = "1",
	},
	{
		.key = "wifi.interface",
		.value = "wlan0",
	},
	{
		.key = "wlan.driver.status",
		.value = "ok",
	},
	{NULL},
};
#endif /* __ANDROID__ */
