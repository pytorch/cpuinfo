#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>
#include <cpuinfo/log.h>

#include "windows-arm-init.h"

/* Efficiency class = 0 means little core, while 1 means big core for now */
#define MAX_WOA_VALID_EFFICIENCY_CLASSES		2
#define VENDOR_NAME_MAX		CPUINFO_PACKAGE_NAME_MAX

struct cpuinfo_arm_isa cpuinfo_isa;

static void set_cpuinfo_isa_fields(void);
static bool get_system_info_from_registry(
	struct woa_chip_info** chip_info,
	enum cpuinfo_vendor* vendor);

struct vendor_info {
	wchar_t vendor_name[VENDOR_NAME_MAX];
	enum cpuinfo_vendor vendor;
};

/* Please add new vendor here! */
static struct vendor_info vendors[] = {
	{
		L"Qualcomm",
		cpuinfo_vendor_qualcomm
	},
	{
		L"Ampere(R)",
		cpuinfo_vendor_ampere
	}
};

static struct woa_chip_info woa_chip_unknown = {
	L"Unknown",
	woa_chip_name_unknown,
	{
		{
			cpuinfo_uarch_unknown,
			0
		}
	}
};

/* Please add new SoC/chip info here! */
static struct woa_chip_info woa_chips[] = {
	/* Microsoft SQ1 Kryo 495 4 + 4 cores (3 GHz + 1.80 GHz) */
	{
		L"Microsoft SQ1",
		woa_chip_name_microsoft_sq_1,
		{
			{
				cpuinfo_uarch_cortex_a55,
				1800000000,
			},
			{
				cpuinfo_uarch_cortex_a76,
				3000000000,
			}
		}
	},
	/* Microsoft SQ2 Kryo 495 4 + 4 cores (3.15 GHz + 2.42 GHz) */
	{
		L"Microsoft SQ2",
		woa_chip_name_microsoft_sq_2,
		{
			{
				cpuinfo_uarch_cortex_a55,
				2420000000,
			},
			{
				cpuinfo_uarch_cortex_a76,
				3150000000
			}
		}
	},
	/* Microsoft Windows Dev Kit 2023 */
	{
		L"Snapdragon (TM) 8cx Gen 3 @ 3.0 GHz",
		woa_chip_name_microsoft_sq_3,
		{
			{
				cpuinfo_uarch_cortex_a78,
				2420000000,
			},
			{
				cpuinfo_uarch_cortex_x1,
				3000000000
			}
		}
	},
	/* Ampere Altra */
	{
		L"Ampere(R) Altra(R) Processor",
		woa_chip_name_ampere_altra,
		{
			{
				cpuinfo_uarch_neoverse_n1,
				3000000000
			}
		}
	}
};

BOOL CALLBACK cpuinfo_arm_windows_init(
	PINIT_ONCE init_once, PVOID parameter, PVOID* context)
{
	struct woa_chip_info *chip_info = NULL;
	enum cpuinfo_vendor vendor = cpuinfo_vendor_unknown;
	
	set_cpuinfo_isa_fields();

	const bool system_result = get_system_info_from_registry(&chip_info, &vendor);
	if (!system_result) {
		chip_info = &woa_chip_unknown;
	}

	cpuinfo_is_initialized = cpu_info_init_by_logical_sys_info(chip_info, vendor);

	return (system_result && cpuinfo_is_initialized ? TRUE : FALSE);
}

bool get_core_uarch_for_efficiency(
	enum woa_chip_name chip, BYTE EfficiencyClass,
	enum cpuinfo_uarch* uarch, uint64_t* frequency)
{
	/* For currently supported WoA chips, the Efficiency class selects
	 * the pre-defined little and big core.
	 * Any further supported SoC's logic should be implemented here.
	 */
	if (uarch && frequency && chip < woa_chip_name_last &&
		EfficiencyClass < MAX_WOA_VALID_EFFICIENCY_CLASSES) {
		*uarch = woa_chips[chip].uarchs[EfficiencyClass].uarch;
		*frequency = woa_chips[chip].uarchs[EfficiencyClass].frequency;
		return true;
	}
	return false;
}

/* Static helper functions */

static bool read_registry(
	LPCWSTR subkey,
	LPCWSTR value,
	char** text_buffer)
{
	DWORD key_type = 0;
	DWORD data_size = 0;
	const DWORD flags = RRF_RT_REG_SZ; /* Only read strings (REG_SZ) */
	LSTATUS result = 0;
	HANDLE heap = GetProcessHeap();

	result = RegGetValueW(
		HKEY_LOCAL_MACHINE,
		subkey,
		value,
		flags,
		&key_type,
		NULL, /* Request buffer size */
		&data_size);
	if (result != 0 || data_size == 0) {
		cpuinfo_log_error("Registry entry size read error");
		return false;
	}

	if (*text_buffer) {
		HeapFree(heap, 0, *text_buffer);
	}
	*text_buffer = HeapAlloc(heap, HEAP_ZERO_MEMORY, data_size * sizeof(wchar_t));
	if (*text_buffer == NULL) {
		cpuinfo_log_error("Registry textbuffer allocation error");
		return false;
	}

	result = RegGetValueW(
		HKEY_LOCAL_MACHINE,
		subkey,
		value,
		flags,
		NULL,
		*text_buffer, /* Write string in this destination buffer */
		&data_size);
	if (result != 0) {
		cpuinfo_log_error("Registry read error");
		return false;
	}
	return true;
}

