#pragma once
#include <stdint.h>


#define CPUINFO_ARM_MIDR_IMPLEMENTER_MASK  UINT32_C(0xFF000000)
#define CPUINFO_ARM_MIDR_VARIANT_MASK      UINT32_C(0x00F00000)
#define CPUINFO_ARM_MIDR_ARCHITECTURE_MASK UINT32_C(0x000F0000)
#define CPUINFO_ARM_MIDR_PART_MASK         UINT32_C(0x0000FFF0)
#define CPUINFO_ARM_MIDR_REVISION_MASK     UINT32_C(0x0000000F)

#define CPUINFO_ARM_MIDR_IMPLEMENTER_OFFSET  24
#define CPUINFO_ARM_MIDR_VARIANT_OFFSET      20
#define CPUINFO_ARM_MIDR_ARCHITECTURE_OFFSET 16
#define CPUINFO_ARM_MIDR_PART_OFFSET          4
#define CPUINFO_ARM_MIDR_REVISION_OFFSET      0

#define CPUINFO_ARM_MIDR_ARM1156         UINT32_C(0x410FB560)
#define CPUINFO_ARM_MIDR_CORTEX_A7       UINT32_C(0x410FC070)
#define CPUINFO_ARM_MIDR_CORTEX_A9       UINT32_C(0x410FC090)
#define CPUINFO_ARM_MIDR_CORTEX_A15      UINT32_C(0x410FC0F0)
#define CPUINFO_ARM_MIDR_CORTEX_A17      UINT32_C(0x410FC0E0)
#define CPUINFO_ARM_MIDR_CORTEX_A35      UINT32_C(0x410FD040)
#define CPUINFO_ARM_MIDR_CORTEX_A53      UINT32_C(0x410FD030)
#define CPUINFO_ARM_MIDR_CORTEX_A55      UINT32_C(0x410FD050)
#define CPUINFO_ARM_MIDR_CORTEX_A57      UINT32_C(0x410FD070)
#define CPUINFO_ARM_MIDR_CORTEX_A72      UINT32_C(0x410FD080)
#define CPUINFO_ARM_MIDR_CORTEX_A73      UINT32_C(0x410FD090)
#define CPUINFO_ARM_MIDR_CORTEX_A75      UINT32_C(0x410FD0A0)
#define CPUINFO_ARM_MIDR_KRYO280_GOLD    UINT32_C(0x510F8000)
#define CPUINFO_ARM_MIDR_KRYO280_SILVER  UINT32_C(0x510F8010)
#define CPUINFO_ARM_MIDR_KRYO_SILVER_821 UINT32_C(0x510F2010)
#define CPUINFO_ARM_MIDR_KRYO_GOLD       UINT32_C(0x510F2050)
#define CPUINFO_ARM_MIDR_KRYO_SILVER_820 UINT32_C(0x510F2110)
#define CPUINFO_ARM_MIDR_MONGOOSE        UINT32_C(0x530F0010)
#define CPUINFO_ARM_MIDR_DENVER2         UINT32_C(0x4E0F0030)

inline static uint32_t midr_set_implementer(uint32_t midr, uint32_t implementer) {
	return (midr & ~CPUINFO_ARM_MIDR_IMPLEMENTER_MASK) | 
		((implementer << CPUINFO_ARM_MIDR_IMPLEMENTER_OFFSET) & CPUINFO_ARM_MIDR_IMPLEMENTER_MASK);
}

inline static uint32_t midr_set_variant(uint32_t midr, uint32_t variant) {
	return (midr & ~CPUINFO_ARM_MIDR_VARIANT_MASK) | 
		((variant << CPUINFO_ARM_MIDR_VARIANT_OFFSET) & CPUINFO_ARM_MIDR_VARIANT_MASK);
}

inline static uint32_t midr_set_architecture(uint32_t midr, uint32_t architecture) {
	return (midr & ~CPUINFO_ARM_MIDR_ARCHITECTURE_MASK) | 
		((architecture << CPUINFO_ARM_MIDR_ARCHITECTURE_OFFSET) & CPUINFO_ARM_MIDR_ARCHITECTURE_MASK);
}

