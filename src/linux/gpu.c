#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arm/linux/api.h>
#include <log.h>


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

/* Map from Android GPU vendor ID to its string representation */
static const char* gpu_vendor_string[cpuinfo_arm_chipset_vendor_max] = {
	[cpuinfo_android_gpu_vendor_unknown]  = "Unknown",
	[cpuinfo_android_gpu_vendor_arm]      = "ARM",
	[cpuinfo_android_gpu_vendor_broadcom] = "Broadcom",
	[cpuinfo_android_gpu_vendor_intel]    = "Intel",
	[cpuinfo_android_gpu_vendor_nvidia]   = "Nvidia",
	[cpuinfo_android_gpu_vendor_powervr]  = "PowerVR",
	[cpuinfo_android_gpu_vendor_qualcomm] = "Qualcomm",
	[cpuinfo_android_gpu_vendor_vivante]  = "Vivante",
};

/* Map from ARM chipset series ID to its string representation */
static const char* gpu_series_string[cpuinfo_arm_chipset_series_max] = {
	[cpuinfo_android_gpu_series_arm_mali]                = "Mali-",
	[cpuinfo_android_gpu_series_arm_mali_t]              = "Mali-T",
	[cpuinfo_android_gpu_series_arm_mali_g]              = "Mali-G",
	[cpuinfo_android_gpu_series_broadcom_videocore]      = "VideoCore ",
	[cpuinfo_android_gpu_series_intel_gen]               = "Gen ",
	[cpuinfo_android_gpu_series_intel_hd_graphics]       = "HD Graphics ",
	[cpuinfo_android_gpu_series_nvidia_tegra]            = "Tegra ",
	[cpuinfo_android_gpu_series_nvidia_tegra_k]          = "Tegra K",
	[cpuinfo_android_gpu_series_nvidia_tegra_x]          = "Tegra X",
	[cpuinfo_android_gpu_series_powervr_sgx]             = "SGX",
	[cpuinfo_android_gpu_series_powervr_rogue_g]         = "Rogue G",
	[cpuinfo_android_gpu_series_powervr_rogue_ge]        = "Rogue GE",
	[cpuinfo_android_gpu_series_powervr_rogue_gm]        = "Rogue GM",
	[cpuinfo_android_gpu_series_powervr_rogue_gt]        = "Rogue GT",
	[cpuinfo_android_gpu_series_powervr_rogue_gx]        = "Rogue GX",
	[cpuinfo_android_gpu_series_qualcomm_adreno]         = "Adreno ",
	[cpuinfo_android_gpu_series_vivante_gc]              = "GC",
};

/* Convert GPU name represented by cpuinfo_android_gpu structure to a string representation */
void cpuinfo_android_gpu_to_string(
	const struct cpuinfo_android_gpu gpu[restrict static 1],
	char name[restrict static CPUINFO_GPU_NAME_MAX])
{
	enum cpuinfo_android_gpu_vendor vendor = gpu->vendor;
	if (vendor >= cpuinfo_android_gpu_vendor_max) {
		vendor = cpuinfo_android_gpu_vendor_unknown;
	}
	enum cpuinfo_android_gpu_series series = gpu->series;
	if (series >= cpuinfo_android_gpu_series_max) {
		series = cpuinfo_android_gpu_series_unknown;
	}
	const char* vendor_string = gpu_vendor_string[vendor];
	const char* series_string = gpu_series_string[series];
	const uint32_t model = gpu->model;
	if (model == 0) {
		if (series == cpuinfo_android_gpu_series_unknown) {
			strncpy(name, vendor_string, CPUINFO_GPU_NAME_MAX);
		} else {
			snprintf(name, CPUINFO_GPU_NAME_MAX,
				"%s %s", vendor_string, series_string);
		}
	} else {
		snprintf(name, CPUINFO_GPU_NAME_MAX,
			"%s %s%"PRIu32"%.*s",
			vendor_string, series_string, model,
			(int) strnlen(gpu->suffix, CPUINFO_ANDROID_GPU_SUFFIX_MAX), gpu->suffix);
	}
}

/*
 * Decodes GPU name from OpenGL (ES) GL_RENDERER string.
 *
 * @param[in] renderer - the GL_RENDERER string.
 *
 * @returns Decoded GPU name. If GPU could not be decoded, the resulting structure would use `unknown` vendor
 *          and series identifiers.
 */
struct cpuinfo_android_gpu cpuinfo_android_decode_gpu(const char* renderer) {
	struct cpuinfo_android_gpu gpu;
	memset(&gpu, 0, sizeof(gpu));

