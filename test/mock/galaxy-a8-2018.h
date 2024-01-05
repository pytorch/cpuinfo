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
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd03\n"
			   "CPU revision\t: 4\n"
			   "\n"
			   "processor\t: 5\n"
			   "BogoMIPS\t: 52.00\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd03\n"
			   "CPU revision\t: 4\n"
			   "\n"
			   "processor\t: 6\n"
			   "BogoMIPS\t: 52.00\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd09\n"
			   "CPU revision\t: 2\n"
			   "\n"
			   "processor\t: 7\n"
			   "BogoMIPS\t: 52.00\n"
			   "Features\t: fp asimd evtstrm aes pmull sha1 sha2 crc32\n"
			   "CPU implementer\t: 0x41\n"
			   "CPU architecture: 8\n"
			   "CPU variant\t: 0x0\n"
			   "CPU part\t: 0xd09\n"
			   "CPU revision\t: 2\n"
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
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd03\n"
			"CPU revision\t: 4\n"
			"\n"
			"processor\t: 5\n"
			"BogoMIPS\t: 52.00\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd03\n"
			"CPU revision\t: 4\n"
			"\n"
			"processor\t: 6\n"
			"BogoMIPS\t: 52.00\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd09\n"
			"CPU revision\t: 2\n"
			"\n"
			"processor\t: 7\n"
			"BogoMIPS\t: 52.00\n"
			"Features\t: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm aes pmull sha1 sha2 crc32\n"
			"CPU implementer\t: 0x41\n"
			"CPU architecture: 8\n"
			"CPU variant\t: 0x0\n"
			"CPU part\t: 0xd09\n"
			"CPU revision\t: 2\n"
			"\n",
	},
