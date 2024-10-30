#include <stdint.h>

#include <powerpc/linux/api.h>
#include <cpuinfo/log.h>

void cpuinfo_ppc64_linux_decode_isa_from_hwcap(
	uint32_t features, uint32_t features2,
	struct cpuinfo_powerpc_isa isa[restrict static 1])
{
	isa->vsx=!!(features & CPUINFO_POWERPC_LINUX_FEATURE_HAS_VSX);
	isa->htm=!!(features2 & CPUINFO_POWERPC_LINUX_FEATURE_HTM);
	isa->mma=!!(features2 & CPUINFO_POWERPC_LINUX_FEATURE_HAS_MMA);
}