inline static uint32_t midr_set_part(uint32_t midr, uint32_t part) {
	return (midr & ~CPUINFO_ARM_MIDR_PART_MASK) | 
		((part << CPUINFO_ARM_MIDR_PART_OFFSET) & CPUINFO_ARM_MIDR_PART_MASK);
}

inline static uint32_t midr_set_revision(uint32_t midr, uint32_t revision) {
	return (midr & ~CPUINFO_ARM_MIDR_REVISION_MASK) | 
		((revision << CPUINFO_ARM_MIDR_REVISION_OFFSET) & CPUINFO_ARM_MIDR_REVISION_MASK);
}

inline static uint32_t midr_get_implementer(uint32_t midr) {
	return (midr & CPUINFO_ARM_MIDR_IMPLEMENTER_MASK) >> CPUINFO_ARM_MIDR_IMPLEMENTER_OFFSET;
}

inline static uint32_t midr_get_part(uint32_t midr) {
	return (midr & CPUINFO_ARM_MIDR_PART_MASK) >> CPUINFO_ARM_MIDR_PART_OFFSET;
}

inline static bool midr_is_arm1156(uint32_t midr) {
	const uint32_t uarch_mask = CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_PART_MASK;
	return (midr & uarch_mask) == (CPUINFO_ARM_MIDR_ARM1156 & uarch_mask);
}

inline static bool midr_is_arm11(uint32_t midr) {
	return (midr & (CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | 0x0000F000)) == UINT32_C(0x4100B000);
}

inline static bool midr_is_cortex_a9(uint32_t midr) {
	const uint32_t uarch_mask = CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_PART_MASK;
	return (midr & uarch_mask) == (CPUINFO_ARM_MIDR_CORTEX_A9 & uarch_mask);
}

inline static bool midr_is_krait(uint32_t midr) {
	switch (midr & (CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_PART_MASK)) {
		case UINT32_C(0x510004D0):
		case UINT32_C(0x510006F0):
			return true;
		default:
			return false;
	}
}

inline static bool midr_is_cortex_a53(uint32_t midr) {
	const uint32_t uarch_mask = CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_PART_MASK;
	return (midr & uarch_mask) == (CPUINFO_ARM_MIDR_CORTEX_A53 & uarch_mask);
}

inline static bool midr_is_kryo280_silver(uint32_t midr) {
	const uint32_t uarch_mask = CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_PART_MASK;
	return (midr & uarch_mask) == (CPUINFO_ARM_MIDR_KRYO280_SILVER & uarch_mask);
}

inline static bool midr_is_kryo280_gold(uint32_t midr) {
	const uint32_t uarch_mask = CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_PART_MASK;
	return (midr & uarch_mask) == (CPUINFO_ARM_MIDR_KRYO280_GOLD & uarch_mask);
}

inline static bool midr_is_kryo_silver(uint32_t midr) {
	const uint32_t uarch_mask =
		CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_ARCHITECTURE_MASK | CPUINFO_ARM_MIDR_PART_MASK;
	switch (midr & uarch_mask) {
		case CPUINFO_ARM_MIDR_KRYO_SILVER_820:
		case CPUINFO_ARM_MIDR_KRYO_SILVER_821:
			return true;
		default:
			return false;
	}
}

inline static bool midr_is_kryo_gold(uint32_t midr) {
	const uint32_t uarch_mask = CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_PART_MASK;
	return (midr & uarch_mask) == (CPUINFO_ARM_MIDR_KRYO_GOLD & uarch_mask);
}

inline static bool midr_is_big_core(uint32_t midr) {
	const uint32_t core_mask = 
		CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_ARCHITECTURE_MASK | CPUINFO_ARM_MIDR_PART_MASK;
	switch (midr & core_mask) {
		case CPUINFO_ARM_MIDR_CORTEX_A75:
		case CPUINFO_ARM_MIDR_CORTEX_A73:
		case CPUINFO_ARM_MIDR_CORTEX_A72:
		case CPUINFO_ARM_MIDR_CORTEX_A57:
		case CPUINFO_ARM_MIDR_CORTEX_A17:
		case CPUINFO_ARM_MIDR_CORTEX_A15:
		case CPUINFO_ARM_MIDR_KRYO280_GOLD:
		case CPUINFO_ARM_MIDR_KRYO_GOLD:
		case CPUINFO_ARM_MIDR_MONGOOSE:
		case CPUINFO_ARM_MIDR_DENVER2:
			return true;
		default:
			return false;
	}
}