	if (renderer != NULL) {
		switch (renderer[0]) {
			case 'A':
				if (sscanf(renderer, "Adreno (TM) %"SCNu32, &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_qualcomm_adreno;
				}
				break;
			case 'G':
				if (sscanf(renderer, "GC%"SCNu32" core", &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_vivante_gc;
				}
				break;
			case 'I':
				if (strcmp(renderer, "Immersion.16") == 0) {
					/* Vivante GC4000 in early HiSilicon chipsets */
					gpu.series = cpuinfo_android_gpu_series_vivante_gc;
					gpu.model = 4000;
				} else if (strcmp(renderer, "Intel(R) HD Graphics for BayTrail") == 0) {
					gpu.series = cpuinfo_android_gpu_series_intel_gen;
					gpu.model = 7;
				} else if (strcmp(renderer, "Intel(R) HD Graphics for Atom(TM) x5/x7") == 0) {
					gpu.series = cpuinfo_android_gpu_series_intel_gen;
					gpu.model = 8;
				}
				break;
			case 'M':
				if (sscanf(renderer, "Mali-%"SCNu32, &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_arm_mali;
				} else if (sscanf(renderer, "Mali-T%"SCNu32, &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_arm_mali_t;
					/* Common bug: Mali-T760 reported as Mali-T764 */
					if (gpu.model == 764) {
						gpu.model = 760;
					}
				} else if (sscanf(renderer, "Mali-G%"SCNu32, &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_arm_mali_g;
				} else if (sscanf(renderer, "Mesa DRI Intel(R) HD Graphics %"SCNu32, &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_intel_hd_graphics;
				} else if (strcmp(renderer, "Mesa DRI Intel(R) Sandybridge Mobile x86/MMX/SSE2") == 0) {
					gpu.series = cpuinfo_android_gpu_series_intel_gen;
					gpu.model = 6;
				}
				break;
			case 'N':
				if (strcmp(renderer, "NVIDIA Tegra 3") == 0) {
					gpu.series = cpuinfo_android_gpu_series_nvidia_tegra;
					gpu.model = 3;
				} else if (strcmp(renderer, "NVIDIA Tegra") == 0) {
					gpu.series = cpuinfo_android_gpu_series_nvidia_tegra;
					gpu.model = 0;
				}
				break;
			case 'P':
				if (strcmp(renderer, "PXA1928 GPU") == 0) {
					/* Vivante GC5000 in PXA1928 chipset */
					gpu.series = cpuinfo_android_gpu_series_vivante_gc;
					gpu.model = 5000;
				} else if (sscanf(renderer, "PowerVR SGX %"SCNu32, &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_powervr_sgx;
				} else if (sscanf(renderer, "PowerVR Rogue G%"SCNu32, &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_powervr_rogue_g;
				} else if (sscanf(renderer, "PowerVR Rogue GE%"SCNu32, &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_powervr_rogue_ge;
				} else if (sscanf(renderer, "PowerVR Rogue GM%"SCNu32, &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_powervr_rogue_gm;
				} else if (sscanf(renderer, "PowerVR Rogue GT%"SCNu32, &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_powervr_rogue_gt;
				} else if (sscanf(renderer, "PowerVR Rogue GX%"SCNu32, &gpu.model) == 1 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_powervr_rogue_gx;
				} else if (strcmp(renderer, "PowerVR Rogue Han") == 0) {
					gpu.series = cpuinfo_android_gpu_series_powervr_rogue_g;
					gpu.model = 6200;
				} else if (strcmp(renderer, "PowerVR Rogue Hood") == 0) {
					gpu.series = cpuinfo_android_gpu_series_powervr_rogue_g;
					gpu.model = 6430;
				} else if (strcmp(renderer, "PowerVR Rogue Marlowe") == 0) {
					gpu.series = cpuinfo_android_gpu_series_powervr_rogue_gt;
					gpu.model = 7400;
					gpu.suffix[0] = '+';
				}
				break;
			case 'S':
				if (strcmp(renderer, "Sapphire 650") == 0) {
					/* ARM Mali-T720 in some MT8163 chipsets */
					gpu.series = cpuinfo_android_gpu_series_arm_mali_t;
					gpu.model = 720;
				}
				break;
			case 'V':
				if (strcmp(renderer, "VideoCore IV HW") == 0) {
					gpu.series = cpuinfo_android_gpu_series_broadcom_videocore;
					gpu.model = 4;
				} else if (sscanf(renderer, "Vivante GC%"SCNu32"%3s", &gpu.model, gpu.suffix) == 2 && gpu.model != 0) {
					gpu.series = cpuinfo_android_gpu_series_vivante_gc;
				}
				break;
		}
	}
	gpu.vendor = gpu_series_vendor[gpu.series];

	return gpu;
}
