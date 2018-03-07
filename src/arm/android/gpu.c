#include <linux/api.h>
#include <arm/android/api.h>


#define CPUINFO_COUNT_OF(x) (sizeof(x) / sizeof(0[x]))

struct cpuinfo_chipset_gpu_entry {
	uint8_t chipset_series;
	char chipset_suffix;
	uint16_t chipset_model;
	uint8_t gpu_series;
	char gpu_suffix;
	uint16_t gpu_model;
};

/*
 * Table of Android GPUs for different chipsets.
 */
static const struct cpuinfo_chipset_gpu_entry chipset_gpu_map[] = {
	{
		/* Actions ATM7029 -> Vivante GC1000 */
		.chipset_series = cpuinfo_arm_chipset_series_actions_atm,
		.chipset_model = 7029,
		.gpu_series = cpuinfo_android_gpu_series_vivante_gc,
		.gpu_model = 1000,
	},
	{
		/* Actions ATM7029B -> PowerVR SGX 540 */
		.chipset_series = cpuinfo_arm_chipset_series_actions_atm,
		.chipset_model = 7029,
		.chipset_suffix = 'B',
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 540,
	},
	{
		/* Allwinner A10 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_allwinner_a,
		.chipset_model = 10,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Allwinner A13 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_allwinner_a,
		.chipset_model = 13,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Allwinner A20 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_allwinner_a,
		.chipset_model = 20,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Allwinner A23 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_allwinner_a,
		.chipset_model = 23,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Allwinner A31 -> PowerVR SGX 544 */
		.chipset_series = cpuinfo_arm_chipset_series_allwinner_a,
		.chipset_model = 31,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 544,
	},
	{
		/* Allwinner A33 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_allwinner_a,
		.chipset_model = 33,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Allwinner A80 -> PowerVR Rogue G6230 */
		.chipset_series = cpuinfo_arm_chipset_series_allwinner_a,
		.chipset_model = 80,
		.gpu_series = cpuinfo_android_gpu_series_powervr_rogue_g,
		.gpu_model = 6230,
	},
	{
		/* Allwinner A83T -> PowerVR SGX 544 */
		.chipset_series = cpuinfo_arm_chipset_series_allwinner_a,
		.chipset_model = 83,
		.chipset_suffix = 'T',
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 544,
	},
	{
		/* Amlogic AML8726* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_amlogic_aml,
		.chipset_model = 8726,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Amlogic S805 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_amlogic_s,
		.chipset_model = 805,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* Amlogic S812 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_amlogic_s,
		.chipset_model = 812,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* Amlogic S905* -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_amlogic_s,
		.chipset_model = 905,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* Amlogic S912 -> ARM Mali-T820 */
		.chipset_series = cpuinfo_arm_chipset_series_amlogic_s,
		.chipset_model = 912,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 820,
	},
	{
		/* Broadcom BCM21654* -> Broadcom VideoCore IV */
		.chipset_series = cpuinfo_arm_chipset_series_broadcom_bcm,
		.chipset_model = 21654,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_broadcom_videocore,
		.gpu_model = 4,
	},
	{
		/* Broadcom BCM21663 -> Broadcom VideoCore IV */
		.chipset_series = cpuinfo_arm_chipset_series_broadcom_bcm,
		.chipset_model = 21663,
		.gpu_series = cpuinfo_android_gpu_series_broadcom_videocore,
		.gpu_model = 4,
	},
	{
		/* Broadcom BCM21664* -> Broadcom VideoCore IV */
		.chipset_series = cpuinfo_arm_chipset_series_broadcom_bcm,
		.chipset_model = 21664,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_broadcom_videocore,
		.gpu_model = 4,
	},
	{
		/* Broadcom BCM23550 -> Broadcom VideoCore IV */
		.chipset_series = cpuinfo_arm_chipset_series_broadcom_bcm,
		.chipset_model = 23550,
		.gpu_series = cpuinfo_android_gpu_series_broadcom_videocore,
		.gpu_model = 4,
	},
	{
		/* Broadcom BCM28155 -> Broadcom VideoCore IV */
		.chipset_series = cpuinfo_arm_chipset_series_broadcom_bcm,
		.chipset_model = 28155,
		.gpu_series = cpuinfo_android_gpu_series_broadcom_videocore,
		.gpu_model = 4,
	},
	{
		/* HiSilicon Hi3751 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_hi,
		.chipset_model = 3751,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* HiSilicon K3V2 -> Vivante GC4000 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_k3v,
		.chipset_model = 2,
		.gpu_series = cpuinfo_android_gpu_series_vivante_gc,
		.gpu_model = 4000,
	},
	{
		/* HiSilicon Kirin 620 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 620,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* HiSilicon Kirin 650 -> ARM Mali-T830 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 650,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 830,
	},
	{
		/* HiSilicon Kirin 659 -> ARM Mali-T830 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 659,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 830,
	},
	{
		/* HiSilicon Kirin 910T -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 910,
		.chipset_suffix = 'T',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* HiSilicon Kirin 920 -> ARM Mali-T624 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 920,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 624,
	},
	{
		/* HiSilicon Kirin 925 -> ARM Mali-T624 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 925,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 624,
	},
	{
		/* HiSilicon Kirin 930 -> ARM Mali-T624 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 930,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 624,
	},
	{
		/* HiSilicon Kirin 935 -> ARM Mali-T624 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 935,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 624,
	},
	{
		/* HiSilicon Kirin 950 -> ARM Mali-T880 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 950,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 880,
	},
	{
		/* HiSilicon Kirin 955 -> ARM Mali-T880 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 955,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 880,
	},
	{
		/* HiSilicon Kirin 960 -> ARM Mali-G71 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 960,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_g,
		.gpu_model = 71,
	},
	{
		/* HiSilicon Kirin 970 -> ARM Mali-G72 */
		.chipset_series = cpuinfo_arm_chipset_series_hisilicon_kirin,
		.chipset_model = 970,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_g,
		.gpu_model = 72,
	},
	{
		/* LG Nuclun 7111 -> PowerVR Rogue G6430 */
		.chipset_series = cpuinfo_arm_chipset_series_lg_nuclun,
		.chipset_model = 7111,
		.gpu_series = cpuinfo_android_gpu_series_powervr_rogue_g,
		.gpu_model = 6430,
	},
	{
		/* Leadcore LC1860 -> ARM Mali-T622 */
		.chipset_series = cpuinfo_arm_chipset_series_leadcore_lc,
		.chipset_model = 1860,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 622,
	},
	{
		/* MStar 6A338 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_mstar_6a,
		.chipset_model = 338,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* Marvell PXA1088 -> Vivante GC1000 */
		.chipset_series = cpuinfo_arm_chipset_series_marvell_pxa,
		.chipset_model = 1088,
		.gpu_series = cpuinfo_android_gpu_series_vivante_gc,
		.gpu_model = 1000,
	},
	{
		/* Marvell PXA1908 -> Vivante GC7000UL */
		.chipset_series = cpuinfo_arm_chipset_series_marvell_pxa,
		.chipset_model = 1908,
		.gpu_series = cpuinfo_android_gpu_series_vivante_gc,
		.gpu_model = 7000,
		.gpu_suffix = 'U',
	},
	{
		/* Marvell PXA1928 -> Vivante GC5000 */
		.chipset_series = cpuinfo_arm_chipset_series_marvell_pxa,
		.chipset_model = 1928,
		.gpu_series = cpuinfo_android_gpu_series_vivante_gc,
		.gpu_model = 5000,
	},
	{
		/* Marvell PXA986 -> Vivante GC1000 */
		.chipset_series = cpuinfo_arm_chipset_series_marvell_pxa,
		.chipset_model = 986,
		.gpu_series = cpuinfo_android_gpu_series_vivante_gc,
		.gpu_model = 1000,
	},
	{
		/* Marvell PXA988 -> Vivante GC1000 */
		.chipset_series = cpuinfo_arm_chipset_series_marvell_pxa,
		.chipset_model = 988,
		.gpu_series = cpuinfo_android_gpu_series_vivante_gc,
		.gpu_model = 1000,
	},
	{
		/* MediaTek MT5507 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 5507,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* MediaTek MT5508 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 5508,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* MediaTek MT5863 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 5863,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* MediaTek MT5891 -> ARM Mali-T860 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 5891,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 860,
	},
	{
		/* MediaTek MT6517 -> PowerVR SGX 531 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6517,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 531,
	},
	{
		/* MediaTek MT6570 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6570,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* MediaTek MT6571 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6571,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* MediaTek MT6572* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6572,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* MediaTek MT6575 -> PowerVR SGX 531 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6575,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 531,
	},
	{
		/* MediaTek MT6580* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6580,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* MediaTek MT6581 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6581,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* MediaTek MT6582* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6582,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* MediaTek MT6588 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6588,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* MediaTek MT6591 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6591,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* MediaTek MT6732* -> ARM Mali-T760 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6732,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 760,
	},
	{
		/* MediaTek MT6735* -> ARM Mali-T720 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6735,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 720,
	},
	{
		/* MediaTek MT6737* -> ARM Mali-T720 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6737,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 720,
	},
	{
		/* MediaTek MT6738 -> ARM Mali-T860 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6738,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 860,
	},
	{
		/* MediaTek MT6750* -> ARM Mali-T860 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6750,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 860,
	},
	{
		/* MediaTek MT6752* -> ARM Mali-T760 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6752,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 760,
	},
	{
		/* MediaTek MT6753* -> ARM Mali-T720 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6753,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 720,
	},
	{
		/* MediaTek MT6755* -> ARM Mali-T860 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6755,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 860,
	},
	{
		/* MediaTek MT6757* -> ARM Mali-T880 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6757,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 880,
	},
	{
		/* MediaTek MT6758* -> ARM Mali-G71 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6758,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_g,
		.gpu_model = 71,
	},
	{
		/* MediaTek MT6763 -> ARM Mali-G71 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6763,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_g,
		.gpu_model = 71,
	},
	{
		/* MediaTek MT6795* -> PowerVR Rogue G6200 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6795,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_powervr_rogue_g,
		.gpu_model = 6200,
	},
	{
		/* MediaTek MT6797* -> ARM Mali-T880 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6797,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 880,
	},
	{
		/* MediaTek MT6799 -> PowerVR Rogue GT7400+ */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 6799,
		.gpu_series = cpuinfo_android_gpu_series_powervr_rogue_gt,
		.gpu_model = 7400,
		.gpu_suffix = '+',
	},
	{
		/* MediaTek MT8111 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8111,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* MediaTek MT8121 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8121,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* MediaTek MT8125 -> PowerVR SGX 544 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8125,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 544,
	},
	{
		/* MediaTek MT8127 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8127,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* MediaTek MT8135 -> PowerVR Rogue G6200 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8135,
		.gpu_series = cpuinfo_android_gpu_series_powervr_rogue_g,
		.gpu_model = 6200,
	},
	{
		/* MediaTek MT8151 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8151,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* MediaTek MT8161* -> ARM Mali-T720 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8161,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 720,
	},
	{
		/* MediaTek MT8163 -> ARM Mali-T720 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8163,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 720,
	},
	{
		/* MediaTek MT8165 -> ARM Mali-T760 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8165,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 760,
	},
	{
		/* MediaTek MT8167* -> PowerVR Rogue GE8300 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8167,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_powervr_rogue_ge,
		.gpu_model = 8300,
	},
	{
		/* MediaTek MT8173 -> PowerVR Rogue GX6250 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8173,
		.gpu_series = cpuinfo_android_gpu_series_powervr_rogue_gx,
		.gpu_model = 6250,
	},
	{
		/* MediaTek MT8176 -> PowerVR Rogue GX6250 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8176,
		.gpu_series = cpuinfo_android_gpu_series_powervr_rogue_gx,
		.gpu_model = 6250,
	},
	{
		/* MediaTek MT8312* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8312,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* MediaTek MT8317 -> PowerVR SGX 531 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8317,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 531,
	},
	{
		/* MediaTek MT8321* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8321,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* MediaTek MT8377 -> PowerVR SGX 531 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8377,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 531,
	},
	{
		/* MediaTek MT8382 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8382,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* MediaTek MT8389 -> PowerVR SGX 544 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8389,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 544,
	},
	{
		/* MediaTek MT8389Q -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8389,
		.chipset_suffix = 'Q',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* MediaTek MT8392 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8392,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* MediaTek MT8685 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8685,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* MediaTek MT8732* -> ARM Mali-T760 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8732,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 760,
	},
	{
		/* MediaTek MT8735* -> ARM Mali-T720 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8735,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 720,
	},
	{
		/* MediaTek MT8783* -> ARM Mali-T720 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8783,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 720,
	},
	{
		/* MediaTek MT8785WA -> ARM Mali-T860 */
		.chipset_series = cpuinfo_arm_chipset_series_mediatek_mt,
		.chipset_model = 8785,
		.chipset_suffix = 'W',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 860,
	},
	{
		/* NovaThor U8500 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_novathor_u,
		.chipset_model = 8500,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Nvidia Tegra AP20H -> Nvidia Tegra 2 */
		.chipset_series = cpuinfo_arm_chipset_series_nvidia_tegra_ap,
		.chipset_model = 20,
		.chipset_suffix = 'H',
		.gpu_series = cpuinfo_android_gpu_series_nvidia_tegra,
		.gpu_model = 2,
	},
	{
		/* Nvidia Tegra AP33 -> Nvidia Tegra 3 */
		.chipset_series = cpuinfo_arm_chipset_series_nvidia_tegra_ap,
		.chipset_model = 33,
		.gpu_series = cpuinfo_android_gpu_series_nvidia_tegra,
		.gpu_model = 3,
	},
	{
		/* Nvidia Tegra SL460N -> Nvidia Tegra 3 */
		.chipset_series = cpuinfo_arm_chipset_series_nvidia_tegra_sl,
		.chipset_model = 460,
		.chipset_suffix = 'N',
		.gpu_series = cpuinfo_android_gpu_series_nvidia_tegra,
		.gpu_model = 3,
	},
	{
		/* Nvidia Tegra T114 -> Nvidia Tegra 4 */
		.chipset_series = cpuinfo_arm_chipset_series_nvidia_tegra_t,
		.chipset_model = 114,
		.gpu_series = cpuinfo_android_gpu_series_nvidia_tegra,
		.gpu_model = 4,
	},
	{
		/* Nvidia Tegra T124 -> Nvidia Tegra K1 */
		.chipset_series = cpuinfo_arm_chipset_series_nvidia_tegra_t,
		.chipset_model = 124,
		.gpu_series = cpuinfo_android_gpu_series_nvidia_tegra_k,
		.gpu_model = 1,
	},
	{
		/* Nvidia Tegra T132 -> Nvidia Tegra K1 */
		.chipset_series = cpuinfo_arm_chipset_series_nvidia_tegra_t,
		.chipset_model = 132,
		.gpu_series = cpuinfo_android_gpu_series_nvidia_tegra_k,
		.gpu_model = 1,
	},
	{
		/* Nvidia Tegra T20 -> Nvidia Tegra 2 */
		.chipset_series = cpuinfo_arm_chipset_series_nvidia_tegra_t,
		.chipset_model = 20,
		.gpu_series = cpuinfo_android_gpu_series_nvidia_tegra,
		.gpu_model = 2,
	},
	{
		/* Nvidia Tegra T210 -> Nvidia Tegra X1 */
		.chipset_series = cpuinfo_arm_chipset_series_nvidia_tegra_t,
		.chipset_model = 210,
		.gpu_series = cpuinfo_android_gpu_series_nvidia_tegra_x,
		.gpu_model = 1,
	},
	{
		/* Nvidia Tegra T30* -> Nvidia Tegra 3 */
		.chipset_series = cpuinfo_arm_chipset_series_nvidia_tegra_t,
		.chipset_model = 30,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_nvidia_tegra,
		.gpu_model = 3,
	},
	{
		/* Nvidia Tegra T33 -> Nvidia Tegra 3 */
		.chipset_series = cpuinfo_arm_chipset_series_nvidia_tegra_t,
		.chipset_model = 33,
		.gpu_series = cpuinfo_android_gpu_series_nvidia_tegra,
		.gpu_model = 3,
	},
	{
		/* Pinecone Surge S1 -> ARM Mali-T860 */
		.chipset_series = cpuinfo_arm_chipset_series_pinecone_surge_s,
		.chipset_model = 1,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 860,
	},
	{
		/* Qualcomm APQ8009 -> Qualcomm Adreno 304 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8009,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 304,
	},
	{
		/* Qualcomm APQ8016 -> Qualcomm Adreno 306 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8016,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 306,
	},
	{
		/* Qualcomm APQ8017 -> Qualcomm Adreno 308 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8017,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 308,
	},
	{
		/* Qualcomm APQ8026 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8026,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm APQ8028 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8028,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm APQ8039 -> Qualcomm Adreno 405 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8039,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 405,
	},
	{
		/* Qualcomm APQ8053 -> Qualcomm Adreno 506 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8053,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 506,
	},
	{
		/* Qualcomm APQ8064* -> Qualcomm Adreno 320 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8064,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 320,
	},
	{
		/* Qualcomm APQ8074* -> Qualcomm Adreno 330 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8074,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 330,
	},
	{
		/* Qualcomm APQ8076 -> Qualcomm Adreno 510 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8076,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 510,
	},
	{
		/* Qualcomm APQ8084 -> Qualcomm Adreno 420 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8084,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 420,
	},
	{
		/* Qualcomm APQ8094 -> Qualcomm Adreno 430 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_apq,
		.chipset_model = 8094,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 430,
	},
	{
		/* Qualcomm MSM7225AB -> Qualcomm Adreno 200 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 7225,
		.chipset_suffix = 'A',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 200,
	},
	{
		/* Qualcomm MSM7625A -> Qualcomm Adreno 200 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 7625,
		.chipset_suffix = 'A',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 200,
	},
	{
		/* Qualcomm MSM7630 -> Qualcomm Adreno 205 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 7630,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 205,
	},
	{
		/* Qualcomm MSM8208 -> Qualcomm Adreno 304 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8208,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 304,
	},
	{
		/* Qualcomm MSM8209 -> Qualcomm Adreno 304 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8209,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 304,
	},
	{
		/* Qualcomm MSM8210 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8210,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8212 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8212,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8225 -> Qualcomm Adreno 203 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8225,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 203,
	},
	{
		/* Qualcomm MSM8226 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8226,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8228 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8228,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8230* -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8230,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8239 -> Qualcomm Adreno 405 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8239,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 405,
	},
	{
		/* Qualcomm MSM8260A -> Qualcomm Adreno 225 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8260,
		.chipset_suffix = 'A',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 225,
	},
	{
		/* Qualcomm MSM8274 -> Qualcomm Adreno 330 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8274,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 330,
	},
	{
		/* Qualcomm MSM8609 -> Qualcomm Adreno 304 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8609,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 304,
	},
	{
		/* Qualcomm MSM8610 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8610,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8612 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8612,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8625* -> Qualcomm Adreno 203 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8625,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 203,
	},
	{
		/* Qualcomm MSM8626 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8626,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8627 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8627,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8628 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8628,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8660 -> Qualcomm Adreno 220 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8660,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 220,
	},
	{
		/* Qualcomm MSM8674PRO -> Qualcomm Adreno 330 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8674,
		.chipset_suffix = 'P',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 330,
	},
	{
		/* Qualcomm MSM8909 -> Qualcomm Adreno 304 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8909,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 304,
	},
	{
		/* Qualcomm MSM8916 -> Qualcomm Adreno 306 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8916,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 306,
	},
	{
		/* Qualcomm MSM8917 -> Qualcomm Adreno 308 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8917,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 308,
	},
	{
		/* Qualcomm MSM8920 -> Qualcomm Adreno 308 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8920,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 308,
	},
	{
		/* Qualcomm MSM8926 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8926,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8928 -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8928,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8929 -> Qualcomm Adreno 405 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8929,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 405,
	},
	{
		/* Qualcomm MSM8930* -> Qualcomm Adreno 305 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8930,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 305,
	},
	{
		/* Qualcomm MSM8937 -> Qualcomm Adreno 505 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8937,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 505,
	},
	{
		/* Qualcomm MSM8939 -> Qualcomm Adreno 405 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8939,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 405,
	},
	{
		/* Qualcomm MSM8940 -> Qualcomm Adreno 505 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8940,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 505,
	},
	{
		/* Qualcomm MSM8952 -> Qualcomm Adreno 405 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8952,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 405,
	},
	{
		/* Qualcomm MSM8953* -> Qualcomm Adreno 506 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8953,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 506,
	},
	{
		/* Qualcomm MSM8956 -> Qualcomm Adreno 510 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8956,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 510,
	},
	{
		/* Qualcomm MSM8974* -> Qualcomm Adreno 330 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8974,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 330,
	},
	{
		/* Qualcomm MSM8976* -> Qualcomm Adreno 510 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8976,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 510,
	},
	{
		/* Qualcomm MSM8992 -> Qualcomm Adreno 418 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8992,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 418,
	},
	{
		/* Qualcomm MSM8994* -> Qualcomm Adreno 430 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8994,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 430,
	},
	{
		/* Qualcomm MSM8996* -> Qualcomm Adreno 530 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8996,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 530,
	},
	{
		/* Qualcomm MSM8998 -> Qualcomm Adreno 540 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_msm,
		.chipset_model = 8998,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 540,
	},
	{
		/* Qualcomm Snapdragon 450 -> Qualcomm Adreno 506 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_snapdragon,
		.chipset_model = 450,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 506,
	},
	{
		/* Qualcomm Snapdragon 630 -> Qualcomm Adreno 508 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_snapdragon,
		.chipset_model = 630,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 508,
	},
	{
		/* Qualcomm Snapdragon 660 -> Qualcomm Adreno 512 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_snapdragon,
		.chipset_model = 660,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 512,
	},
	{
		/* Qualcomm Snapdragon 670 -> Qualcomm Adreno 615 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_snapdragon,
		.chipset_model = 670,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 615,
	},
	{
		/* Qualcomm Snapdragon 845 -> Qualcomm Adreno 630 */
		.chipset_series = cpuinfo_arm_chipset_series_qualcomm_snapdragon,
		.chipset_model = 845,
		.gpu_series = cpuinfo_android_gpu_series_qualcomm_adreno,
		.gpu_model = 630,
	},
	{
		/* Renesas MP5232 -> PowerVR SGX 544 */
		.chipset_series = cpuinfo_arm_chipset_series_renesas_mp,
		.chipset_model = 5232,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 544,
	},
	{
		/* Rockchip RK2928 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_rockchip_rk,
		.chipset_model = 2928,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Rockchip RK3026 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_rockchip_rk,
		.chipset_model = 3026,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Rockchip RK3066 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_rockchip_rk,
		.chipset_model = 3066,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Rockchip RK3126* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_rockchip_rk,
		.chipset_model = 3126,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Rockchip RK3128 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_rockchip_rk,
		.chipset_model = 3128,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Rockchip RK3168 -> PowerVR SGX 540 */
		.chipset_series = cpuinfo_arm_chipset_series_rockchip_rk,
		.chipset_model = 3168,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 540,
	},
	{
		/* Rockchip RK3188 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_rockchip_rk,
		.chipset_model = 3188,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Rockchip RK3229 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_rockchip_rk,
		.chipset_model = 3229,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Rockchip RK3328 -> ARM Mali-450 */
		.chipset_series = cpuinfo_arm_chipset_series_rockchip_rk,
		.chipset_model = 3328,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 450,
	},
	{
		/* Rockchip RK3368 -> PowerVR Rogue G6110 */
		.chipset_series = cpuinfo_arm_chipset_series_rockchip_rk,
		.chipset_model = 3368,
		.gpu_series = cpuinfo_android_gpu_series_powervr_rogue_g,
		.gpu_model = 6110,
	},
	{
		/* Rockchip RK3399 -> ARM Mali-T860 */
		.chipset_series = cpuinfo_arm_chipset_series_rockchip_rk,
		.chipset_model = 3399,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 860,
	},
	{
		/* Samsung Exynos 3110 -> PowerVR SGX 540 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 3110,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 540,
	},
	{
		/* Samsung Exynos 3470 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 3470,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Samsung Exynos 3475 -> ARM Mali-T720 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 3475,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 720,
	},
	{
		/* Samsung Exynos 4210 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 4210,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Samsung Exynos 4412 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 4412,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Samsung Exynos 4415 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 4415,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Samsung Exynos 5250 -> ARM Mali-T604 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 5250,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 604,
	},
	{
		/* Samsung Exynos 5260 -> ARM Mali-T624 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 5260,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 624,
	},
	{
		/* Samsung Exynos 5410 -> PowerVR SGX 544 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 5410,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 544,
	},
	{
		/* Samsung Exynos 5420 -> ARM Mali-T628 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 5420,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 628,
	},
	{
		/* Samsung Exynos 5422 -> ARM Mali-T628 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 5422,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 628,
	},
	{
		/* Samsung Exynos 5430 -> ARM Mali-T628 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 5430,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 628,
	},
	{
		/* Samsung Exynos 5433 -> ARM Mali-T760 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 5433,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 760,
	},
	{
		/* Samsung Exynos 7420 -> ARM Mali-T760 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 7420,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 760,
	},
	{
		/* Samsung Exynos 7570 -> ARM Mali-T720 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 7570,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 720,
	},
	{
		/* Samsung Exynos 7578 -> ARM Mali-T720 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 7578,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 720,
	},
	{
		/* Samsung Exynos 7580 -> ARM Mali-T720 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 7580,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 720,
	},
	{
		/* Samsung Exynos 7870 -> ARM Mali-T830 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 7870,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 830,
	},
	{
		/* Samsung Exynos 7880 -> ARM Mali-T830 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 7880,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 830,
	},
	{
		/* Samsung Exynos 7885 -> ARM Mali-G71 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 7885,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_g,
		.gpu_model = 71,
	},
	{
		/* Samsung Exynos 8890 -> ARM Mali-T880 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 8890,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 880,
	},
	{
		/* Samsung Exynos 8895 -> ARM Mali-G71 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 8895,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_g,
		.gpu_model = 71,
	},
	{
		/* Samsung Exynos 9810 -> ARM Mali-G72 */
		.chipset_series = cpuinfo_arm_chipset_series_samsung_exynos,
		.chipset_model = 9810,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_g,
		.gpu_model = 72,
	},
	{
		/* Spreadtrum SC5735 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 5735,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC6815AS -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 6815,
		.chipset_suffix = 'A',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC6820I -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 6820,
		.chipset_suffix = 'I',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC7715* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 7715,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC7727* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 7727,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC7730* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 7730,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC7731* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 7731,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC7735S -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 7735,
		.chipset_suffix = 'S',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC8810 -> ARM Mali-300 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 8810,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 300,
	},
	{
		/* Spreadtrum SC8825 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 8825,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC8830 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 8830,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC9830* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 9830,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC9832* -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 9832,
		.chipset_suffix = '*',
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* Spreadtrum SC9850 -> ARM Mali-T820 */
		.chipset_series = cpuinfo_arm_chipset_series_spreadtrum_sc,
		.chipset_model = 9850,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali_t,
		.gpu_model = 820,
	},
	{
		/* Texas Instruments OMAP4430 -> PowerVR SGX 540 */
		.chipset_series = cpuinfo_arm_chipset_series_texas_instruments_omap,
		.chipset_model = 4430,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 540,
	},
	{
		/* Texas Instruments OMAP4460 -> PowerVR SGX 540 */
		.chipset_series = cpuinfo_arm_chipset_series_texas_instruments_omap,
		.chipset_model = 4460,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 540,
	},
	{
		/* Texas Instruments OMAP4470 -> PowerVR SGX 544 */
		.chipset_series = cpuinfo_arm_chipset_series_texas_instruments_omap,
		.chipset_model = 4470,
		.gpu_series = cpuinfo_android_gpu_series_powervr_sgx,
		.gpu_model = 544,
	},
	{
		/* WonderMedia WM8850 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_wondermedia_wm,
		.chipset_model = 8850,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* WonderMedia WM8880 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_wondermedia_wm,
		.chipset_model = 8880,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
	{
		/* WonderMedia WM8950 -> ARM Mali-400 */
		.chipset_series = cpuinfo_arm_chipset_series_wondermedia_wm,
		.chipset_model = 8950,
		.gpu_series = cpuinfo_android_gpu_series_arm_mali,
		.gpu_model = 400,
	},
};

