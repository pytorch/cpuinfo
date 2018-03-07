#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

#include <cpuinfo.h>


static const char* vendor_to_string(enum cpuinfo_vendor vendor) {
	switch (vendor) {
		case cpuinfo_vendor_unknown:
			return "unknown";
		case cpuinfo_vendor_intel:
			return "Intel";
		case cpuinfo_vendor_amd:
			return "AMD";
		case cpuinfo_vendor_arm:
			return "ARM";
		case cpuinfo_vendor_qualcomm:
			return "Qualcomm";
		case cpuinfo_vendor_apple:
			return "Apple";
		case cpuinfo_vendor_samsung:
			return "Samsung";
		case cpuinfo_vendor_nvidia:
			return "Nvidia";
		case cpuinfo_vendor_mips:
			return "MIPS";
		case cpuinfo_vendor_ibm:
			return "IBM";
		case cpuinfo_vendor_ingenic:
			return "Ingenic";
		case cpuinfo_vendor_via:
			return "VIA";
		case cpuinfo_vendor_cavium:
			return "Cavium";
		default:
			return NULL;
	}
}

static const char* uarch_to_string(enum cpuinfo_uarch uarch) {
	switch (uarch) {
		case cpuinfo_uarch_unknown:
			return "unknown";
		case cpuinfo_uarch_p5:
			return "P5";
		case cpuinfo_uarch_quark:
			return "Quark";
		case cpuinfo_uarch_p6:
			return "P6";
		case cpuinfo_uarch_dothan:
			return "Dothan";
		case cpuinfo_uarch_yonah:
			return "Yonah";
		case cpuinfo_uarch_conroe:
			return "Conroe";
		case cpuinfo_uarch_penryn:
			return "Penryn";
		case cpuinfo_uarch_nehalem:
			return "Nehalem";
		case cpuinfo_uarch_sandy_bridge:
			return "Sandy Bridge";
		case cpuinfo_uarch_ivy_bridge:
			return "Ivy Bridge";
		case cpuinfo_uarch_haswell:
			return "Haswell";
		case cpuinfo_uarch_broadwell:
			return "Broadwell";
		case cpuinfo_uarch_sky_lake:
			return "Sky Lake";
		case cpuinfo_uarch_kaby_lake:
			return "Kaby Lake";
		case cpuinfo_uarch_willamette:
			return "Willamette";
		case cpuinfo_uarch_prescott:
			return "Prescott";
		case cpuinfo_uarch_bonnell:
			return "Bonnell";
		case cpuinfo_uarch_saltwell:
			return "Saltwell";
		case cpuinfo_uarch_silvermont:
			return "Silvermont";
		case cpuinfo_uarch_airmont:
			return "Airmont";
		case cpuinfo_uarch_knights_ferry:
			return "Knights Ferry";
		case cpuinfo_uarch_knights_corner:
			return "Knights Corner";
		case cpuinfo_uarch_knights_landing:
			return "Knights Landing";
		case cpuinfo_uarch_knights_hill:
			return "Knights Hill";
		case cpuinfo_uarch_knights_mill:
			return "Knights Mill";
		case cpuinfo_uarch_k5:
			return "K5";
		case cpuinfo_uarch_k6:
			return "K6";
		case cpuinfo_uarch_k7:
			return "K7";
		case cpuinfo_uarch_k8:
			return "K8";
		case cpuinfo_uarch_k10:
			return "K10";
		case cpuinfo_uarch_bulldozer:
			return "Bulldozer";
		case cpuinfo_uarch_piledriver:
			return "Piledriver";
		case cpuinfo_uarch_steamroller:
			return "Steamroller";
		case cpuinfo_uarch_excavator:
			return "Excavator";
		case cpuinfo_uarch_zen:
			return "Zen";
		case cpuinfo_uarch_geode:
			return "Geode";
		case cpuinfo_uarch_bobcat:
			return "Bobcat";
		case cpuinfo_uarch_jaguar:
			return "Jaguar";
		case cpuinfo_uarch_puma:
			return "Puma";
		case cpuinfo_uarch_xscale:
			return "XScale";
		case cpuinfo_uarch_arm7:
			return "ARM7";
		case cpuinfo_uarch_arm9:
			return "ARM9";
		case cpuinfo_uarch_arm11:
			return "ARM11";
		case cpuinfo_uarch_cortex_a5:
			return "Cortex-A5";
		case cpuinfo_uarch_cortex_a7:
			return "Cortex-A7";
		case cpuinfo_uarch_cortex_a8:
			return "Cortex-A8";
		case cpuinfo_uarch_cortex_a9:
			return "Cortex-A9";
		case cpuinfo_uarch_cortex_a12:
			return "Cortex-A12";
		case cpuinfo_uarch_cortex_a15:
			return "Cortex-A15";
		case cpuinfo_uarch_cortex_a17:
			return "Cortex-A17";
		case cpuinfo_uarch_cortex_a32:
			return "Cortex-A32";
		case cpuinfo_uarch_cortex_a35:
			return "Cortex-A35";
		case cpuinfo_uarch_cortex_a53:
			return "Cortex-A53";
		case cpuinfo_uarch_cortex_a57:
			return "Cortex-A57";
		case cpuinfo_uarch_cortex_a72:
			return "Cortex-A72";
		case cpuinfo_uarch_cortex_a73:
			return "Cortex-A73";
		case cpuinfo_uarch_scorpion:
			return "Scorpion";
		case cpuinfo_uarch_krait:
			return "Krait";
		case cpuinfo_uarch_kryo:
			return "Kryo";
		case cpuinfo_uarch_denver:
			return "Denver";
		case cpuinfo_uarch_mongoose_m1:
			return "Mongoose M1";
		case cpuinfo_uarch_mongoose_m2:
			return "Mongoose M2";
		case cpuinfo_uarch_mongoose_m3:
			return "Mongoose M3";
		case cpuinfo_uarch_swift:
			return "Swift";
		case cpuinfo_uarch_cyclone:
			return "Cyclone";
		case cpuinfo_uarch_typhoon:
			return "Typhoon";
		case cpuinfo_uarch_twister:
			return "Twister";
		case cpuinfo_uarch_hurricane:
			return "Hurricane";
		case cpuinfo_uarch_thunderx:
			return "ThunderX";
		case cpuinfo_uarch_pj4:
			return "PJ4";
		default:
			return NULL;
	}
}

