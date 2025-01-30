#include <string.h>
#include <dlfcn.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cpuinfo.h>
#include <cpuinfo/log.h>
#include <powerpc/linux/api.h>
#include <sys/auxv.h>

void cpuinfo_powerpc_linux_hwcap_from_getauxval(uint32_t isa_feature[]) {
	isa_feature[0] = (uint32_t)getauxval(AT_HWCAP);
	isa_feature[1] = (uint32_t)getauxval(AT_HWCAP2);
}