#endif
	{
		.path = "/system/build.prop",
		.size = 5259,
		.content =
			"\n"
			"# begin build properties\n"
			"# autogenerated by buildinfo.sh\n"
			"ro.build.id=NMF26X\n"
			"ro.build.display.id=NMF26X.A530FXXU1ARA3\n"
			"ro.build.version.incremental=A530FXXU1ARA3\n"
			"ro.build.version.sdk=25\n"
			"ro.build.version.preview_sdk=0\n"
			"ro.build.version.codename=REL\n"
			"ro.build.version.all_codenames=REL\n"
			"ro.build.version.release=7.1.1\n"
			"ro.build.version.security_patch=2018-01-01\n"
			"ro.build.version.base_os=\n"
			"ro.build.version.security_index=1\n"
			"ro.build.date=Fri Jan 12 19:09:06 KST 2018\n"
			"ro.build.date.utc=1515751746\n"
			"ro.build.type=user\n"
			"ro.build.user=dpi\n"
			"ro.build.host=SWHE8816\n"
			"ro.build.tags=release-keys\n"
			"ro.build.flavor=jackpotltexx-user\n"
			"ro.product.model=SM-A530F\n"
			"ro.product.brand=samsung\n"
			"# ro.build.product is obsolete; use ro.product.device\n"
			"ro.build.product=jackpotlte\n"
			"ro.product.name=jackpotltexx\n"
			"ro.product.device=jackpotlte\n"
			"ro.product.board=universal7885\n"
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
			"# Do not try to parse description, fingerprint, or thumbprint\n"
			"ro.build.description=jackpotltexx-user 7.1.1 NMF26X A530FXXU1ARA3 release-keys\n"
			"ro.build.fingerprint=samsung/jackpotltexx/jackpotlte:7.1.1/NMF26X/A530FXXU1ARA3:user/release-keys\n"
			"ro.build.characteristics=phone,emulator\n"
			"# Samsung Specific Properties\n"
			"ro.build.PDA=A530FXXU1ARA3\n"
			"ro.build.official.release=true\n"
			"ro.config.rm_preload_enabled=1\n"
			"ro.build.changelist=12719828\n"
			"ro.product_ship=true\n"
			"ro.chipname=exynos7885\n"
			"# end build properties\n"
			"\n"
			"#\n"
			"# HWUI_BUILD_PROPERTIES\n"
			"#\n"
			"ro.hwui.texture_cache_size=50\n"
			"ro.hwui.layer_cache_size=34\n"
			"ro.hwui.path_cache_size=10\n"
			"ro.hwui.texture_cache_flushrate=0.4\n"
			"ro.hwui.shape_cache_size=4\n"
			"ro.hwui.gradient_cache_size=2\n"
			"ro.hwui.drop_shadow_cache_size=6\n"
			"ro.hwui.r_buffer_cache_size=4\n"
			"ro.hwui.text_small_cache_width=1024\n"
			"ro.hwui.text_small_cache_height=1024\n"
			"ro.hwui.text_large_cache_width=2048\n"
			"ro.hwui.text_large_cache_height=1024\n"
			"#\n"
			"# from device/samsung/jackpotlte/system.prop\n"
			"#\n"
			"#\n"
			"# system.prop for universal7885\n"
			"#\n"
			"\n"
			"ro.sf.lcd_density=480\n"
			"\n"
			"ro.arch=exynos7885\n"
			"#ro.kernel.qemu=1\n"
			"#ro.kernel.qemu.gles=2\n"
			"persist.demo.hdmirotationlock=false\n"
			"\n"
			"# read DS/SS property\n"
			"import /efs/factory.prop\n"
			"\n"
			"#Enable S.LSI USB settings\n"
			"dev.usbsetting.embedded=on\n"
			"\n"
			"# Specify a vendor RIL and a device node for IPC\n"
			"rild.libpath=/system/lib64/libsec-ril.so\n"
			"rild.libargs=-d /dev/umts_ipc0\n"
			"exynos.modempath=/dev/block/sda5\n"
			"exynos.modem.logsympath=/dev/block/sda6\n"
			"persist.audio.deepbuffer_delay=99\n"
			"\n"
			"#Phone early start enable flag\n"
			"sys.config.phone_start_early=true\n"
			"#\n"
			"# ADDITIONAL_BUILD_PROPERTIES\n"
			"#\n"
			"ro.astcenc.astcsupport=1\n"
			"ro.mct.compressiontype=ETC1\n"
			"ro.config.dmverity=true\n"
			"ro.config.kap_default_on=true\n"
			"ro.config.kap=true\n"
			"ro.multisim.simslotcount=2\n"
			"dalvik.vm.image-dex2oat-filter=speed\n"
			"dalvik.vm.dex2oat-filter=speed\n"
			"ro.product.first_api_level=25\n"
			"ro.opengles.version=196610\n"
			"ro.sf.lcd_density=480\n"
			"debug.slsi_platform=1\n"
			"debug.hwc.winupdate=1\n"
			"wifi.interface=wlan0\n"
			"drm.service.enabled=true\n"
			"dalvik.vm.heapstartsize=8m\n"
			"dalvik.vm.heapgrowthlimit=192m\n"
			"dalvik.vm.heapsize=512m\n"
			"dalvik.vm.heaptargetutilization=0.75\n"
			"dalvik.vm.heapminfree=512k\n"
			"dalvik.vm.heapmaxfree=8m\n"
			"ro.hdcp2.rx=tz\n"
			"ro.error.receiver.default=com.samsung.receiver.error\n"
			"ro.build.scafe.version=2017A\n"
			"ro.frp.pst=/dev/block/persistent\n"
			"security.mdpp.mass=skmm\n"
			"ro.wsmd.enable=true\n"
			"sys.config.activelaunch_enable=true\n"
			"ro.security.keystore.keytype=sak\n"
			"ro.hardware.keystore=mdfpp\n"
			"ro.config.dha_cached_max=16\n"
			"ro.config.dha_cached_min=4\n"
			"ro.config.dha_empty_init=16\n"
			"ro.config.dha_empty_max=16\n"
			"ro.config.dha_empty_min=8\n"
			"ro.config.dha_th_rate=2.6\n"
			"ro.config.dha_lmk_scale=0.583\n"
			"ro.cfg.dha_th_rate=2.0\n"
			"ro.cfg.dha_lmk_scale=1\n"
			"ro.cfg.dha_cached_min=8\n"
			"ro.cfg.dha_cached_max=24\n"
			"ro.cfg.dha_empty_min=8\n"
			"ro.cfg.dha_empty_init=32\n"
			"ro.cfg.dha_empty_max=32\n"
			"ro.config.dha_pwhl_key=96\n"
			"ro.config.dha_pwhitelist_enable=1\n"
			"ro.config.ringtone=Over_the_Horizon.ogg\n"
			"ro.config.notification_sound=Skyline.ogg\n"
			"ro.config.alarm_alert=Morning_Glory.ogg\n"
			"ro.config.media_sound=Media_preview_Touch_the_light.ogg\n"
			"ro.config.ringtone_2=Basic_Bell.ogg\n"
			"ro.config.notification_sound_2=S_Charming_Bell.ogg\n"
			"ro.config.systemaudiodebug=abox\n"
			"ro.security.vaultkeeper.feature=1\n"
			"keyguard.no_require_sim=true\n"
			"ro.carrier=unknown\n"
			"rild.libpath=/system/lib64/libsec-ril.so\n"
			"rild.libpath2=/system/lib64/libsec-ril-dsds.so\n"
			"persist.radio.sib16_support=1\n"
			"ro.security.icd.flagmode=multi\n"
			"security.ASKS.policy_version=000000\n"
			"ro.ril.hsxpa=1\n"
			"ro.ril.gprsclass=10\n"
			"ro.adb.qemud=1\n"
			"ro.com.google.clientidbase=android-samsung-ss\n"
			"ro.setupwizard.wifi_on_exit=false\n"
			"ro.setupwizard.mode=OPTIONAL\n"
			"ro.com.google.gmsversion=7.1_r8\n"
			"ro.build.selinux=1\n"
			"persist.sys.dalvik.vm.lib.2=libart.so\n"
			"dalvik.vm.isa.arm64.variant=generic\n"
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
			"ro.build.version.sem=2502\n"
			"ro.build.version.sep=80500\n"
			"ro.expect.recovery_id=0xae1c9693749cb6862cd35307ec060ab105079a77000000000000000000000000\n"
			"\n",
	},
	{
		.path = "/sys/devices/soc0/family",
		.size = 15,
		.content = "Samsung Exynos\n",
	},
	{
		.path = "/sys/devices/soc0/machine",
		.size = 55,
		.content = "Samsung JACKPOTLTE EUR rev07 board based on EXYNOS7885\n",
	},
	{
		.path = "/sys/devices/soc0/revision",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/soc0/soc_id",
		.size = 11,
		.content = "EXYNOS7885\n",
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
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1586000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/related_cpus",
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors",
		.size = 35,
		.content = "userspace interactive performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1586000\n",
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
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/stats/time_in_state",
		.size = 123,
		.content = "1586000 8312\n"
			   "1482000 36\n"
			   "1352000 60\n"
			   "1248000 38\n"
			   "1144000 26\n"
			   "1014000 10\n"
			   "902000 4\n"
			   "839000 52\n"
			   "757000 4\n"
			   "676000 2\n"
			   "546000 8\n"
			   "449000 8\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/cpufreq/stats/total_trans",
		.size = 3,
		.content = "92\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_siblings",
		.size = 3,
		.content = "3f\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu0/topology/core_siblings_list",
		.size = 4,
		.content = "0-5\n",
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
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1586000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/related_cpus",
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_available_governors",
		.size = 35,
		.content = "userspace interactive performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1586000\n",
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
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/stats/time_in_state",
		.size = 123,
		.content = "1586000 8523\n"
			   "1482000 36\n"
			   "1352000 60\n"
			   "1248000 38\n"
			   "1144000 26\n"
			   "1014000 10\n"
			   "902000 4\n"
			   "839000 52\n"
			   "757000 4\n"
			   "676000 2\n"
			   "546000 8\n"
			   "449000 8\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/cpufreq/stats/total_trans",
		.size = 3,
		.content = "92\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_id",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_siblings",
		.size = 3,
		.content = "3f\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu1/topology/core_siblings_list",
		.size = 4,
		.content = "0-5\n",
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
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1586000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/related_cpus",
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_available_governors",
		.size = 35,
		.content = "userspace interactive performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1586000\n",
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
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/stats/time_in_state",
		.size = 123,
		.content = "1586000 8736\n"
			   "1482000 36\n"
			   "1352000 60\n"
			   "1248000 38\n"
			   "1144000 26\n"
			   "1014000 10\n"
			   "902000 4\n"
			   "839000 52\n"
			   "757000 4\n"
			   "676000 2\n"
			   "546000 8\n"
			   "449000 8\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/cpufreq/stats/total_trans",
		.size = 3,
		.content = "92\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_id",
		.size = 2,
		.content = "2\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_siblings",
		.size = 3,
		.content = "3f\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu2/topology/core_siblings_list",
		.size = 4,
		.content = "0-5\n",
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
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1586000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/related_cpus",
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_available_governors",
		.size = 35,
		.content = "userspace interactive performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1586000\n",
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
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/stats/time_in_state",
		.size = 123,
		.content = "1586000 8977\n"
			   "1482000 36\n"
			   "1352000 60\n"
			   "1248000 38\n"
			   "1144000 26\n"
			   "1014000 10\n"
			   "902000 4\n"
			   "839000 52\n"
			   "757000 4\n"
			   "676000 2\n"
			   "546000 8\n"
			   "449000 8\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/cpufreq/stats/total_trans",
		.size = 3,
		.content = "92\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_id",
		.size = 2,
		.content = "3\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_siblings",
		.size = 3,
		.content = "3f\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu3/topology/core_siblings_list",
		.size = 4,
		.content = "0-5\n",
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
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1586000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/related_cpus",
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_available_governors",
		.size = 35,
		.content = "userspace interactive performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1586000\n",
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
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/stats/time_in_state",
		.size = 123,
		.content = "1586000 9233\n"
			   "1482000 36\n"
			   "1352000 68\n"
			   "1248000 38\n"
			   "1144000 26\n"
			   "1014000 10\n"
			   "902000 4\n"
			   "839000 52\n"
			   "757000 4\n"
			   "676000 2\n"
			   "546000 8\n"
			   "449000 8\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/cpufreq/stats/total_trans",
		.size = 3,
		.content = "94\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/core_id",
		.size = 2,
		.content = "4\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/core_siblings",
		.size = 3,
		.content = "3f\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu4/topology/core_siblings_list",
		.size = 4,
		.content = "0-5\n",
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
		.size = 12,
		.content = "exynos_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/affected_cpus",
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "1586000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/related_cpus",
		.size = 12,
		.content = "0 1 2 3 4 5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_available_governors",
		.size = 35,
		.content = "userspace interactive performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_cur_freq",
		.size = 8,
		.content = "1586000\n",
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
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "449000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/stats/time_in_state",
		.size = 123,
		.content = "1586000 9459\n"
			   "1482000 36\n"
			   "1352000 68\n"
			   "1248000 38\n"
			   "1144000 26\n"
			   "1014000 10\n"
			   "902000 4\n"
			   "839000 52\n"
			   "757000 4\n"
			   "676000 2\n"
			   "546000 8\n"
			   "449000 8\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/cpufreq/stats/total_trans",
		.size = 3,
		.content = "94\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/core_id",
		.size = 2,
		.content = "5\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/core_siblings",
		.size = 3,
		.content = "3f\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu5/topology/core_siblings_list",
		.size = 4,
		.content = "0-5\n",
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
		.size = 12,
		.content = "exynos_idle\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/affected_cpus",
		.size = 4,
		.content = "6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "2184000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "936000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/related_cpus",
		.size = 4,
		.content = "6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_available_governors",
		.size = 35,
		.content = "userspace interactive performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "936000\n",
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
		.content = "2184000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "936000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/stats/time_in_state",
		.size = 119,
		.content = "2184000 5049\n"
			   "2080000 70\n"
			   "1976000 93\n"
			   "1872000 287\n"
			   "1768000 190\n"
			   "1664000 195\n"
			   "1560000 104\n"
			   "1352000 378\n"
			   "1144000 164\n"
			   "936000 3414\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/cpufreq/stats/total_trans",
		.size = 4,
		.content = "409\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/core_id",
		.size = 2,
		.content = "0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/core_siblings",
		.size = 3,
		.content = "c0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu6/topology/core_siblings_list",
		.size = 4,
		.content = "6-7\n",
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
		.size = 4,
		.content = "6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_max_freq",
		.size = 8,
		.content = "2184000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_min_freq",
		.size = 7,
		.content = "936000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_transition_latency",
		.size = 7,
		.content = "100000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/related_cpus",
		.size = 4,
		.content = "6 7\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_available_governors",
		.size = 35,
		.content = "userspace interactive performance \n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_cur_freq",
		.size = 7,
		.content = "936000\n",
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
		.content = "2184000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/scaling_min_freq",
		.size = 7,
		.content = "936000\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/stats/time_in_state",
		.size = 119,
		.content = "2184000 5049\n"
			   "2080000 70\n"
			   "1976000 93\n"
			   "1872000 293\n"
			   "1768000 194\n"
			   "1664000 203\n"
			   "1560000 104\n"
			   "1352000 382\n"
			   "1144000 164\n"
			   "936000 3646\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/cpufreq/stats/total_trans",
		.size = 4,
		.content = "416\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/core_id",
		.size = 2,
		.content = "1\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/core_siblings",
		.size = 3,
		.content = "c0\n",
	},
	{
		.path = "/sys/devices/system/cpu/cpu7/topology/core_siblings_list",
		.size = 4,
		.content = "6-7\n",
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
		.key = "dalvik.vm.dex2oat-filter",
		.value = "speed",
	},
	{
		.key = "dalvik.vm.heapgrowthlimit",
		.value = "192m",
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
		.key = "dalvik.vm.image-dex2oat-filter",
		.value = "speed",
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
		.key = "debug.sf.framedrop",
		.value = "0",
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
		.key = "dev.mtp.opensession",
		.value = "0",
	},
	{
		.key = "dev.ssrm.appsync3p",
		.value = "true",
	},
	{
		.key = "dev.ssrm.gamelevel",
		.value = "-4,5,-1,3",
	},
	{
		.key = "dev.ssrm.init",
		.value = "1",
	},
	{
		.key = "dev.ssrm.live_thumbnail",
		.value = "1",
	},
	{
		.key = "dev.ssrm.mode",
		.value = "dm;",
	},
	{
		.key = "dev.ssrm.pst",
		.value = "365",
	},
	{
		.key = "dev.ssrm.smart_switch",
		.value = "false",
	},
	{
		.key = "dev.usbsetting.embedded",
		.value = "on",
	},
	{
		.key = "drm.service.enabled",
		.value = "true",
	},
	{
		.key = "exynos.modem.logsympath",
		.value = "/dev/block/sda6",
	},
	{
		.key = "exynos.modempath",
		.value = "/dev/block/sda5",
	},
	{
		.key = "gsm.current.phone-type",
		.value = "1,1",
	},
	{
		.key = "gsm.network.type",
		.value = "Unknown,Unknown",
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
		.value = "false,false",
	},
	{
		.key = "gsm.operator.isroaming",
		.value = "false,false",
	},
	{
		.key = "gsm.operator.numeric",
		.value = "310260",
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
		.key = "gsm.version.baseband",
		.value = "A530FXXU1ARA9",
	},
	{
		.key = "gsm.version.baseband1",
		.value = "A530FXXU1ARA9",
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
		.key = "init.svc.jackservice",
		.value = "running",
	},
	{
		.key = "init.svc.keystore",
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
		.key = "init.svc.remosaic_daemon",
		.value = "running",
	},
	{
		.key = "init.svc.remotedisplay",
		.value = "running",
	},
	{
		.key = "init.svc.ril-daemon",
		.value = "running",
	},
	{
		.key = "init.svc.ril-daemon1",
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
		.value = "android-c9f7806e78a58393",
	},
	{
		.key = "net.iptype",
		.value = "502:v4v6v6",
	},
	{
		.key = "net.knox.shareddevice.version",
		.value = "2.9.0",
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
		.key = "nfc.boot.reason",
		.value = "1",
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
		.value = "5",
	},
	{
		.key = "nfc.fw.rfreg_ver",
		.value = "17/10/31/14.46.44",
	},
	{
		.key = "nfc.fw.ver",
		.value = "S.LSI 4.1.13",
	},
	{
		.key = "nfc.product.support.ese",
		.value = "1",
	},
	{
		.key = "nfc.product.support.uicc",
		.value = "1",
	},
	{
		.key = "persist.app.permission.monitor",
		.value = "1048647",
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
		.key = "persist.audio.deepbuffer_delay",
		.value = "99",
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
		.key = "persist.audio.pcmdump",
		.value = "0",
	},
	{
		.key = "persist.audio.ringermode",
		.value = "2",
	},
	{
		.key = "persist.audio.sales_code",
		.value = "XSG",
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
		.value = "f/w: O|2110|2017-12-15 15:14:39 UTC|lassen_s612_lassen_a5_rev02_2017_07)",
	},
	{
		.key = "persist.demo.hdmirotationlock",
		.value = "false",
	},
	{
		.key = "persist.ims.dsds.end_ipc",
		.value = "true",
	},
	{
		.key = "persist.ims.dsds.start_ipc",
		.value = "true",
	},
	{
		.key = "persist.nfc.log.index",
		.value = "4",
	},
	{
		.key = "persist.radio.embms.support",
		.value = "false",
	},
	{
		.key = "persist.radio.latest-modeltype",
		.value = "2,2",
	},
	{
		.key = "persist.radio.multisim.config",
		.value = "dsds",
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
		.key = "persist.ril.ims.utranParam",
		.value = "0",
	},
	{
		.key = "persist.ril.modem.board",
		.value = "SHANNON327",
	},
	{
		.key = "persist.ril.modem.board2",
		.value = "SHANNON327",
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
		.value = "Fri Jan 12 19:09:06 KST 2018",
	},
	{
		.key = "persist.sys.clssprld2",
		.value = "354",
	},
	{
		.key = "persist.sys.clssprld3",
		.value = "1211",
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
		.key = "persist.sys.enablehomekey",
		.value = "false",
	},
	{
		.key = "persist.sys.ims.supportmmtel1",
		.value = "0",
	},
	{
		.key = "persist.sys.ims.supportmmtel2",
		.value = "0",
	},
	{
		.key = "persist.sys.locale",
		.value = "en-US",
	},
	{
		.key = "persist.sys.localedefault",
		.value = "en-GB",
	},
	{
		.key = "persist.sys.omc.enable",
		.value = "true",
	},
	{
		.key = "persist.sys.omc_etcpath",
		.value = "/system/omc/XSG/etc",
	},
	{
		.key = "persist.sys.omc_path",
		.value = "/system/omc/XSG",
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
		.value = "/system/omc/XSG",
	},
	{
		.key = "persist.sys.ppr",
		.value = "true",
	},
	{
		.key = "persist.sys.prev_omcnwcode",
		.value = "XSG,0",
	},
	{
		.key = "persist.sys.prev_salescode",
		.value = "XSG,0",
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
		.key = "persist.sys.sm_mode",
		.value = "1",
	},
	{
		.key = "persist.sys.storage_preload",
		.value = "2",
	},
	{
		.key = "persist.sys.timezone",
		.value = "Asia/Muscat",
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
		.key = "persist.sys.vzw_wifi_running",
		.value = "false",
	},
	{
		.key = "persist.sys.webview.vmsize",
		.value = "113140720",
	},
	{
		.key = "persist.sys.zygote.early",
		.value = "true",
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
		.value = "0,0",
	},
	{
		.key = "ril.ICC_TYPE0",
		.value = "0",
	},
	{
		.key = "ril.ICC_TYPE1",
		.value = "0",
	},
	{
		.key = "ril.LIMA",
		.value = "40491",
	},
	{
		.key = "ril.RildInit",
		.value = "1,1",
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
		.key = "ril.callcount",
		.value = "0",
	},
	{
		.key = "ril.cbd.boot_done",
		.value = "1",
	},
	{
		.key = "ril.cbd.dt_revision",
		.value = "007",
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
		.key = "ril.dds.socket_num",
		.value = "-1",
	},
	{
		.key = "ril.debug_modemfactory",
		.value = "CSC Feature State: IMS ON, EPDG OFF",
	},
	{
		.key = "ril.ecclist00",
		.value = "112,911,999,000,110,118,119",
	},
	{
		.key = "ril.ecclist10",
		.value = "112,911,999,000,08,110,118,119",
	},
	{
		.key = "ril.ecclist_net0",
		.value = "",
	},
	{
		.key = "ril.hasisim",
		.value = "0,0",
	},
	{
		.key = "ril.hw_ver",
		.value = "REV0.3",
	},
	{
		.key = "ril.hw_ver2",
		.value = "REV0.3",
	},
	{
		.key = "ril.ims.ecsupport",
		.value = "2,2",
	},
	{
		.key = "ril.ims.ltevoicesupport",
		.value = "2,2",
	},
	{
		.key = "ril.initPB",
		.value = "0",
	},
	{
		.key = "ril.initPB2",
		.value = "0",
	},
	{
		.key = "ril.iscdma",
		.value = "0",
	},
	{
		.key = "ril.model_id",
		.value = "QB8682035",
	},
	{
		.key = "ril.model_id2",
		.value = "QB8682035",
	},
	{
		.key = "ril.modem.board",
		.value = "SHANNON327",
	},
	{
		.key = "ril.modem.board2",
		.value = "SHANNON327",
	},
	{
		.key = "ril.official_cscver",
		.value = "A530FOJM1ARA2",
	},
	{
		.key = "ril.product_code",
		.value = "SM-A530FZKGXSG",
	},
	{
		.key = "ril.product_code2",
		.value = "SM-A530FZKGXSG",
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
		.key = "ril.region_props0",
		.value = "XSG.UAE.AE.XSG",
	},
	{
		.key = "ril.region_props1",
		.value = "XSG.UAE.AE.XSG",
	},
	{
		.key = "ril.rfcal_date",
		.value = "20180117",
	},
	{
		.key = "ril.rfcal_date2",
		.value = "20180117",
	},
	{
		.key = "ril.serialnumber",
		.value = "R58K13VTFRN",
	},
	{
		.key = "ril.servicestate",
		.value = "2,1",
	},
	{
		.key = "ril.simoperator",
		.value = ",",
	},
	{
		.key = "ril.ss.routing",
		.value = "0,0",
	},
	{
		.key = "ril.subinfo",
		.value = "0:-2,1:-3",
	},
	{
		.key = "ril.sw_ver",
		.value = "A530FXXU1ARA9",
	},
	{
		.key = "ril.sw_ver2",
		.value = "A530FXXU1ARA9",
	},
	{
		.key = "ril.timezoneID",
		.value = "",
	},
	{
		.key = "ril.twwan911Timer",
		.value = "0",
	},
	{
		.key = "ril.voice.rat",
		.value = "3",
	},
	{
		.key = "rild.libargs",
		.value = "-d /dev/umts_ipc0",
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
		.value = "exynos7885",
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
		.key = "ro.board.platform",
		.value = "exynos5",
	},
	{
		.key = "ro.boot.ap_serial",
		.value = "0x0774EF5052E4",
	},
	{
		.key = "ro.boot.bootloader",
		.value = "A530FXXU1ARA3",
	},
	{
		.key = "ro.boot.carrierid",
		.value = "XSG",
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
		.key = "ro.boot.dram_info",
		.value = "01,06,01,4G",
	},
	{
		.key = "ro.boot.em.did",
		.value = "0774ef5052e4",
	},
	{
		.key = "ro.boot.em.model",
		.value = "SM-A530F",
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
		.key = "ro.boot.hardware",
		.value = "samsungexynos7885",
	},
	{
		.key = "ro.boot.hmac_mismatch",
		.value = "0",
	},
	{
		.key = "ro.boot.hw_rev",
		.value = "7",
	},
	{
		.key = "ro.boot.odin_download",
		.value = "1",
	},
	{
		.key = "ro.boot.prototype.param.offset",
		.value = "7345920",
	},
	{
		.key = "ro.boot.recovery_offset",
		.value = "7347968",
	},
	{
		.key = "ro.boot.sales.param.offset",
		.value = "7340572",
	},
	{
		.key = "ro.boot.sec_atd.tty",
		.value = "/dev/ttySAC2",
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
		.value = "52001f82ee5db461",
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
		.value = "Fri Jan 12 19:09:06 KST 2018",
	},
	{
		.key = "ro.bootimage.build.date.utc",
		.value = "1515751746",
	},
	{
		.key = "ro.bootimage.build.fingerprint",
		.value = "samsung/jackpotltexx/jackpotlte:7.1.1/NMF26X/A530FXXU1ARA3:user/test-keys",
	},
	{
		.key = "ro.bootloader",
		.value = "A530FXXU1ARA3",
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
		.value = "A530FXXU1ARA3",
	},
	{
		.key = "ro.build.changelist",
		.value = "12719828",
	},
	{
		.key = "ro.build.characteristics",
		.value = "phone,emulator",
	},
	{
		.key = "ro.build.date",
		.value = "Fri Jan 12 19:09:06 KST 2018",
	},
	{
		.key = "ro.build.date.utc",
		.value = "1515751746",
	},
	{
		.key = "ro.build.description",
		.value = "jackpotltexx-user 7.1.1 NMF26X A530FXXU1ARA3 release-keys",
	},
	{
		.key = "ro.build.display.id",
		.value = "NMF26X.A530FXXU1ARA3",
	},
	{
		.key = "ro.build.fingerprint",
		.value = "samsung/jackpotltexx/jackpotlte:7.1.1/NMF26X/A530FXXU1ARA3:user/release-keys",
	},
	{
		.key = "ro.build.flavor",
		.value = "jackpotltexx-user",
	},
	{
		.key = "ro.build.host",
		.value = "SWHE8816",
	},
	{
		.key = "ro.build.id",
		.value = "NMF26X",
	},
	{
		.key = "ro.build.official.release",
		.value = "true",
	},
	{
		.key = "ro.build.product",
		.value = "jackpotlte",
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
		.value = "A530FXXU1ARA3",
	},
	{
		.key = "ro.build.version.preview_sdk",
		.value = "0",
	},
	{
		.key = "ro.build.version.release",
		.value = "7.1.1",
	},
	{
		.key = "ro.build.version.sdk",
		.value = "25",
	},
	{
		.key = "ro.build.version.security_index",
		.value = "1",
	},
	{
		.key = "ro.build.version.security_patch",
		.value = "2018-01-01",
	},
	{
		.key = "ro.build.version.sem",
		.value = "2502",
	},
	{
		.key = "ro.build.version.sep",
		.value = "80500",
	},
	{
		.key = "ro.carrier",
		.value = "unknown",
	},
	{
		.key = "ro.carrierid",
		.value = "XSG",
	},
	{
		.key = "ro.carrierid.param.offset",
		.value = "7340608",
	},
	{
		.key = "ro.cfg.dha_cached_max",
		.value = "24",
	},
	{
		.key = "ro.cfg.dha_cached_min",
		.value = "8",
	},
	{
		.key = "ro.cfg.dha_empty_init",
		.value = "32",
	},
	{
		.key = "ro.cfg.dha_empty_max",
		.value = "32",
	},
	{
		.key = "ro.cfg.dha_empty_min",
		.value = "8",
	},
	{
		.key = "ro.cfg.dha_lmk_scale",
		.value = "1",
	},
	{
		.key = "ro.cfg.dha_th_rate",
		.value = "2.0",
	},
	{
		.key = "ro.chipname",
		.value = "exynos7885",
	},
	{
		.key = "ro.com.google.clientidbase",
		.value = "android-samsung-ss",
	},
	{
		.key = "ro.com.google.clientidbase.ms",
		.value = "android-samsung-ga-rev1",
	},
	{
		.key = "ro.com.google.gmsversion",
		.value = "7.1_r8",
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
		.value = "4",
	},
	{
		.key = "ro.config.dha_empty_init",
		.value = "16",
	},
	{
		.key = "ro.config.dha_empty_max",
		.value = "16",
	},
	{
		.key = "ro.config.dha_empty_min",
		.value = "8",
	},
	{
		.key = "ro.config.dha_lmk_scale",
		.value = "0.583",
	},
	{
		.key = "ro.config.dha_pwhitelist_enable",
		.value = "1",
	},
	{
		.key = "ro.config.dha_pwhl_key",
		.value = "96",
	},
	{
		.key = "ro.config.dha_th_rate",
		.value = "2.6",
	},
	{
		.key = "ro.config.dmverity",
		.value = "true",
	},
	{
		.key = "ro.config.iccc_version",
		.value = "1.0",
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
		.value = "UAE",
	},
	{
		.key = "ro.csc.countryiso_code",
		.value = "AE",
	},
	{
		.key = "ro.csc.facebook.partnerid",
		.value = "samsung:dec1cc9c-1497-4aab-b953-cee702c2a481",
	},
	{
		.key = "ro.csc.omcnw_code",
		.value = "XSG",
	},
	{
		.key = "ro.csc.sales_code",
		.value = "XSG",
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
		.value = "0xae1c9693749cb6862cd35307ec060ab105079a77000000000000000000000000",
	},
	{
		.key = "ro.fmp_config",
		.value = "unknown",
	},
	{
		.key = "ro.frp.pst",
		.value = "/dev/block/persistent",
	},
	{
		.key = "ro.hardware",
		.value = "samsungexynos7885",
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
		.value = "34",
	},
	{
		.key = "ro.hwui.path_cache_size",
		.value = "10",
	},
	{
		.key = "ro.hwui.r_buffer_cache_size",
		.value = "4",
	},
	{
		.key = "ro.hwui.shape_cache_size",
		.value = "4",
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
		.value = "50",
	},
	{
		.key = "ro.im.param.offset",
		.value = "unknown",
	},
	{
		.key = "ro.kernel.qemu",
		.value = "0",
	},
	{
		.key = "ro.ldu",
		.value = "unknown",
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
		.value = "2",
	},
	{
		.key = "ro.nfc.port",
		.value = "I2C",
	},
	{
		.key = "ro.oem.key1",
		.value = "XSG",
	},
	{
		.key = "ro.oem_unlock_supported",
		.value = "1",
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
		.value = "universal7885",
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
		.value = "jackpotlte",
	},
	{
		.key = "ro.product.first_api_level",
		.value = "25",
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
		.value = "SM-A530F",
	},
	{
		.key = "ro.product.name",
		.value = "jackpotltexx",
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
		.value = "7",
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
		.value = "1524185862623",
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
		.key = "ro.security.ese.cosname",
		.value = "UT4.0_0100000C",
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
		.key = "ro.security.keystore.keytype",
		.value = "sak",
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
		.key = "ro.security_mode",
		.value = "1526595585",
	},
	{
		.key = "ro.serialno",
		.value = "52001f82ee5db461",
	},
	{
		.key = "ro.setupwizard.mode",
		.value = "OPTIONAL",
	},
	{
		.key = "ro.setupwizard.wifi_on_exit",
		.value = "false",
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
		.key = "security.mdpp.mass",
		.value = "skmm",
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
		.value = "SEPF_SECMOBILE_7.1.1_0004",
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
		.value = "62537072640",
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
		.key = "sys.bartender.batterystats.ver",
		.value = "17",
	},
	{
		.key = "sys.boot.enable_screen",
		.value = "1",
	},
	{
		.key = "sys.boot.end_package",
		.value = "1",
	},
	{
		.key = "sys.boot.end_preload",
		.value = "1",
	},
	{
		.key = "sys.boot.loop_forever",
		.value = "1",
	},
	{
		.key = "sys.boot.start_package",
		.value = "1",
	},
	{
		.key = "sys.boot.start_preload",
		.value = "1",
	},
	{
		.key = "sys.boot.zygote_ready",
		.value = "1",
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
		.key = "sys.config.activelaunch_enable",
		.value = "true",
	},
	{
		.key = "sys.config.mars_version",
		.value = "2.10",
	},
	{
		.key = "sys.config.phone_start_early",
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
		.key = "sys.dualapp.profile_id",
		.value = "",
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
		.value = "UNKOWN",
	},
	{
		.key = "sys.gps.chipvendor",
		.value = "S.LSI",
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
		.value = "",
	},
	{
		.key = "sys.siop.level",
		.value = "0",
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
		.key = "wlan.wfd.screenratiovalue",
		.value = "true",
	},
	{
		.key = "wlan.wfd.status",
		.value = "disconnected",
	},
	{NULL},
};
#endif /* __ANDROID__ */