/*
 * Map from Android GPU series ID to Android GPU vendor ID.
 * This map is used to avoid storing vendor IDs in tables.
 */
static enum cpuinfo_android_gpu_vendor gpu_series_vendor[cpuinfo_android_gpu_series_max] = {
	[cpuinfo_android_gpu_series_unknown]                 = cpuinfo_android_gpu_vendor_unknown,
	[cpuinfo_android_gpu_series_arm_mali]                = cpuinfo_android_gpu_vendor_arm,
	[cpuinfo_android_gpu_series_arm_mali_t]              = cpuinfo_android_gpu_vendor_arm,
	[cpuinfo_android_gpu_series_arm_mali_g]              = cpuinfo_android_gpu_vendor_arm,
	[cpuinfo_android_gpu_series_broadcom_videocore]      = cpuinfo_android_gpu_vendor_broadcom,
	[cpuinfo_android_gpu_series_intel_gen]               = cpuinfo_android_gpu_vendor_intel,
	[cpuinfo_android_gpu_series_intel_hd_graphics]       = cpuinfo_android_gpu_vendor_intel,
	[cpuinfo_android_gpu_series_nvidia_tegra]            = cpuinfo_android_gpu_vendor_nvidia,
	[cpuinfo_android_gpu_series_nvidia_tegra_k]          = cpuinfo_android_gpu_vendor_nvidia,
	[cpuinfo_android_gpu_series_nvidia_tegra_x]          = cpuinfo_android_gpu_vendor_nvidia,
	[cpuinfo_android_gpu_series_powervr_sgx]             = cpuinfo_android_gpu_vendor_powervr,
	[cpuinfo_android_gpu_series_powervr_rogue_g]         = cpuinfo_android_gpu_vendor_powervr,
	[cpuinfo_android_gpu_series_powervr_rogue_ge]        = cpuinfo_android_gpu_vendor_powervr,
	[cpuinfo_android_gpu_series_powervr_rogue_gm]        = cpuinfo_android_gpu_vendor_powervr,
	[cpuinfo_android_gpu_series_powervr_rogue_gt]        = cpuinfo_android_gpu_vendor_powervr,
	[cpuinfo_android_gpu_series_powervr_rogue_gx]        = cpuinfo_android_gpu_vendor_powervr,
	[cpuinfo_android_gpu_series_qualcomm_adreno]         = cpuinfo_android_gpu_vendor_qualcomm,
	[cpuinfo_android_gpu_series_vivante_gc]              = cpuinfo_android_gpu_vendor_vivante,
};

bool cpuinfo_arm_android_lookup_gpu(
	const struct cpuinfo_arm_chipset chipset[restrict static 1],
	struct cpuinfo_android_gpu gpu[restrict static 1])
{
	if (chipset->series == cpuinfo_arm_chipset_series_unknown) {
		return false;
	}

	for (uint32_t i = 0; i < CPUINFO_COUNT_OF(chipset_gpu_map); i++) {
		if (chipset_gpu_map[i].chipset_series == chipset->series &&
			chipset_gpu_map[i].chipset_model == chipset->model &&
			(chipset_gpu_map[i].chipset_suffix == '*' || chipset_gpu_map[i].chipset_suffix == chipset->suffix[0]))
		{
			*gpu = (struct cpuinfo_android_gpu) {
				.vendor = gpu_series_vendor[chipset_gpu_map[i].gpu_series],
				.series = (enum cpuinfo_android_gpu_series) chipset_gpu_map[i].gpu_series,
				.model = chipset_gpu_map[i].gpu_model,
				.suffix = {
					[0] = chipset_gpu_map[i].gpu_suffix,
					[1] = chipset_gpu_map[i].gpu_suffix == 'U' ? 'L' : '\0',
				},
			};
			return true;
		}
	}
	return false;
}
