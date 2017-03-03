#include <stdio.h>

#include <cpuinfo.h>

int main(int argc, char** argv) {
	cpuinfo_initialize();	
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	printf("ISA extensions:\n");
	#if CPUINFO_ARCH_X86
		printf("\tRDTSC: %s\n", cpuinfo_isa.rdtsc ? "yes" : "no");
	#endif
	printf("\tSYSENTER/SYSEXIT: %s\n", cpuinfo_isa.sysenter ? "yes" : "no");
	#if CPUINFO_ARCH_X86
		printf("\tSYSCALL/SYSRET: %s\n", cpuinfo_isa.syscall ? "yes" : "no");
	#endif
	printf("\tRDMSR/WRMSR: %s\n", cpuinfo_isa.msr ? "yes" : "no");
	printf("\tCLFLUSH: %s\n", cpuinfo_isa.clflush ? "yes" : "no");
	printf("\tCLFLUSHOPT: %s\n", cpuinfo_isa.clflushopt ? "yes" : "no");
	printf("\tMONITOR/MWAIT: %s\n", cpuinfo_isa.mwait ? "yes" : "no");
	printf("\tMONITORX/MWAITX: %s\n", cpuinfo_isa.mwaitx ? "yes" : "no");
	#if CPUINFO_ARCH_X86
		printf("\tEMMX: %s\n", cpuinfo_isa.emmx ? "yes" : "no");
	#endif
	printf("\tFXSAVE/FXSTOR: %s\n", cpuinfo_isa.fxsave ? "yes" : "no");
	printf("\tXSAVE/XSTOR: %s\n", cpuinfo_isa.xsave ? "yes" : "no");
	#if CPUINFO_ARCH_X86
		printf("\tx87 FPU: %s\n", cpuinfo_isa.fpu ? "yes" : "no");
		printf("\tMMX: %s\n", cpuinfo_isa.mmx ? "yes" : "no");
		printf("\tMMX+: %s\n", cpuinfo_isa.mmx_plus ? "yes" : "no");
	#endif
	printf("\t3dnow!: %s\n", cpuinfo_isa.three_d_now ? "yes" : "no");
	printf("\t3dnow!+: %s\n", cpuinfo_isa.three_d_now_plus ? "yes" : "no");
	#if CPUINFO_ARCH_X86
		printf("\t3dnow! Geode: %s\n", cpuinfo_isa.three_d_now_geode ? "yes" : "no");
	#endif
	printf("\t3dnow! Prefetch: %s\n", cpuinfo_isa.three_d_now_prefetch ? "yes" : "no");
	#if CPUINFO_ARCH_X86
		printf("\tSSE: %s\n", cpuinfo_isa.sse ? "yes" : "no");
		printf("\tSSE2: %s\n", cpuinfo_isa.sse2 ? "yes" : "no");
	#endif
	printf("\tSSE3: %s\n", cpuinfo_isa.sse3 ? "yes" : "no");
	printf("\tSSSE3: %s\n", cpuinfo_isa.ssse3 ? "yes" : "no");
	printf("\tSSE4.1: %s\n", cpuinfo_isa.sse4_1 ? "yes" : "no");
	printf("\tSSE4.2: %s\n", cpuinfo_isa.sse4_2 ? "yes" : "no");
	printf("\tSSE4a: %s\n", cpuinfo_isa.sse4a ? "yes" : "no");
	printf("\tMisaligned SSE: %s\n", cpuinfo_isa.misaligned_sse ? "yes" : "no");
	printf("\tAVX: %s\n", cpuinfo_isa.avx ? "yes" : "no");
	printf("\tFMA3: %s\n", cpuinfo_isa.fma3 ? "yes" : "no");
	printf("\tFMA4: %s\n", cpuinfo_isa.fma4 ? "yes" : "no");
	printf("\tXOP: %s\n", cpuinfo_isa.xop ? "yes" : "no");
	printf("\tF16C: %s\n", cpuinfo_isa.f16c ? "yes" : "no");
	printf("\tAVX2: %s\n", cpuinfo_isa.avx2 ? "yes" : "no");
	printf("\tHLE: %s\n", cpuinfo_isa.hle ? "yes" : "no");
	printf("\tRTM: %s\n", cpuinfo_isa.rtm ? "yes" : "no");
	printf("\tXTEST: %s\n", cpuinfo_isa.xtest ? "yes" : "no");
	printf("\tMPX: %s\n", cpuinfo_isa.mpx ? "yes" : "no");
	#if CPUINFO_ARCH_X86		
		printf("\tCMOV: %s\n", cpuinfo_isa.cmov ? "yes" : "no");
		printf("\tCMPXCHG8B: %s\n", cpuinfo_isa.cmpxchg8b ? "yes" : "no");
	#endif
	printf("\tCMPXCHG16B: %s\n", cpuinfo_isa.cmpxchg16b ? "yes" : "no");
	printf("\tMOVBE: %s\n", cpuinfo_isa.movbe ? "yes" : "no");
	#if CPUINFO_ARCH_X86_64
		printf("\tLAHF/SAHF: %s\n", cpuinfo_isa.lahf_sahf ? "yes" : "no");
	#endif
	printf("\tFS/GS Base: %s\n", cpuinfo_isa.fs_gs_base ? "yes" : "no");
	printf("\tLZCNT: %s\n", cpuinfo_isa.lzcnt ? "yes" : "no");
	printf("\tPOPCNT: %s\n", cpuinfo_isa.popcnt ? "yes" : "no");
	printf("\tTBM: %s\n", cpuinfo_isa.tbm ? "yes" : "no");
	printf("\tBMI: %s\n", cpuinfo_isa.bmi ? "yes" : "no");
	printf("\tBMI2: %s\n", cpuinfo_isa.bmi2 ? "yes" : "no");
	printf("\tADCX/ADOX: %s\n", cpuinfo_isa.adx ? "yes" : "no");
	printf("\tAES: %s\n", cpuinfo_isa.aes ? "yes" : "no");
	printf("\tPCLMULQDQ: %s\n", cpuinfo_isa.pclmulqdq ? "yes" : "no");
	printf("\tRDRAND: %s\n", cpuinfo_isa.rdrand ? "yes" : "no");
	printf("\tRDSEED: %s\n", cpuinfo_isa.rdseed ? "yes" : "no");
	printf("\tSHA: %s\n", cpuinfo_isa.sha ? "yes" : "no");
	printf("\tPadlock RNG: %s\n", cpuinfo_isa.rng ? "yes" : "no");
	printf("\tPadlock ACE: %s\n", cpuinfo_isa.ace ? "yes" : "no");
	printf("\tPadlock ACE 2: %s\n", cpuinfo_isa.ace2 ? "yes" : "no");
	printf("\tPadlock PHE: %s\n", cpuinfo_isa.phe ? "yes" : "no");
	printf("\tPadlock PMM: %s\n", cpuinfo_isa.pmm ? "yes" : "no");
	printf("\tPadlock LWP: %s\n", cpuinfo_isa.lwp ? "yes" : "no");
	printf("\tPadlock RDTSCP: %s\n", cpuinfo_isa.rdtscp ? "yes" : "no");
#endif

}
