#include <stdio.h>

#include <cpuinfo.h>

int main(int argc, char** argv) {
	cpuinfo_initialize();
	switch (cpuinfo_processors[0].vendor) {
		case cpuinfo_vendor_unknown:
			printf("Vendor: unknown\n");
			break;
		case cpuinfo_vendor_intel:
			printf("Vendor: Intel\n");
			break;
		case cpuinfo_vendor_amd:
			printf("Vendor: AMD\n");
			break;
		case cpuinfo_vendor_arm:
			printf("Vendor: ARM\n");
			break;
		case cpuinfo_vendor_qualcomm:
			printf("Vendor: Qualcomm\n");
			break;
		case cpuinfo_vendor_apple:
			printf("Vendor: Apple\n");
			break;
		case cpuinfo_vendor_samsung:
			printf("Vendor: Samsung\n");
			break;
		case cpuinfo_vendor_nvidia:
			printf("Vendor: nVidia\n");
			break;
		case cpuinfo_vendor_mips:
			printf("Vendor: MIPS\n");
			break;
		case cpuinfo_vendor_ibm:
			printf("Vendor: IBM\n");
			break;
		case cpuinfo_vendor_ingenic:
			printf("Vendor: Ingenic\n");
			break;
		case cpuinfo_vendor_via:
			printf("Vendor: VIA\n");
			break;
		default:
			printf("Vendor: other (%d)\n", cpuinfo_processors[0].vendor);
	}
	switch (cpuinfo_processors[0].uarch) {
		case cpuinfo_uarch_unknown:
			printf("uArch: unknown\n");
			break;
		case cpuinfo_uarch_p5:
			printf("uArch: P5\n");
			break;
		case cpuinfo_uarch_quark:
			printf("uArch: Quark\n");
			break;
		case cpuinfo_uarch_p6:
			printf("uArch: P6\n");
			break;
		case cpuinfo_uarch_dothan:
			printf("uArch: Dothan\n");
			break;
		case cpuinfo_uarch_yonah:
			printf("uArch: Yonah\n");
			break;
		case cpuinfo_uarch_conroe:
			printf("uArch: Conroe\n");
			break;
		case cpuinfo_uarch_penryn:
			printf("uArch: Penryn\n");
			break;
		case cpuinfo_uarch_nehalem:
			printf("uArch: Nehalem\n");
			break;
		case cpuinfo_uarch_sandy_bridge:
			printf("uArch: Sandy Bridge\n");
			break;
		case cpuinfo_uarch_ivy_bridge:
			printf("uArch: Ivy Bridge\n");
			break;
		case cpuinfo_uarch_haswell:
			printf("uArch: Haswell\n");
			break;
		case cpuinfo_uarch_broadwell:
			printf("uArch: Broadwell\n");
			break;
		case cpuinfo_uarch_sky_lake:
			printf("uArch: Sky Lake\n");
			break;
		case cpuinfo_uarch_kaby_lake:
			printf("uArch: Kaby Lake\n");
			break;
		case cpuinfo_uarch_willamette:
			printf("uArch: Willamette\n");
			break;
		case cpuinfo_uarch_prescott:
			printf("uArch: Prescott\n");
			break;
		case cpuinfo_uarch_bonnell:
			printf("uArch: Bonnell\n");
			break;
		case cpuinfo_uarch_saltwell:
			printf("uArch: Saltwell\n");
			break;
		case cpuinfo_uarch_silvermont:
			printf("uArch: Silvermont\n");
			break;
		case cpuinfo_uarch_airmont:
			printf("uArch: Airmont\n");
			break;
		case cpuinfo_uarch_knights_ferry:
			printf("uArch: Knights Ferry\n");
			break;
		case cpuinfo_uarch_knights_corner:
			printf("uArch: Knights Corner\n");
			break;
		case cpuinfo_uarch_knights_landing:
			printf("uArch: Knights Landing\n");
			break;
		case cpuinfo_uarch_knights_hill:
			printf("uArch: Knights Hill\n");
			break;
		case cpuinfo_uarch_knights_mill:
			printf("uArch: Knights Mill\n");
			break;
		case cpuinfo_uarch_k5:
			printf("uArch: K5\n");
			break;
		case cpuinfo_uarch_k6:
			printf("uArch: K6\n");
			break;
		case cpuinfo_uarch_k7:
			printf("uArch: K7\n");
			break;
		case cpuinfo_uarch_k8:
			printf("uArch: K8\n");
			break;
		case cpuinfo_uarch_k10:
			printf("uArch: K10\n");
			break;
		case cpuinfo_uarch_bulldozer:
			printf("uArch: Bulldozer\n");
			break;
		case cpuinfo_uarch_piledriver:
			printf("uArch: Piledriver\n");
			break;
		case cpuinfo_uarch_steamroller:
			printf("uArch: Steamroller\n");
			break;
		case cpuinfo_uarch_excavator:
			printf("uArch: Excavator\n");
			break;
		case cpuinfo_uarch_zen:
			printf("uArch: Zen\n");
			break;
		case cpuinfo_uarch_geode:
			printf("uArch: Geode\n");
			break;
		case cpuinfo_uarch_bobcat:
			printf("uArch: Bobcat\n");
			break;
		case cpuinfo_uarch_jaguar:
			printf("uArch: Jaguar/Puma\n");
			break;
		case cpuinfo_uarch_xscale:
			printf("uArch: XScale\n");
			break;
		case cpuinfo_uarch_arm7:
			printf("uArch: ARM7\n");
			break;
		case cpuinfo_uarch_arm9:
			printf("uArch: ARM9\n");
			break;
		case cpuinfo_uarch_arm11:
			printf("uArch: ARM11\n");
			break;
		case cpuinfo_uarch_cortex_a5:
			printf("uArch: Cortex-A5\n");
			break;
		case cpuinfo_uarch_cortex_a7:
			printf("uArch: Cortex-A7\n");
			break;
		case cpuinfo_uarch_cortex_a8:
			printf("uArch: Cortex-A8\n");
			break;
		case cpuinfo_uarch_cortex_a9:
			printf("uArch: Cortex-A9\n");
			break;
		case cpuinfo_uarch_cortex_a12:
			printf("uArch: Cortex-A12\n");
			break;
		case cpuinfo_uarch_cortex_a15:
			printf("uArch: Cortex-A15\n");
			break;
		case cpuinfo_uarch_cortex_a17:
			printf("uArch: Cortex-A17\n");
			break;
		case cpuinfo_uarch_cortex_a32:
			printf("uArch: Cortex-A32\n");
			break;
		case cpuinfo_uarch_cortex_a35:
			printf("uArch: Cortex-A35\n");
			break;
		case cpuinfo_uarch_cortex_a53:
			printf("uArch: Cortex-A53\n");
			break;
		case cpuinfo_uarch_cortex_a57:
			printf("uArch: Cortex-A57\n");
			break;
		case cpuinfo_uarch_cortex_a72:
			printf("uArch: Cortex-A72\n");
			break;
		case cpuinfo_uarch_cortex_a73:
			printf("uArch: Cortex-A73\n");
			break;
		case cpuinfo_uarch_scorpion:
			printf("uArch: Scorpion\n");
			break;
		case cpuinfo_uarch_krait:
			printf("uArch: Krait\n");
			break;
		case cpuinfo_uarch_kryo:
			printf("uArch: Kryo\n");
			break;
		case cpuinfo_uarch_denver:
			printf("uArch: Denver\n");
			break;
		case cpuinfo_uarch_mongoose:
			printf("uArch: Mongoose\n");
			break;
		case cpuinfo_uarch_swift:
			printf("uArch: Swift\n");
			break;
		case cpuinfo_uarch_cyclone:
			printf("uArch: Cyclone\n");
			break;
		case cpuinfo_uarch_typhoon:
			printf("uArch: Typhoon\n");
			break;
		case cpuinfo_uarch_twister:
			printf("uArch: Twister\n");
			break;
		case cpuinfo_uarch_hurricane:
			printf("uArch: Hurricane\n");
			break;
		default:
			printf("uArch: other (0x%08X)\n", cpuinfo_processors[0].uarch);
	}
}
