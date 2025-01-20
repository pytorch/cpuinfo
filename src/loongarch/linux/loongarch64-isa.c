#include <stdint.h>

#include <loongarch/linux/api.h>

void cpuinfo_loongarch64_linux_decode_isa_from_proc_cpuinfo(
	uint32_t features,
	struct cpuinfo_loongarch_isa isa[restrict static 1])
{
#define ISA_ENABLE(BIT, FLAG) isa->BIT = !!(features & CPUINFO_LOONGARCH_LINUX_FEATURE_##FLAG)
	ISA_ENABLE(cpucfg, CPUCFG);
	ISA_ENABLE(lam, LAM);
	ISA_ENABLE(ual, UAL);
	ISA_ENABLE(fpu, FPU);
	ISA_ENABLE(lsx, LSX);
	ISA_ENABLE(lasx, LASX);
	ISA_ENABLE(crc32, CRC32);
	ISA_ENABLE(complex, COMPLEX);
	ISA_ENABLE(crypto, CRYPTO);
	ISA_ENABLE(lvz, LVZ);
	ISA_ENABLE(lbt_x86, LBT_X86);
	ISA_ENABLE(lbt_arm, LBT_ARM);
	ISA_ENABLE(lbt_mips, LBT_MIPS);
	ISA_ENABLE(ptw, PTW);
	ISA_ENABLE(lspw, LSPW);
}
