struct cpuinfo_mock_file filesystem[] = {
#if CPUINFO_ARCH_ARM64
	{
		.path = "/proc/cpuinfo",
		.size = 1448,
		.content = "processor\t: 0\n"
			   "BogoMIPS\t: 52.00\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd03\n"
			   "CPU revision\t: 4\n"
			   "\n"
			   "processor\t: 1\n"
			   "BogoMIPS\t: 52.00\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd03\n"
			   "CPU revision\t: 4\n"
			   "\n"
			   "processor\t: 2\n"
			   "BogoMIPS\t: 52.00\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd03\n"
			   "CPU revision\t: 4\n"
			   "\n"
			   "processor\t: 3\n"
			   "BogoMIPS\t: 52.00\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd03\n"
			   "CPU revision\t: 4\n"
			   "\n"
			   "processor\t: 4\n"
			   "BogoMIPS\t: 52.00\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x53\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x4\n"
			   "CPU part\t: 0x001\n"
			   "CPU revision\t: 0\n"
			   "\n"
			   "processor\t: 5\n"
			   "BogoMIPS\t: 52.00\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x53\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x4\n"
			   "CPU part\t: 0x001\n"
			   "CPU revision\t: 0\n"
			   "\n"
			   "processor\t: 6\n"
			   "BogoMIPS\t: 52.00\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x53\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x4\n"
			   "CPU part\t: 0x001\n"
			   "CPU revision\t: 0\n"
			   "\n"
			   "processor\t: 7\n"
			   "BogoMIPS\t: 52.00\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x53\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x4\n"
			   "CPU part\t: 0x001\n"
			   "CPU revision\t: 0\n"
			   "\n",
	},
#elif CPUINFO_ARCH_ARM
	{
		.path = "/proc/cpuinfo",
		.size = 1912,
		.content =
			"processor\t: 0\n"
			"BogoMIPS\t: 52.00\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd03\n"
			"CPU revision\t: 4\n"
			"\n"
			"processor\t: 1\n"
			"BogoMIPS\t: 52.00\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd03\n"
			"CPU revision\t: 4\n"
			"\n"
			"processor\t: 2\n"
			"BogoMIPS\t: 52.00\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd03\n"
			"CPU revision\t: 4\n"
			"\n"
			"processor\t: 3\n"
			"BogoMIPS\t: 52.00\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd03\n"
			"CPU revision\t: 4\n"
			"\n"
			"processor\t: 4\n"
			"BogoMIPS\t: 52.00\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x53\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x4\n"
			"CPU part\t: 0x001\n"
			"CPU revision\t: 0\n"
			"\n"
			"processor\t: 5\n"
			"BogoMIPS\t: 52.00\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x53\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x4\n"
			"CPU part\t: 0x001\n"
			"CPU revision\t: 0\n"
			"\n"
			"processor\t: 6\n"
			"BogoMIPS\t: 52.00\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x53\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x4\n"
			"CPU part\t: 0x001\n"
			"CPU revision\t: 0\n"
			"\n"
			"processor\t: 7\n"
			"BogoMIPS\t: 52.00\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x53\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x4\n"
			"CPU part\t: 0x001\n"
			"CPU revision\t: 0\n"
			"\n",
	},
#endif
	{
		.path = "/system/build.prop",
		.size = 5179,
		.content =
			"\n"
			"# begin build properties\n"
			"# autogenerated by buildinfo.sh\n"
			"ro.build.id=NRD90M\n"
			"ro.build.display.id=NRD90M.G950FXXU1AQL5\n"
			"ro.build.version.incremental=G950FXXU1AQL5\n"
			"ro.build.version.sdk=24\n"
			"ro.build.version.preview_sdk=0\n"
			"ro.build.version.codename=REL\n"
			"ro.build.version.all_codenames=REL\n"
			"ro.build.version.release=7.0\n"
			"ro.build.version.security_patch=2017-12-01\n"
			"ro.build.version.base_os=\n"
			"ro.build.version.security_index=1\n"
			"ro.build.date=Wed Dec 13 20:58:07 KST 2017\n"
			"ro.build.date.utc=1513166287\n"
			"ro.build.type=user\n"
			"ro.build.user=dpi\n"
			"ro.build.host=SWDG4522\n"
			"ro.build.tags=release-keys\n"
			"ro.build.flavor=dreamltexx-user\n"
			"ro.product.model=SM-G950F\n"
			"ro.product.brand=samsung\n"
			"ro.product.name=dreamltexx\n"
			"ro.product.device=dreamlte\n"
			"ro.product.board=universal8895\n"
			"# ro.product.cpu.abi and ro.product.cpu.abi2 are obsolete,\n"
			"# use ro.product.cpu.abilist instead.\n"
			"ro.product.cpu.abi=arm64-v8a\n"
			"ro.product.cpu.abilist=arm64-v8a,armeabi-v7a,armeabi\n"
			"ro.product.cpu.abilist32=armeabi-v7a,armeabi\n"
			"ro.product.cpu.abilist64=arm64-v8a\n"
			"ro.product.manufacturer=samsung\n"
			"ro.product.locale=en-GB\n"
			"ro.wifi.channels=\n"
			"ro.board.platform=exynos5\n"
			"# ro.build.product is obsolete; use ro.product.device\n"
			"ro.build.product=dreamlte\n"
			"# Do not try to parse description, fingerprint, or thumbprint\n"
			"ro.build.description=dreamltexx-user 7.0 NRD90M G950FXXU1AQL5 release-keys\n"
			"ro.build.fingerprint=samsung/dreamltexx/dreamlte:7.0/NRD90M/G950FXXU1AQL5:user/release-keys\n"
			"ro.build.characteristics=phone,emulator\n"
			"# Samsung Specific Properties\n"
			"ro.build.PDA=G950FXXU1AQL5\n"
			"ro.build.official.release=true\n"
			"ro.config.rm_preload_enabled=1\n"
			"ro.build.changelist=12640043\n"
			"ro.product_ship=true\n"
			"ro.chipname=exynos8895\n"
			"# end build properties\n"
			"\n"
			"#\n"
			"# HWUI_BUILD_PROPERTIES\n"
			"#\n"
			"ro.hwui.texture_cache_size=88\n"
			"ro.hwui.layer_cache_size=58\n"
			"ro.hwui.path_cache_size=16\n"
			"ro.hwui.texture_cache_flushrate=0.4\n"
			"ro.hwui.shape_cache_size=4\n"
			"ro.hwui.gradient_cache_size=2\n"
			"ro.hwui.drop_shadow_cache_size=6\n"
			"ro.hwui.r_buffer_cache_size=8\n"
			"ro.hwui.text_small_cache_width=1024\n"
			"ro.hwui.text_small_cache_height=1024\n"
			"ro.hwui.text_large_cache_width=4096\n"
			"ro.hwui.text_large_cache_height=2048\n"
			"#\n"
			"# from device/samsung/dreamlte/system.prop\n"
			"#\n"
			"#\n"
			"# system.prop for universal8895\n"
			"#\n"
			"\n"
			"ro.sf.lcd_density=480\n"
			"ro.sf.init.lcd_density=640\n"
			"\n"
			"ro.arch=exynos8895\n"
			"ro.kernel.qemu=0\n"
			"ro.kernel.qemu.gles=0\n"
			"persist.demo.hdmirotationlock=false\n"
			"\n"
			"# read DS/SS property\n"
			"import /efs/factory.prop\n"
			"\n"
			"#\n"
			"# ADDITIONAL_BUILD_PROPERTIES\n"
			"#\n"
			"ro.astcenc.astcsupport=1\n"
			"ro.mct.compressiontype=ETC1\n"
			"persist.radio.sib16_support=1\n"
			"ro.config.dmverity=true\n"
			"ro.config.rkp=true\n"
			"ro.config.kap_default_on=true\n"
			"ro.config.kap=true\n"
			"ro.supportmodel.mptcp=1\n"
			"rild.libpath=/system/lib64/libsec-ril.so\n"
			"rild.libpath2=/system/lib64/libsec-ril-dsds.so\n"
			"ro.radio.noril=no\n"
			"ro.telephony.default_network=9\n"
			"ro.multisim.simslotcount=2\n"
			"ro.knox.enhance.zygote.aslr=0\n"
			"ro.product.first_api_level=24\n"
			"ro.config.ringtone=Over_the_Horizon.ogg\n"
			"ro.config.notification_sound=Skyline.ogg\n"
			"ro.config.alarm_alert=Morning_Glory.ogg\n"
			"ro.config.media_sound=Media_preview_Touch_the_light.ogg\n"
			"ro.config.ringtone_2=Basic_Bell.ogg\n"
			"ro.config.notification_sound_2=S_Charming_Bell.ogg\n"
			"ro.config.systemaudiodebug=abox\n"
			"dalvik.vm.heapstartsize=8m\n"
			"dalvik.vm.heapgrowthlimit=256m\n"
			"dalvik.vm.heapsize=512m\n"
			"dalvik.vm.heaptargetutilization=0.75\n"
			"dalvik.vm.heapminfree=2m\n"
			"dalvik.vm.heapmaxfree=8m\n"
			"ro.opengles.version=196610\n"
			"ro.sf.lcd_density=480\n"
			"debug.slsi_platform=1\n"
			"debug.hwc.winupdate=1\n"
			"drm.service.enabled=true\n"
			"ro.hdcp2.rx=tz\n"
			"ro.securestorage.support=true\n"
			"af.fast_period_size=192\n"
			"af.fast_track_multiplier=1\n"
			"ro.error.receiver.default=com.samsung.receiver.error\n"
			"ro.build.scafe.version=2017A\n"
			"ro.frp.pst=/dev/block/persistent\n"
			"security.mdpp.mass=skmm\n"
			"ro.security.vpnpp.ver=1.4\n"
			"ro.security.vpnpp.release=8.1\n"
			"security.mdpp=None\n"
			"ro.security.mdpp.ver=3.0\n"
			"ro.security.mdpp.release=2\n"
			"ro.security.wlan.ver=1.0\n"
			"ro.security.wlan.release=2\n"
			"security.mdpp.result=None\n"
			"ro.hardware.keystore=mdfpp\n"
			"ro.security.mdpp.ux=Enabled\n"
			"sys.config.amp_perf_enable=true\n"
			"ro.config.dha_cached_min=6\n"
			"ro.config.dha_cached_max=16\n"
			"ro.config.dha_empty_min=8\n"
			"ro.config.dha_empty_init=32\n"
			"ro.config.dha_empty_max=32\n"
			"ro.config.dha_th_rate=2.0\n"
			"ro.config.dha_pwhitelist_enable=1\n"
			"ro.config.dha_pwhl_key=7938\n"
			"ro.config.fall_prevent_enable=true\n"
			"ro.config.infinite_bg_enable=true\n"
			"sys.use_fifo_ui=0\n"
			"ro.wsmd.enable=true\n"
			"ro.security.vaultkeeper.feature=1\n"
			"keyguard.no_require_sim=true\n"
			"ro.carrier=unknown\n"
			"ro.security.icd.flagmode=multi\n"
			"security.ASKS.policy_version=000000\n"
			"ro.com.google.clientidbase=android-samsung\n"
			"ro.ril.hsxpa=1\n"
			"ro.ril.gprsclass=10\n"
			"ro.adb.qemud=1\n"
			"ro.gfx.driver.0=com.samsung.gpudriver.S8MaliG71_70\n"
			"ro.hardware.egl=mali\n"
			"ro.opa.eligible_device=true\n"
			"ro.setupwizard.mode=OPTIONAL\n"
			"ro.com.google.gmsversion=7.0_r11\n"
			"ro.build.selinux=1\n"
			"persist.sys.dalvik.vm.lib.2=libart.so\n"
			"dalvik.vm.isa.arm64.variant=exynos-m2\n"
			"dalvik.vm.isa.arm64.features=default\n"
			"dalvik.vm.isa.arm.variant=cortex-a53\n"
			"dalvik.vm.isa.arm.features=default\n"
			"ro.config.knox=v30\n"
			"ro.config.tima=1\n"
			"ro.config.timaversion=3.0\n"
			"ro.config.iccc_version=1.0\n"
			"ro.kernel.qemu=0\n"
			"net.bt.name=Android\n"
			"dalvik.vm.stack-trace-file=/data/anr/traces.txt\n"
			"ro.build.version.sem=2403\n"
			"ro.build.version.sep=80100\n"
			"ro.expect.recovery_id=0x48c53cca3ea48ceba1bcfabccc7259c10b06c5d2000000000000000000000000\n"
			"\n",
	},
	{
		.path = "/sys/devices/soc0/family",
		.size = 15,
		.content = "Samsung Exynos\n",
	},
	{
		.path = "/sys/devices/soc0/machine",
		.size = 53,
		.content = "Samsung DREAMLTE EUR rev06 board based on EXYNOS8895\n",
	},
	{
		.path = "/sys/devices/soc0/revision",
		.size = 3,
		.content = "17\n",
	},
	{
		.path = "/sys/devices/soc0/soc_id",
		.size = 11,
		.content = "EXYNOS8895\n",
	},
	{
		.path = "/sys/devices/system/cpu/isolated",
		.size = 1,
		.content = "\n",
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
		.path = "/sys/devices/system/cpu/cpufreq/current_in_state",
		.size = 1276,
		.content =
			"CPU0:1690000=60240 1456000=40410 1248000=27260 1053000=20180 949000=16850 832000=13710 715000=10710 598000=8290 455000=6060 \n"
			"CPU1:1690000=60240 1456000=40410 1248000=27260 1053000=20180 949000=16850 832000=13710 715000=10710 598000=8290 455000=6060 \n"
			"CPU2:1690000=60240 1456000=40410 1248000=27260 1053000=20180 949000=16850 832000=13710 715000=10710 598000=8290 455000=6060 \n"
			"CPU3:1690000=60240 1456000=40410 1248000=27260 1053000=20180 949000=16850 832000=13710 715000=10710 598000=8290 455000=6060 \n"
			"CPU4:2314000=355850 2158000=295970 2002000=251060 1937000=233620 1807000=201350 1703000=177320 1469000=137850 1261000=111850 1170000=100000 1066000=88090 962000=78470 858000=69380 741000=58880 \n"
			"CPU5:2314000=355850 2158000=295970 2002000=251060 1937000=233620 1807000=201350 1703000=177320 1469000=137850 1261000=111850 1170000=100000 1066000=88090 962000=78470 858000=69380 741000=58880 \n"
			"CPU6:2314000=355850 2158000=295970 2002000=251060 1937000=233620 1807000=201350 1703000=177320 1469000=137850 1261000=111850 1170000=100000 1066000=88090 962000=78470 858000=69380 741000=58880 \n"
			"CPU7:2314000=355850 2158000=295970 2002000=251060 1937000=233620 1807000=201350 1703000=177320 1469000=137850 1261000=111850 1170000=100000 1066000=88090 962000=78470 858000=69380 741000=58880 \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpuidle/current_driver",
		.size = 12,
		.content = "exynos_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpuidle/current_governor_ro",
		.size = 5,
		.content = "menu\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpuidle/driver/name",
		.size = 12,
		.content = "exynos_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/affected_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1690000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "455000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/related_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors",
		.size = 23,
		.content = "userspace interactive \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1456000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_driver",
		.size = 15,
		.content = "exynos_cpufreq\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "455000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/stats/time_in_state",
		.size = 101,
		.content = "1690000 6834\n"
			   "1456000 427\n"
			   "1248000 277\n"
			   "1053000 102\n"
			   "949000 117\n"
			   "832000 75\n"
			   "715000 64\n"
			   "598000 83\n"
			   "455000 257\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/stats/total_trans",
		.size = 4,
		.content = "216\n",
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
		.content = "1\n",
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
		.size = 12,
		.content = "exynos_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/affected_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1690000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "455000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/related_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_available_governors",
		.size = 23,
		.content = "userspace interactive \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "455000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_driver",
		.size = 15,
		.content = "exynos_cpufreq\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "455000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/stats/time_in_state",
		.size = 101,
		.content = "1690000 6861\n"
			   "1456000 471\n"
			   "1248000 298\n"
			   "1053000 105\n"
			   "949000 141\n"
			   "832000 75\n"
			   "715000 69\n"
			   "598000 91\n"
			   "455000 391\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/stats/total_trans",
		.size = 4,
		.content = "235\n",
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
		.content = "1\n",
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
		.size = 12,
		.content = "exynos_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/affected_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1690000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "455000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/related_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_available_governors",
		.size = 23,
		.content = "userspace interactive \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "455000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_driver",
		.size = 15,
		.content = "exynos_cpufreq\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "455000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/stats/time_in_state",
		.size = 101,
		.content = "1690000 6861\n"
			   "1456000 471\n"
			   "1248000 298\n"
			   "1053000 105\n"
			   "949000 150\n"
			   "832000 75\n"
			   "715000 71\n"
			   "598000 95\n"
			   "455000 647\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/stats/total_trans",
		.size = 4,
		.content = "240\n",
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
		.content = "1\n",
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
		.size = 12,
		.content = "exynos_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/affected_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1690000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "455000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/related_cpus",
		.size = 8,
		.content = "0 1 2 3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_available_governors",
		.size = 23,
		.content = "userspace interactive \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1690000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_driver",
		.size = 15,
		.content = "exynos_cpufreq\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "455000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/stats/time_in_state",
		.size = 102,
		.content = "1690000 6951\n"
			   "1456000 538\n"
			   "1248000 301\n"
			   "1053000 105\n"
			   "949000 154\n"
			   "832000 78\n"
			   "715000 108\n"
			   "598000 95\n"
			   "455000 666\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/stats/total_trans",
		.size = 4,
		.content = "251\n",
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
		.content = "1\n",
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
		.size = 12,
		.content = "exynos_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/affected_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "2314000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "741000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/related_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_available_governors",
		.size = 23,
		.content = "userspace interactive \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1261000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_driver",
		.size = 15,
		.content = "exynos_cpufreq\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq",
		.size = 8,
		.content = "2314000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "741000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/stats/time_in_state",
		.size = 147,
		.content = "2314000 5088\n"
			   "2158000 62\n"
			   "2002000 22\n"
			   "1937000 62\n"
			   "1807000 120\n"
			   "1703000 78\n"
			   "1469000 143\n"
			   "1261000 691\n"
			   "1170000 28\n"
			   "1066000 57\n"
			   "962000 28\n"
			   "858000 77\n"
			   "741000 2792\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/stats/total_trans",
		.size = 4,
		.content = "347\n",
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
		.content = "0\n",
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
		.size = 12,
		.content = "exynos_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/affected_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "2314000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "741000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/related_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_available_governors",
		.size = 23,
		.content = "userspace interactive \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1261000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_driver",
		.size = 15,
		.content = "exynos_cpufreq\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_max_freq",
		.size = 8,
		.content = "2314000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "741000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/stats/time_in_state",
		.size = 148,
		.content = "2314000 5088\n"
			   "2158000 68\n"
			   "2002000 22\n"
			   "1937000 64\n"
			   "1807000 127\n"
			   "1703000 100\n"
			   "1469000 165\n"
			   "1261000 750\n"
			   "1170000 28\n"
			   "1066000 57\n"
			   "962000 28\n"
			   "858000 85\n"
			   "741000 2932\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/stats/total_trans",
		.size = 4,
		.content = "375\n",
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
		.content = "0\n",
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
		.size = 12,
		.content = "exynos_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/affected_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "2314000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "741000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/related_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_available_governors",
		.size = 23,
		.content = "userspace interactive \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "741000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_driver",
		.size = 15,
		.content = "exynos_cpufreq\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_max_freq",
		.size = 8,
		.content = "2314000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "741000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/stats/time_in_state",
		.size = 148,
		.content = "2314000 5140\n"
			   "2158000 73\n"
			   "2002000 22\n"
			   "1937000 68\n"
			   "1807000 128\n"
			   "1703000 108\n"
			   "1469000 193\n"
			   "1261000 824\n"
			   "1170000 28\n"
			   "1066000 68\n"
			   "962000 28\n"
			   "858000 92\n"
			   "741000 3016\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/stats/total_trans",
		.size = 4,
		.content = "413\n",
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
		.content = "0\n",
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
		.size = 12,
		.content = "exynos_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/affected_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "2314000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "741000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/related_cpus",
		.size = 8,
		.content = "4 5 6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_available_governors",
		.size = 23,
		.content = "userspace interactive \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "741000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_driver",
		.size = 15,
		.content = "exynos_cpufreq\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_governor",
		.size = 12,
		.content = "interactive\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_max_freq",
		.size = 8,
		.content = "2314000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "741000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/stats/time_in_state",
		.size = 149,
		.content = "2314000 5140\n"
			   "2158000 73\n"
			   "2002000 22\n"
			   "1937000 68\n"
			   "1807000 128\n"
			   "1703000 122\n"
			   "1469000 220\n"
			   "1261000 904\n"
			   "1170000 28\n"
			   "1066000 70\n"
			   "962000 36\n"
			   "858000 108\n"
			   "741000 3112\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/stats/total_trans",
		.size = 4,
		.content = "446\n",
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
		.content = "0\n",
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
		.key = "af.fast_period_size",
		.value = "192",
	},
	{
		.key = "af.fast_track_multiplier",
		.value = "1",
	},
	{
		.key = "audioflinger.bootsnd",
		.value = "0",
	},
	{
		.key = "boot.sfbootcomplete",
		.value = "0",
	},
	{
		.key = "dalvik.vm.appimageformat",
		.value = "lz4",
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
		.key = "dalvik.vm.heapgrowthlimit",
		.value = "256m",
	},
	{
		.key = "dalvik.vm.heapmaxfree",
		.value = "8m",
	},
	{
		.key = "dalvik.vm.heapminfree",
		.value = "2m",
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
		.key = "dalvik.vm.isa.arm.features",
		.value = "default",
	},
	{
		.key = "dalvik.vm.isa.arm.variant",
		.value = "cortex-a53",
	},
	{
		.key = "dalvik.vm.isa.arm64.features",
		.value = "default",
	},
	{
		.key = "dalvik.vm.isa.arm64.variant",
		.value = "exynos-m2",
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
		.key = "debug.atrace.tags.enableflags",
		.value = "0",
	},
	{
		.key = "debug.force_rtl",
		.value = "0",
	},
	{
		.key = "debug.hwc.winupdate",
		.value = "1",
	},
	{
		.key = "debug.sf.layerdump",
		.value = "0",
	},
	{
		.key = "debug.slsi_platform",
		.value = "1",
	},
	{
		.key = "dev.bootcomplete",
		.value = "1",
	},
	{
		.key = "dev.kies.drivedisplay",
		.value = "0",
	},
	{
		.key = "dev.kies.drivedisplay.trust",
		.value = "1",
	},
	{
		.key = "dev.kies.sommode",
		.value = "TRUE",
	},
	{
		.key = "dev.kiessupport",
		.value = "TRUE",
	},
	{
		.key = "dev.knoxapp.running",
		.value = "false",
	},
	{
		.key = "dev.mtp.opensession",
		.value = "0",
	},
	{
		.key = "dev.ssrm.appsync3p",
		.value = "true",
	},
	{
		.key = "dev.ssrm.dex2oat.affinity",
		.value = "",
	},
	{
		.key = "dev.ssrm.gamelevel",
		.value = "-4,6,-3,-3",
	},
	{
		.key = "dev.ssrm.init",
		.value = "1",
	},
	{
		.key = "dev.ssrm.mode",
		.value = "",
	},
	{
		.key = "dev.ssrm.pst",
		.value = "376",
	},
	{
		.key = "dev.ssrm.smart_switch",
		.value = "true",
	},
	{
		.key = "drm.service.enabled",
		.value = "true",
	},
	{
		.key = "gsm.current.phone-type",
		.value = "1",
	},
	{
		.key = "gsm.network.type",
		.value = "Unknown",
	},
	{
		.key = "gsm.operator.alpha",
		.value = "",
	},
	{
		.key = "gsm.operator.iso-country",
		.value = "us",
	},
	{
		.key = "gsm.operator.ispsroaming",
		.value = "false",
	},
	{
		.key = "gsm.operator.isroaming",
		.value = "false",
	},
	{
		.key = "gsm.operator.numeric",
		.value = "310260",
	},
	{
		.key = "gsm.sim.operator.alpha",
		.value = "",
	},
	{
		.key = "gsm.sim.operator.iso-country",
		.value = "",
	},
	{
		.key = "gsm.sim.operator.numeric",
		.value = "",
	},
	{
		.key = "gsm.sim.state",
		.value = "ABSENT",
	},
	{
		.key = "gsm.version.baseband",
		.value = "G950FXXU1AQL5",
	},
	{
		.key = "gsm.version.ril-impl",
		.value = "Samsung RIL v3.0",
	},
	{
		.key = "init.svc.BCS-daemon",
		.value = "running",
	},
	{
		.key = "init.svc.DIAG-daemon",
		.value = "running",
	},
	{
		.key = "init.svc.DR-daemon",
		.value = "running",
	},
	{
		.key = "init.svc.SIDESYNC_service",
		.value = "running",
	},
	{
		.key = "init.svc.SMD-daemon",
		.value = "running",
	},
	{
		.key = "init.svc.TvoutService_C",
		.value = "running",
	},
	{
		.key = "init.svc.adbd",
		.value = "running",
	},
	{
		.key = "init.svc.apaservice",
		.value = "running",
	},
	{
		.key = "init.svc.argos-daemon",
		.value = "running",
	},
	{
		.key = "init.svc.at_distributor",
		.value = "running",
	},
	{
		.key = "init.svc.audioserver",
		.value = "running",
	},
	{
		.key = "init.svc.auditd",
		.value = "running",
	},
	{
		.key = "init.svc.bootanim",
		.value = "stopped",
	},
	{
		.key = "init.svc.bootchecker",
		.value = "running",
	},
	{
		.key = "init.svc.cameraserver",
		.value = "running",
	},
	{
		.key = "init.svc.ccm",
		.value = "running",
	},
	{
		.key = "init.svc.cpboot-daemon",
		.value = "running",
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
		.key = "init.svc.drm",
		.value = "running",
	},
	{
		.key = "init.svc.epmlogd",
		.value = "stopped",
	},
	{
		.key = "init.svc.esecomm",
		.value = "running",
	},
	{
		.key = "init.svc.faced",
		.value = "running",
	},
	{
		.key = "init.svc.fingerprintd",
		.value = "running",
	},
	{
		.key = "init.svc.flash_recovery",
		.value = "stopped",
	},
	{
		.key = "init.svc.gatekeeperd",
		.value = "running",
	},
	{
		.key = "init.svc.gpsd",
		.value = "running",
	},
	{
		.key = "init.svc.healthd",
		.value = "running",
	},
	{
		.key = "init.svc.icd",
		.value = "stopped",
	},
	{
		.key = "init.svc.imsd",
		.value = "running",
	},
	{
		.key = "init.svc.installd",
		.value = "running",
	},
	{
		.key = "init.svc.iod",
		.value = "running",
	},
	{
		.key = "init.svc.irisd",
		.value = "running",
	},
	{
		.key = "init.svc.jackservice",
		.value = "running",
	},
	{
		.key = "init.svc.keystore",
		.value = "running",
	},
	{
		.key = "init.svc.lhd",
		.value = "running",
	},
	{
		.key = "init.svc.lmkd",
		.value = "running",
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
		.key = "init.svc.macloader",
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
		.key = "init.svc.mobicore",
		.value = "running",
	},
	{
		.key = "init.svc.netd",
		.value = "running",
	},
	{
		.key = "init.svc.otp",
		.value = "running",
	},
	{
		.key = "init.svc.p2p_supplicant",
		.value = "running",
	},
	{
		.key = "init.svc.powersnd",
		.value = "stopped",
	},
	{
		.key = "init.svc.prepare_param",
		.value = "stopped",
	},
	{
		.key = "init.svc.ril-daemon",
		.value = "running",
	},
	{
		.key = "init.svc.scs",
		.value = "stopped",
	},
	{
		.key = "init.svc.sdp_cryptod",
		.value = "running",
	},
	{
		.key = "init.svc.sec-sh",
		.value = "stopped",
	},
	{
		.key = "init.svc.secure_storage",
		.value = "running",
	},
	{
		.key = "init.svc.sem_daemon",
		.value = "stopped",
	},
	{
		.key = "init.svc.sensorhubservice",
		.value = "running",
	},
	{
		.key = "init.svc.servicemanager",
		.value = "running",
	},
	{
		.key = "init.svc.surfaceflinger",
		.value = "running",
	},
	{
		.key = "init.svc.swapon",
		.value = "stopped",
	},
	{
		.key = "init.svc.ueventd",
		.value = "running",
	},
	{
		.key = "init.svc.vaultkeeperd",
		.value = "running",
	},
	{
		.key = "init.svc.visiond",
		.value = "running",
	},
	{
		.key = "init.svc.vold",
		.value = "running",
	},
	{
		.key = "init.svc.watchdogd",
		.value = "running",
	},
	{
		.key = "init.svc.wsmd",
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
		.key = "logd.auditd",
		.value = "false",
	},
	{
		.key = "logd.kernel",
		.value = "false",
	},
	{
		.key = "net.bt.name",
		.value = "Android",
	},
	{
		.key = "net.change",
		.value = "net.iptype",
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
		.value = "android-59a0bfcfa426476b",
	},
	{
		.key = "net.iptype",
		.value = "502:v4v6v6",
	},
	{
		.key = "net.knox.shareddevice.version",
		.value = "2.8.0",
	},
	{
		.key = "net.knoxscep.version",
		.value = "2.2.0",
	},
	{
		.key = "net.knoxvpn.version",
		.value = "2.4.0",
	},
	{
		.key = "net.qtaguid_enabled",
		.value = "1",
	},
	{
		.key = "net.smart_switch.disabled",
		.value = "1",
	},
	{
		.key = "net.tcp.default_init_rwnd",
		.value = "60",
	},
	{
		.key = "nfc.fw.dfl_areacode",
		.value = "DEF",
	},
	{
		.key = "nfc.fw.downloadmode_force",
		.value = "0",
	},
	{
		.key = "nfc.fw.rfreg_display_ver",
		.value = "3",
	},
	{
		.key = "nfc.fw.rfreg_ver",
		.value = "17/6/22/21.53.25",
	},
	{
		.key = "nfc.fw.ver",
		.value = "S.LSI 4.5.2",
	},
	{
		.key = "nfc.smartcard.binded",
		.value = "true",
	},
	{
		.key = "persist.audio.allsoundmute",
		.value = "0",
	},
	{
		.key = "persist.audio.corefx",
		.value = "1",
	},
	{
		.key = "persist.audio.effectcpufreq",
		.value = "350000",
	},
	{
		.key = "persist.audio.finemediavolume",
		.value = "1",
	},
	{
		.key = "persist.audio.globaleffect",
		.value = "1",
	},
	{
		.key = "persist.audio.headsetsysvolume",
		.value = "9",
	},
	{
		.key = "persist.audio.hphonesysvolume",
		.value = "9",
	},
	{
		.key = "persist.audio.k2hd",
		.value = "1",
	},
	{
		.key = "persist.audio.mpseek",
		.value = "0",
	},
	{
		.key = "persist.audio.mysound",
		.value = "1",
	},
	{
		.key = "persist.audio.ringermode",
		.value = "1",
	},
	{
		.key = "persist.audio.sales_code",
		.value = "TPA",
	},
	{
		.key = "persist.audio.soundalivefxsec",
		.value = "1",
	},
	{
		.key = "persist.audio.stereospeaker",
		.value = "0",
	},
	{
		.key = "persist.audio.sysvolume",
		.value = "9",
	},
	{
		.key = "persist.audio.uhqa",
		.value = "1",
	},
	{
		.key = "persist.audio.voipcpufreq",
		.value = "0",
	},
	{
		.key = "persist.bluetooth_fw_ver",
		.value = "bcm4361B0_V0267.0301_semco.hcd",
	},
	{
		.key = "persist.demo.hdmirotationlock",
		.value = "false",
	},
	{
		.key = "persist.radio.embms.support",
		.value = "false",
	},
	{
		.key = "persist.radio.initphone-type",
		.value = "1",
	},
	{
		.key = "persist.radio.latest-modeltype",
		.value = "2",
	},
	{
		.key = "persist.radio.multisim.config",
		.value = "ss",
	},
	{
		.key = "persist.radio.plmnname",
		.value = "",
	},
	{
		.key = "persist.radio.sib16_support",
		.value = "1",
	},
	{
		.key = "persist.ril.ims.eutranParam",
		.value = "3",
	},
	{
		.key = "persist.ril.ims.org.eutranParam",
		.value = "3",
	},
	{
		.key = "persist.ril.ims.org.utranParam",
		.value = "0",
	},
	{
		.key = "persist.ril.ims.utranParam",
		.value = "0",
	},
	{
		.key = "persist.ril.modem.board",
		.value = "SHANNON355",
	},
	{
		.key = "persist.service.bdroid.version",
		.value = "5.0",
	},
	{
		.key = "persist.service.bgkeycount",
		.value = "",
	},
	{
		.key = "persist.sys.camera.preview",
		.value = "0",
	},
	{
		.key = "persist.sys.ccm.date",
		.value = "Wed Dec 13 20:58:07 KST 2017",
	},
	{
		.key = "persist.sys.clssprld1",
		.value = "888",
	},
	{
		.key = "persist.sys.clssprld2",
		.value = "348",
	},
	{
		.key = "persist.sys.cpboot",
		.value = "unknown",
	},
	{
		.key = "persist.sys.csc_status",
		.value = "normal",
	},
	{
		.key = "persist.sys.dalvik.vm.lib.2",
		.value = "libart.so",
	},
	{
		.key = "persist.sys.debug_omc",
		.value = "/system/omc/TPA",
	},
	{
		.key = "persist.sys.debug_omcnw",
		.value = "/system/omc/TPA",
	},
	{
		.key = "persist.sys.display_density",
		.value = "480",
	},
	{
		.key = "persist.sys.locale",
		.value = "en-US",
	},
	{
		.key = "persist.sys.localedefault",
		.value = "es-US",
	},
	{
		.key = "persist.sys.omc.enable",
		.value = "true",
	},
	{
		.key = "persist.sys.omc_etcpath",
		.value = "/system/omc/TPA/etc",
	},
	{
		.key = "persist.sys.omc_path",
		.value = "/system/omc/TPA",
	},
	{
		.key = "persist.sys.omc_respath",
		.value = "/omr/res",
	},
	{
		.key = "persist.sys.omc_support",
		.value = "true",
	},
	{
		.key = "persist.sys.omcnw_path",
		.value = "/system/omc/TPA",
	},
	{
		.key = "persist.sys.prev_omcnwcode",
		.value = "TPA",
	},
	{
		.key = "persist.sys.prev_salescode",
		.value = "TPA",
	},
	{
		.key = "persist.sys.profiler_ms",
		.value = "0",
	},
	{
		.key = "persist.sys.sb.setting.enabled",
		.value = "false",
	},
	{
		.key = "persist.sys.setupwizard",
		.value = "FINISH",
	},
	{
		.key = "persist.sys.silent",
		.value = "1",
	},
	{
		.key = "persist.sys.sm_mode",
		.value = "1",
	},
	{
		.key = "persist.sys.storage_preload",
		.value = "2",
	},
	{
		.key = "persist.sys.timezone",
		.value = "Asia/Shanghai",
	},
	{
		.key = "persist.sys.usb.config",
		.value = "mtp,adb",
	},
	{
		.key = "persist.sys.usb.dualrole",
		.value = "true",
	},
	{
		.key = "persist.sys.webview.vmsize",
		.value = "113626032",
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
		.key = "ril.CHAR",
		.value = "111727107",
	},
	{
		.key = "ril.CompleteMsg",
		.value = "OK",
	},
	{
		.key = "ril.ICC_TYPE",
		.value = "0",
	},
	{
		.key = "ril.IsManualSelection",
		.value = "0",
	},
	{
		.key = "ril.LIMA",
		.value = "40491",
	},
	{
		.key = "ril.RildInit",
		.value = "1",
	},
	{
		.key = "ril.airplane.mode",
		.value = "0",
	},
	{
		.key = "ril.approved_codever",
		.value = "none",
	},
	{
		.key = "ril.approved_cscver",
		.value = "none",
	},
	{
		.key = "ril.approved_modemver",
		.value = "none",
	},
	{
		.key = "ril.atd_status",
		.value = "1_1_0",
	},
	{
		.key = "ril.backoffstate",
		.value = "1024",
	},
	{
		.key = "ril.cbd.boot_done",
		.value = "1",
	},
	{
		.key = "ril.cbd.dt_revision",
		.value = "010",
	},
	{
		.key = "ril.cbd.first_xmit_done",
		.value = "1",
	},
	{
		.key = "ril.cbd.rfs_check_done",
		.value = "1",
	},
	{
		.key = "ril.cs_svc",
		.value = "1",
	},
	{
		.key = "ril.data.intfprefix",
		.value = "rmnet",
	},
	{
		.key = "ril.debug_modemfactory",
		.value = "CSC Feature State: IMS ON, EPDG ON",
	},
	{
		.key = "ril.ecclist00",
		.value = "112,911,999,000,110,118,119",
	},
	{
		.key = "ril.ecclist_net0",
		.value = "",
	},
	{
		.key = "ril.hasisim",
		.value = "0",
	},
	{
		.key = "ril.hw_ver",
		.value = "MP 0.700",
	},
	{
		.key = "ril.ims.ecsupport",
		.value = "2",
	},
	{
		.key = "ril.ims.ltevoicesupport",
		.value = "2",
	},
	{
		.key = "ril.initPB",
		.value = "0",
	},
	{
		.key = "ril.iscdma",
		.value = "0",
	},
	{
		.key = "ril.isctc",
		.value = "0",
	},
	{
		.key = "ril.model_id",
		.value = "QB8346489",
	},
	{
		.key = "ril.modem.board",
		.value = "SHANNON355",
	},
	{
		.key = "ril.official_cscver",
		.value = "G950FOXM1AQL5",
	},
	{
		.key = "ril.product_code",
		.value = "SM-G950FZKLTPA",
	},
	{
		.key = "ril.radiostate",
		.value = "10",
	},
	{
		.key = "ril.read.done",
		.value = "1",
	},
	{
		.key = "ril.rfcal_date",
		.value = "20170911",
	},
	{
		.key = "ril.serialnumber",
		.value = "RF8J9134VLF",
	},
	{
		.key = "ril.servicestate",
		.value = "2",
	},
	{
		.key = "ril.signal.disprssi",
		.value = "false",
	},
	{
		.key = "ril.signal.param",
		.value = "-11,0,255",
	},
	{
		.key = "ril.simoperator",
		.value = "",
	},
	{
		.key = "ril.ss.routing",
		.value = "0",
	},
	{
		.key = "ril.subinfo",
		.value = "0:-2",
	},
	{
		.key = "ril.sw_ver",
		.value = "G950FXXU1AQL5",
	},
	{
		.key = "ril.timezoneID",
		.value = "",
	},
	{
		.key = "ril.twwan911Timer",
		.value = "40",
	},
	{
		.key = "ril.voice.rat",
		.value = "3",
	},
	{
		.key = "ril.voicecapable",
		.value = "true",
	},
	{
		.key = "rild.libpath",
		.value = "/system/lib64/libsec-ril.so",
	},
	{
		.key = "rild.libpath2",
		.value = "/system/lib64/libsec-ril-dsds.so",
	},
	{
		.key = "ro.adb.qemud",
		.value = "1",
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
		.key = "ro.arch",
		.value = "exynos8895",
	},
	{
		.key = "ro.astcenc.astcsupport",
		.value = "1",
	},
	{
		.key = "ro.baseband",
		.value = "unknown",
	},
	{
		.key = "ro.bluetooth.tty",
		.value = "ttySAC1",
	},
	{
		.key = "ro.board.platform",
		.value = "exynos5",
	},
	{
		.key = "ro.boot.ap_serial",
		.value = "0x03B5CD48DA62",
	},
	{
		.key = "ro.boot.bootloader",
		.value = "G950FXXU1AQL5",
	},
	{
		.key = "ro.boot.carrierid",
		.value = "TPA",
	},
	{
		.key = "ro.boot.carrierid_offset",
		.value = "7340608",
	},
	{
		.key = "ro.boot.cp_reserved_mem",
		.value = "off",
	},
	{
		.key = "ro.boot.debug_level",
		.value = "0x4f4c",
	},
	{
		.key = "ro.boot.em.did",
		.value = "03b5cd48da62",
	},
	{
		.key = "ro.boot.em.model",
		.value = "SM-G950F",
	},
	{
		.key = "ro.boot.em.status",
		.value = "0x0",
	},
	{
		.key = "ro.boot.emmc_checksum",
		.value = "3",
	},
	{
		.key = "ro.boot.flash.locked",
		.value = "1",
	},
	{
		.key = "ro.boot.fmp_config",
		.value = "1",
	},
	{
		.key = "ro.boot.hardware",
		.value = "samsungexynos8895",
	},
	{
		.key = "ro.boot.hmac_mismatch",
		.value = "0",
	},
	{
		.key = "ro.boot.hw_rev",
		.value = "10",
	},
	{
		.key = "ro.boot.odin_download",
		.value = "1",
	},
	{
		.key = "ro.boot.sales.param.offset",
		.value = "7340572",
	},
	{
		.key = "ro.boot.sec_atd.tty",
		.value = "/dev/ttySAC0",
	},
	{
		.key = "ro.boot.security_mode",
		.value = "1526595585",
	},
	{
		.key = "ro.boot.selinux",
		.value = "enforcing",
	},
	{
		.key = "ro.boot.serialno",
		.value = "9889d5373450584155",
	},
	{
		.key = "ro.boot.smsn_offset",
		.value = "7345920",
	},
	{
		.key = "ro.boot.ucs_mode",
		.value = "0",
	},
	{
		.key = "ro.boot.verifiedbootstate",
		.value = "green",
	},
	{
		.key = "ro.boot.warranty_bit",
		.value = "0",
	},
	{
		.key = "ro.boot_recovery",
		.value = "unknown",
	},
	{
		.key = "ro.bootimage.build.date",
		.value = "Wed Dec 13 20:58:07 KST 2017",
	},
	{
		.key = "ro.bootimage.build.date.utc",
		.value = "1513166287",
	},
	{
		.key = "ro.bootimage.build.fingerprint",
		.value = "samsung/dreamltexx/dreamlte:7.0/NRD90M/G950FXXU1AQL5:user/test-keys",
	},
	{
		.key = "ro.bootloader",
		.value = "G950FXXU1AQL5",
	},
	{
		.key = "ro.bootmode",
		.value = "unknown",
	},
	{
		.key = "ro.bt.bdaddr_path",
		.value = "/efs/bluetooth/bt_addr",
	},
	{
		.key = "ro.build.PDA",
		.value = "G950FXXU1AQL5",
	},
	{
		.key = "ro.build.changelist",
		.value = "12640043",
	},
	{
		.key = "ro.build.characteristics",
		.value = "phone,emulator",
	},
	{
		.key = "ro.build.date",
		.value = "Wed Dec 13 20:58:07 KST 2017",
	},
	{
		.key = "ro.build.date.utc",
		.value = "1513166287",
	},
	{
		.key = "ro.build.description",
		.value = "dreamltexx-user 7.0 NRD90M G950FXXU1AQL5 release-keys",
	},
	{
		.key = "ro.build.display.id",
		.value = "NRD90M.G950FXXU1AQL5",
	},
	{
		.key = "ro.build.fingerprint",
		.value = "samsung/dreamltexx/dreamlte:7.0/NRD90M/G950FXXU1AQL5:user/release-keys",
	},
	{
		.key = "ro.build.flavor",
		.value = "dreamltexx-user",
	},
	{
		.key = "ro.build.host",
		.value = "SWDG4522",
	},
	{
		.key = "ro.build.id",
		.value = "NRD90M",
	},
	{
		.key = "ro.build.official.release",
		.value = "true",
	},
	{
		.key = "ro.build.product",
		.value = "dreamlte",
	},
	{
		.key = "ro.build.scafe.version",
		.value = "2017A",
	},
	{
		.key = "ro.build.selinux",
		.value = "1",
	},
	{
		.key = "ro.build.selinux.enforce",
		.value = "1",
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
		.key = "ro.build.user",
		.value = "dpi",
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
		.key = "ro.build.version.incremental",
		.value = "G950FXXU1AQL5",
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
		.key = "ro.build.version.security_index",
		.value = "1",
	},
	{
		.key = "ro.build.version.security_patch",
		.value = "2017-12-01",
	},
	{
		.key = "ro.build.version.sem",
		.value = "2403",
	},
	{
		.key = "ro.build.version.sep",
		.value = "80100",
	},
	{
		.key = "ro.carrier",
		.value = "unknown",
	},
	{
		.key = "ro.carrierid",
		.value = "TPA",
	},
	{
		.key = "ro.carrierid.param.offset",
		.value = "7340608",
	},
	{
		.key = "ro.chipname",
		.value = "exynos8895",
	},
	{
		.key = "ro.com.google.clientidbase",
		.value = "android-samsung",
	},
	{
		.key = "ro.com.google.gmsversion",
		.value = "7.0_r11",
	},
	{
		.key = "ro.config.alarm_alert",
		.value = "Morning_Glory.ogg",
	},
	{
		.key = "ro.config.dha_cached_max",
		.value = "16",
	},
	{
		.key = "ro.config.dha_cached_min",
		.value = "6",
	},
	{
		.key = "ro.config.dha_empty_init",
		.value = "32",
	},
	{
		.key = "ro.config.dha_empty_max",
		.value = "32",
	},
	{
		.key = "ro.config.dha_empty_min",
		.value = "8",
	},
	{
		.key = "ro.config.dha_pwhitelist_enable",
		.value = "1",
	},
	{
		.key = "ro.config.dha_pwhl_key",
		.value = "7938",
	},
	{
		.key = "ro.config.dha_th_rate",
		.value = "2.0",
	},
	{
		.key = "ro.config.dmverity",
		.value = "true",
	},
	{
		.key = "ro.config.fall_prevent_enable",
		.value = "true",
	},
	{
		.key = "ro.config.iccc_version",
		.value = "1.0",
	},
	{
		.key = "ro.config.infinite_bg_enable",
		.value = "true",
	},
	{
		.key = "ro.config.kap",
		.value = "true",
	},
	{
		.key = "ro.config.kap_default_on",
		.value = "true",
	},
	{
		.key = "ro.config.knox",
		.value = "v30",
	},
	{
		.key = "ro.config.media_sound",
		.value = "Media_preview_Touch_the_light.ogg",
	},
	{
		.key = "ro.config.notification_sound",
		.value = "Skyline.ogg",
	},
	{
		.key = "ro.config.notification_sound_2",
		.value = "S_Charming_Bell.ogg",
	},
	{
		.key = "ro.config.ringtone",
		.value = "Over_the_Horizon.ogg",
	},
	{
		.key = "ro.config.ringtone_2",
		.value = "Basic_Bell.ogg",
	},
	{
		.key = "ro.config.rkp",
		.value = "true",
	},
	{
		.key = "ro.config.rm_preload_enabled",
		.value = "1",
	},
	{
		.key = "ro.config.systemaudiodebug",
		.value = "abox",
	},
	{
		.key = "ro.config.tima",
		.value = "1",
	},
	{
		.key = "ro.config.timaversion",
		.value = "3.0",
	},
	{
		.key = "ro.cp_debug_level",
		.value = "unknown",
	},
	{
		.key = "ro.crypto.fs_crypto_blkdev",
		.value = "/dev/block/dm-1",
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
		.value = "block",
	},
	{
		.key = "ro.csc.country_code",
		.value = "PANAMA",
	},
	{
		.key = "ro.csc.countryiso_code",
		.value = "PA",
	},
	{
		.key = "ro.csc.facebook.partnerid",
		.value = "samsung:dec1cc9c-1497-4aab-b953-cee702c2a481",
	},
	{
		.key = "ro.csc.omcnw_code",
		.value = "TPA",
	},
	{
		.key = "ro.csc.sales_code",
		.value = "TPA",
	},
	{
		.key = "ro.dalvik.vm.native.bridge",
		.value = "0",
	},
	{
		.key = "ro.debug_level",
		.value = "0x4f4c",
	},
	{
		.key = "ro.debuggable",
		.value = "0",
	},
	{
		.key = "ro.emmc_checksum",
		.value = "3",
	},
	{
		.key = "ro.error.receiver.default",
		.value = "com.samsung.receiver.error",
	},
	{
		.key = "ro.expect.recovery_id",
		.value = "0x48c53cca3ea48ceba1bcfabccc7259c10b06c5d2000000000000000000000000",
	},
	{
		.key = "ro.fmp_config",
		.value = "1",
	},
	{
		.key = "ro.frp.pst",
		.value = "/dev/block/persistent",
	},
	{
		.key = "ro.gfx.driver.0",
		.value = "com.samsung.gpudriver.S8MaliG71_70",
	},
	{
		.key = "ro.hardware",
		.value = "samsungexynos8895",
	},
	{
		.key = "ro.hardware.egl",
		.value = "mali",
	},
	{
		.key = "ro.hardware.keystore",
		.value = "mdfpp",
	},
	{
		.key = "ro.hdcp2.rx",
		.value = "tz",
	},
	{
		.key = "ro.hmac_mismatch",
		.value = "0",
	},
	{
		.key = "ro.hwui.drop_shadow_cache_size",
		.value = "6",
	},
	{
		.key = "ro.hwui.gradient_cache_size",
		.value = "2",
	},
	{
		.key = "ro.hwui.layer_cache_size",
		.value = "58",
	},
	{
		.key = "ro.hwui.path_cache_size",
		.value = "16",
	},
	{
		.key = "ro.hwui.r_buffer_cache_size",
		.value = "8",
	},
	{
		.key = "ro.hwui.shape_cache_size",
		.value = "4",
	},
	{
		.key = "ro.hwui.text_large_cache_height",
		.value = "2048",
	},
	{
		.key = "ro.hwui.text_large_cache_width",
		.value = "4096",
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
		.value = "88",
	},
	{
		.key = "ro.im.param.offset",
		.value = "unknown",
	},
	{
		.key = "ro.input.resamplelatency",
		.value = "1",
	},
	{
		.key = "ro.kernel.qemu",
		.value = "0",
	},
	{
		.key = "ro.kernel.qemu.gles",
		.value = "0",
	},
	{
		.key = "ro.knox.enhance.zygote.aslr",
		.value = "0",
	},
	{
		.key = "ro.mct.compressiontype",
		.value = "ETC1",
	},
	{
		.key = "ro.me.param.offset",
		.value = "unknown",
	},
	{
		.key = "ro.mst.support",
		.value = "1",
	},
	{
		.key = "ro.multisim.simslotcount",
		.value = "1",
	},
	{
		.key = "ro.oem.key1",
		.value = "TPA",
	},
	{
		.key = "ro.oem_unlock_supported",
		.value = "1",
	},
	{
		.key = "ro.opa.eligible_device",
		.value = "true",
	},
	{
		.key = "ro.opengles.version",
		.value = "196610",
	},
	{
		.key = "ro.pr.param.offset",
		.value = "unknown",
	},
	{
		.key = "ro.product.board",
		.value = "universal8895",
	},
	{
		.key = "ro.product.brand",
		.value = "samsung",
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
		.key = "ro.product.device",
		.value = "dreamlte",
	},
	{
		.key = "ro.product.first_api_level",
		.value = "24",
	},
	{
		.key = "ro.product.locale",
		.value = "en-GB",
	},
	{
		.key = "ro.product.manufacturer",
		.value = "samsung",
	},
	{
		.key = "ro.product.model",
		.value = "SM-G950F",
	},
	{
		.key = "ro.product.name",
		.value = "dreamltexx",
	},
	{
		.key = "ro.product_ship",
		.value = "true",
	},
	{
		.key = "ro.radio.noril",
		.value = "no",
	},
	{
		.key = "ro.revision",
		.value = "10",
	},
	{
		.key = "ro.ril.gprsclass",
		.value = "10",
	},
	{
		.key = "ro.ril.hsxpa",
		.value = "1",
	},
	{
		.key = "ro.ril.network_code",
		.value = "EUR",
	},
	{
		.key = "ro.rtn_config",
		.value = "unknown",
	},
	{
		.key = "ro.runtime.firstboot",
		.value = "1524177974249",
	},
	{
		.key = "ro.sales.param.offset",
		.value = "7340572",
	},
	{
		.key = "ro.secure",
		.value = "1",
	},
	{
		.key = "ro.securestorage.support",
		.value = "true",
	},
	{
		.key = "ro.security.ese.cosname",
		.value = "UT3.0_01000005",
	},
	{
		.key = "ro.security.esest",
		.value = "per0201exi",
	},
	{
		.key = "ro.security.icd.flagmode",
		.value = "multi",
	},
	{
		.key = "ro.security.mdpp.release",
		.value = "2",
	},
	{
		.key = "ro.security.mdpp.ux",
		.value = "Enabled",
	},
	{
		.key = "ro.security.mdpp.ver",
		.value = "3.0",
	},
	{
		.key = "ro.security.reactive.version",
		.value = "2.0.11",
	},
	{
		.key = "ro.security.vaultkeeper.feature",
		.value = "1",
	},
	{
		.key = "ro.security.vpnpp.release",
		.value = "8.1",
	},
	{
		.key = "ro.security.vpnpp.ver",
		.value = "1.4",
	},
	{
		.key = "ro.security.wlan.release",
		.value = "2",
	},
	{
		.key = "ro.security.wlan.ver",
		.value = "1.0",
	},
	{
		.key = "ro.security_mode",
		.value = "1526595585",
	},
	{
		.key = "ro.serialno",
		.value = "9889d5373450584155",
	},
	{
		.key = "ro.setupwizard.mode",
		.value = "OPTIONAL",
	},
	{
		.key = "ro.sf.init.lcd_density",
		.value = "640",
	},
	{
		.key = "ro.sf.lcd_density",
		.value = "480",
	},
	{
		.key = "ro.sku.param.offset",
		.value = "unknown",
	},
	{
		.key = "ro.sn.param.offset",
		.value = "unknown",
	},
	{
		.key = "ro.supportmodel.mptcp",
		.value = "1",
	},
	{
		.key = "ro.telephony.default_network",
		.value = "9",
	},
	{
		.key = "ro.warranty_bit",
		.value = "0",
	},
	{
		.key = "ro.wifi.channels",
		.value = "",
	},
	{
		.key = "ro.wsmd.enable",
		.value = "true",
	},
	{
		.key = "ro.zygote",
		.value = "zygote64_32",
	},
	{
		.key = "secmm.codecsolution.ready",
		.value = "1",
	},
	{
		.key = "secmm.player.uhqamode",
		.value = "True",
	},
	{
		.key = "security.ASKS.policy_version",
		.value = "161228",
	},
	{
		.key = "security.ASKS.version",
		.value = "1.4",
	},
	{
		.key = "security.mdpp",
		.value = "Ready",
	},
	{
		.key = "security.mdpp.mass",
		.value = "skmm",
	},
	{
		.key = "security.mdpp.result",
		.value = "None",
	},
	{
		.key = "security.perf_harden",
		.value = "1",
	},
	{
		.key = "security.semdaemonfinish",
		.value = "1",
	},
	{
		.key = "selinux.policy_version",
		.value = "SEPF_SECMOBILE_7.0_0010",
	},
	{
		.key = "selinux.reload_policy",
		.value = "1",
	},
	{
		.key = "service.bootanim.exit",
		.value = "0",
	},
	{
		.key = "service.camera.client",
		.value = "",
	},
	{
		.key = "service.camera.running",
		.value = "0",
	},
	{
		.key = "service.camera.running_0",
		.value = "0",
	},
	{
		.key = "service.media.powersnd",
		.value = "1",
	},
	{
		.key = "storage.mmc.size",
		.value = "63963136000",
	},
	{
		.key = "storage.support.sdcard",
		.value = "1",
	},
	{
		.key = "storage.support.usb",
		.value = "1",
	},
	{
		.key = "sys.aasservice.aason",
		.value = "true",
	},
	{
		.key = "sys.bartender.batterystats.ver",
		.value = "16",
	},
	{
		.key = "sys.boot_completed",
		.value = "1",
	},
	{
		.key = "sys.cameramode.cam_binning",
		.value = "0",
	},
	{
		.key = "sys.cameramode.cam_fps",
		.value = "-1",
	},
	{
		.key = "sys.config.amp_perf_enable",
		.value = "true",
	},
	{
		.key = "sys.config.mars_version",
		.value = "2.10",
	},
	{
		.key = "sys.config.slginfo_debug",
		.value = "false",
	},
	{
		.key = "sys.config.slginfo_dha",
		.value = "true",
	},
	{
		.key = "sys.config.slginfo_enable",
		.value = "false",
	},
	{
		.key = "sys.config.slginfo_max_count",
		.value = "1000",
	},
	{
		.key = "sys.config.slginfo_meminfo",
		.value = "true",
	},
	{
		.key = "sys.config.slginfo_vmstat",
		.value = "true",
	},
	{
		.key = "sys.daydream.connected",
		.value = "0",
	},
	{
		.key = "sys.dockstate",
		.value = "0",
	},
	{
		.key = "sys.enterprise.billing.dualsim",
		.value = "true",
	},
	{
		.key = "sys.enterprise.billing.version",
		.value = "1.3.0",
	},
	{
		.key = "sys.enterprise.otp.version",
		.value = "2.6.0",
	},
	{
		.key = "sys.fdcloseobs",
		.value = "1",
	},
	{
		.key = "sys.gps.chipinfo",
		.value = "BCM4774",
	},
	{
		.key = "sys.gps.chipvendor",
		.value = "Broadcom",
	},
	{
		.key = "sys.gps.swversion",
		.value = "305302",
	},
	{
		.key = "sys.is_members",
		.value = "exist",
	},
	{
		.key = "sys.isdumpstaterunning",
		.value = "0",
	},
	{
		.key = "sys.knox.exists",
		.value = "0",
	},
	{
		.key = "sys.knox.store",
		.value = "0",
	},
	{
		.key = "sys.mdniecontrolservice.mscon",
		.value = "true",
	},
	{
		.key = "sys.nfc.support",
		.value = "1",
	},
	{
		.key = "sys.oem_unlock_allowed",
		.value = "0",
	},
	{
		.key = "sys.reset_reason",
		.value = "N|RP",
	},
	{
		.key = "sys.siop.level",
		.value = "-3",
	},
	{
		.key = "sys.skip_lockscreen",
		.value = "0",
	},
	{
		.key = "sys.ssrm.game_running",
		.value = "false",
	},
	{
		.key = "sys.ssrm.mdnie",
		.value = "-1",
	},
	{
		.key = "sys.sysctl.extra_free_kbytes",
		.value = "28096",
	},
	{
		.key = "sys.sysctl.tcp_def_init_rwnd",
		.value = "60",
	},
	{
		.key = "sys.usb.config",
		.value = "mtp,adb",
	},
	{
		.key = "sys.usb.state",
		.value = "mtp,adb",
	},
	{
		.key = "sys.use_fifo_ui",
		.value = "0",
	},
	{
		.key = "sys.vs.display",
		.value = "",
	},
	{
		.key = "sys.vs.mode",
		.value = "false",
	},
	{
		.key = "system.camera.CC.disable",
		.value = "0",
	},
	{
		.key = "telephony.lteOnCdmaDevice",
		.value = "0",
	},
	{
		.key = "vold.crypt.type",
		.value = "default",
	},
	{
		.key = "vold.decrypt",
		.value = "trigger_restart_framework",
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
		.key = "vzw.os.rooted",
		.value = "false",
	},
	{
		.key = "wifi.interface",
		.value = "wlan0",
	},
	{
		.key = "wlan.driver.status",
		.value = "ok",
	},
	{
		.key = "wlan.p2p.chkintent",
		.value = "8",
	},
	{
		.key = "wlan.wfd.status",
		.value = "disconnected",
	},
	{NULL},
};
#endif /* __ANDROID__ */