static bool get_system_info_from_registry(
	struct woa_chip_info** chip_info,
	enum cpuinfo_vendor* vendor)
{
	bool result = false;
	char* text_buffer = NULL;
	LPCWSTR cpu0_subkey = L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
	LPCWSTR chip_name_value = L"ProcessorNameString";
	LPCWSTR vendor_name_value = L"VendorIdentifier";

	*chip_info = NULL;
	*vendor = cpuinfo_vendor_unknown;
	HANDLE heap = GetProcessHeap();

	/* 1. Read processor model name from registry and find in the hard-coded list. */
	if (!read_registry(cpu0_subkey, chip_name_value, &text_buffer)) {
		cpuinfo_log_error("Registry read error");
		goto cleanup;
	}
	for (uint32_t i = 0; i < (uint32_t) woa_chip_name_last; i++) {
		size_t compare_length = wcsnlen(woa_chips[i].chip_name_string, CPUINFO_PACKAGE_NAME_MAX);
		int compare_result = wcsncmp(text_buffer, woa_chips[i].chip_name_string, compare_length);
		if (compare_result == 0) {
			*chip_info = woa_chips+i;
			break;
		}
	}
	if (*chip_info == NULL) {
		/* No match was found, so print a warning and assign the unknown case. */
		cpuinfo_log_error("Unknown chip model name '%ls'.\nPlease add new Windows on Arm SoC/chip support to arm/windows/init.c!", text_buffer);
		goto cleanup;
	}
	cpuinfo_log_debug("detected chip model name: %s", (**chip_info).chip_name_string);

	/* 2. Read vendor/manufacturer name from registry. */
	if (!read_registry(cpu0_subkey, vendor_name_value, &text_buffer)) {
		cpuinfo_log_error("Registry read error");
		goto cleanup;
	}

	for (uint32_t i = 0; i < (sizeof(vendors) / sizeof(struct vendor_info)); i++) {
		if (wcsncmp(text_buffer, vendors[i].vendor_name,
				wcslen(vendors[i].vendor_name)) == 0) {
			*vendor = vendors[i].vendor;
			result = true;
			break;
		}
	}
	if (*vendor == cpuinfo_vendor_unknown) {
		cpuinfo_log_error("Unexpected vendor: %ls", text_buffer);
	}

cleanup:
	HeapFree(heap, 0, text_buffer);
	text_buffer = NULL;
	return result;
}

static void set_cpuinfo_isa_fields(void)
{
	bool armv8 = IsProcessorFeaturePresent(PF_ARM_V8_INSTRUCTIONS_AVAILABLE);
	bool crypto = IsProcessorFeaturePresent(PF_ARM_V8_CRYPTO_INSTRUCTIONS_AVAILABLE);
	bool load_store_atomic = IsProcessorFeaturePresent(PF_ARM_64BIT_LOADSTORE_ATOMIC);
	bool float_multiply_accumulate = IsProcessorFeaturePresent(PF_ARM_FMAC_INSTRUCTIONS_AVAILABLE);
	bool crc32 = IsProcessorFeaturePresent(PF_ARM_V8_CRC32_INSTRUCTIONS_AVAILABLE);
	bool float_emulated = IsProcessorFeaturePresent(PF_FLOATING_POINT_EMULATED);

	/* Read all Arm related Windows features for debug purposes, even if we can't
	 * pair Arm ISA feature to that now.
	 */
#if CPUINFO_LOG_DEBUG_PARSERS
	bool divide = IsProcessorFeaturePresent(PF_ARM_DIVIDE_INSTRUCTION_AVAILABLE);
	bool ext_cache = IsProcessorFeaturePresent(PF_ARM_EXTERNAL_CACHE_AVAILABLE);
	bool vfp_registers = IsProcessorFeaturePresent(PF_ARM_VFP_32_REGISTERS_AVAILABLE);
	bool arm_v81 = IsProcessorFeaturePresent(PF_ARM_V81_ATOMIC_INSTRUCTIONS_AVAILABLE);

	cpuinfo_log_debug("divide present: %d", divide);
	cpuinfo_log_debug("ext_cache present: %d", ext_cache);
	cpuinfo_log_debug("vfp_registers present: %d", vfp_registers);
	cpuinfo_log_debug("arm_v81 present: %d", arm_v81);
#endif

	cpuinfo_log_debug("armv8 present: %d", armv8);
	cpuinfo_log_debug("crypto present: %d", crypto);
	cpuinfo_log_debug("load_store_atomic present: %d", load_store_atomic);
	cpuinfo_log_debug("float_multiply_accumulate present: %d", float_multiply_accumulate);
	cpuinfo_log_debug("crc32 present: %d", crc32);
	cpuinfo_log_debug("float_emulated: %d", float_emulated);

#if CPUINFO_ARCH_ARM
	cpuinfo_isa.armv8 = armv8;
#endif
#if CPUINFO_ARCH_ARM64
	cpuinfo_isa.atomics = load_store_atomic;
#endif
	cpuinfo_isa.crc32 = crc32;
	/* Windows API reports all or nothing for cryptographic instructions. */
	cpuinfo_isa.aes = crypto;
	cpuinfo_isa.sha1 = crypto;
	cpuinfo_isa.sha2 = crypto;
	cpuinfo_isa.pmull = crypto;
	cpuinfo_isa.fp16arith = !float_emulated && float_multiply_accumulate;
}