int main(int argc, char** argv) {
	if (!cpuinfo_initialize()) {
		fprintf(stderr, "failed to initialize CPU information\n");
		exit(EXIT_FAILURE);
	}
	#ifdef __ANDROID__
		printf("SoC name: %s\n", cpuinfo_get_package(0)->name);
		printf("GPU name: %s\n", cpuinfo_get_package(0)->gpu_name);
	#else
		printf("Packages:\n");
		for (uint32_t i = 0; i < cpuinfo_get_packages_count(); i++) {
			printf("\t%"PRIu32": %s\n", i, cpuinfo_get_package(i)->name);
		}
	#endif
	printf("Cores:\n");
	for (uint32_t i = 0; i < cpuinfo_get_cores_count(); i++) {
		const struct cpuinfo_core* core = cpuinfo_get_core(i);
		if (core->processor_count == 1) {
			printf("\t%"PRIu32": 1 processor (%"PRIu32")", i, core->processor_start);
		} else {
			printf("\t%"PRIu32": %"PRIu32" processors (%"PRIu32"-%"PRIu32")",
				i, core->processor_count, core->processor_start, core->processor_start + core->processor_count - 1);
		}
		const char* vendor_string = vendor_to_string(core->vendor);
		const char* uarch_string = uarch_to_string(core->uarch);
		if (vendor_string == NULL) {
			printf(", vendor 0x%08"PRIx32" uarch 0x%08"PRIx32"\n",
				(uint32_t) core->vendor, (uint32_t) core->uarch);
		}
		else if (uarch_string == NULL) {
			printf(", %s uarch 0x%08"PRIx32"\n",
				vendor_string, (uint32_t) core->uarch);
		}
		else {
			printf(", %s %s\n", vendor_string, uarch_string);
		}
	}
	printf("Logical processors:\n");
	for (uint32_t i = 0; i < cpuinfo_get_processors_count(); i++) {
		const struct cpuinfo_processor* processor = cpuinfo_get_processor(i);
		#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
			printf("\t%"PRIu32": APIC ID 0x%08"PRIx32"\n", i, processor->apic_id);
		#else
			printf("\t%"PRIu32"\n", i);
		#endif
	}
}
