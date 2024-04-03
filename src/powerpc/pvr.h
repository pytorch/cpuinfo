#pragma once
#include <stdint.h>

#define CPUINFO_POWERPC_PVR_VERSION_MASK  0xffff0000
#define CPUINFO_POWERPC_PVR_REVISION_MASK 0x0000ffff

#define CPUINFO_POWERPC_PVR_VERSION_OFFSET 16
#define CPUINFO_POWERPC_PVR_REVSION_OFFSET 0

static inline uint32_t pvr_get_revision(uint32_t pvr) {
	return (pvr & CPUINFO_POWERPC_PVR_REVISION_MASK) >> CPUINFO_POWERPC_PVR_REVSION_OFFSET;
}

static inline uint32_t pvr_get_version(uint32_t pvr) {
	return (pvr & CPUINFO_POWERPC_PVR_VERSION_MASK) >> CPUINFO_POWERPC_PVR_VERSION_OFFSET;
}

static inline uint32_t pvr_copy_revision(uint32_t pvr_a, uint32_t pvr_b) {
	return (pvr_a & ~CPUINFO_POWERPC_PVR_REVISION_MASK) | (pvr_b & CPUINFO_POWERPC_PVR_REVISION_MASK);
}

static inline uint32_t pvr_copy_version(uint32_t pvr_a, uint32_t pvr_b) {
	return (pvr_a & ~CPUINFO_POWERPC_PVR_VERSION_MASK) | (pvr_b & CPUINFO_POWERPC_PVR_VERSION_MASK);
}
