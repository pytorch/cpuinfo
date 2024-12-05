#include <string.h>

#include <dlfcn.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#if CPUINFO_MOCK
#include <cpuinfo-mock.h>
#endif

#include <cpuinfo.h>
#include <cpuinfo/log.h>
#include <powerpc/linux/api.h>

#include <sys/auxv.h>

#if CPUINFO_MOCK
static uint32_t mock_hwcap = 0;
void cpuinfo_set_hwcap(uint32_t hwcap) {
	mock_hwcap = hwcap;
}
#endif

void cpuinfo_powerpc_linux_hwcap_from_getauxval(uint32_t isa_feature[]) {
#if CPUINFO_MOCK
	isa_feature[0] = mock_hwcap;
#else
	isa_feature[0] = (uint32_t)getauxval(AT_HWCAP);
	isa_feature[1] = (uint32_t)getauxval(AT_HWCAP2);
#endif
}