inline static bool midr_is_little_core(uint32_t midr) {
	const uint32_t core_mask = 
		CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_ARCHITECTURE_MASK | CPUINFO_ARM_MIDR_PART_MASK;
	switch (midr & core_mask) {
		case CPUINFO_ARM_MIDR_CORTEX_A55:
		case CPUINFO_ARM_MIDR_CORTEX_A53:
		case CPUINFO_ARM_MIDR_CORTEX_A35:
		case CPUINFO_ARM_MIDR_CORTEX_A7:
		case CPUINFO_ARM_MIDR_KRYO280_SILVER:
		case CPUINFO_ARM_MIDR_KRYO_SILVER_820:
		case CPUINFO_ARM_MIDR_KRYO_SILVER_821:
			return true;
		default:
			return false;
	}
}

inline static uint32_t midr_little_core_for_big(uint32_t midr) {
	const uint32_t core_mask = 
		CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_ARCHITECTURE_MASK | CPUINFO_ARM_MIDR_PART_MASK;
	switch (midr & core_mask) {
		case CPUINFO_ARM_MIDR_CORTEX_A75:
			return CPUINFO_ARM_MIDR_CORTEX_A55;
		case CPUINFO_ARM_MIDR_CORTEX_A73:
		case CPUINFO_ARM_MIDR_CORTEX_A72:
		case CPUINFO_ARM_MIDR_CORTEX_A57:
		case CPUINFO_ARM_MIDR_MONGOOSE:
			return CPUINFO_ARM_MIDR_CORTEX_A53;
		case CPUINFO_ARM_MIDR_CORTEX_A17:
		case CPUINFO_ARM_MIDR_CORTEX_A15:
			return CPUINFO_ARM_MIDR_CORTEX_A7;
		case CPUINFO_ARM_MIDR_KRYO280_GOLD:
			return CPUINFO_ARM_MIDR_KRYO280_SILVER;
		case CPUINFO_ARM_MIDR_KRYO_GOLD:
			return CPUINFO_ARM_MIDR_KRYO_SILVER_820;
		case CPUINFO_ARM_MIDR_DENVER2:
			return CPUINFO_ARM_MIDR_CORTEX_A57;
		default:
			return midr;
	}
}

inline static uint32_t midr_big_core_for_little(uint32_t midr) {
	const uint32_t core_mask = 
		CPUINFO_ARM_MIDR_IMPLEMENTER_MASK | CPUINFO_ARM_MIDR_ARCHITECTURE_MASK | CPUINFO_ARM_MIDR_PART_MASK;
	switch (midr & core_mask) {
		case CPUINFO_ARM_MIDR_CORTEX_A57:
			return CPUINFO_ARM_MIDR_DENVER2;
		case CPUINFO_ARM_MIDR_CORTEX_A55:
			return CPUINFO_ARM_MIDR_CORTEX_A75;
		case CPUINFO_ARM_MIDR_CORTEX_A53:
		case CPUINFO_ARM_MIDR_CORTEX_A35:
			return CPUINFO_ARM_MIDR_CORTEX_A57;
		case CPUINFO_ARM_MIDR_CORTEX_A7:
			return CPUINFO_ARM_MIDR_CORTEX_A15;
		case CPUINFO_ARM_MIDR_KRYO280_SILVER:
			return CPUINFO_ARM_MIDR_KRYO280_GOLD;
		case CPUINFO_ARM_MIDR_KRYO_SILVER_820:
		case CPUINFO_ARM_MIDR_KRYO_SILVER_821:
			return CPUINFO_ARM_MIDR_KRYO_GOLD;
		default:
			return midr;
	}
}
