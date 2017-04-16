#include <stdio.h>

#include <cpuinfo.h>

int main(int argc, char** argv) {
	cpuinfo_initialize();

#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64

	printf("Scalar instructions:\n");
#if CPUINFO_ARCH_X86
		printf("\tx87 FPU: %s\n", cpuinfo_isa.fpu ? "yes" : "no");
		printf("\tCMOV: %s\n", cpuinfo_isa.cmov ? "yes" : "no");
#endif
#if CPUINFO_ARCH_X86_64
		printf("\tLAHF/SAHF: %s\n", cpuinfo_isa.lahf_sahf ? "yes" : "no");
#endif
		printf("\tLZCNT: %s\n", cpuinfo_isa.lzcnt ? "yes" : "no");
		printf("\tPOPCNT: %s\n", cpuinfo_isa.popcnt ? "yes" : "no");
		printf("\tTBM: %s\n", cpuinfo_isa.tbm ? "yes" : "no");
		printf("\tBMI: %s\n", cpuinfo_isa.bmi ? "yes" : "no");
		printf("\tBMI2: %s\n", cpuinfo_isa.bmi2 ? "yes" : "no");
		printf("\tADCX/ADOX: %s\n", cpuinfo_isa.adx ? "yes" : "no");


	printf("Memory instructions:\n");
		printf("\tMOVBE: %s\n", cpuinfo_isa.movbe ? "yes" : "no");
		printf("\tPREFETCH: %s\n", cpuinfo_isa.prefetch ? "yes" : "no");
		printf("\tPREFETCHW: %s\n", cpuinfo_isa.prefetchw ? "yes" : "no");
		printf("\tPREFETCHWT1: %s\n", cpuinfo_isa.prefetchwt1 ? "yes" : "no");
		printf("\tCLZERO: %s\n", cpuinfo_isa.clzero ? "yes" : "no");


	printf("SIMD extensions:\n");
#if CPUINFO_ARCH_X86
		printf("\tMMX: %s\n", cpuinfo_isa.mmx ? "yes" : "no");
		printf("\tMMX+: %s\n", cpuinfo_isa.mmx_plus ? "yes" : "no");
		printf("\tEMMX: %s\n", cpuinfo_isa.emmx ? "yes" : "no");
#endif
		printf("\t3dnow!: %s\n", cpuinfo_isa.three_d_now ? "yes" : "no");
		printf("\t3dnow!+: %s\n", cpuinfo_isa.three_d_now_plus ? "yes" : "no");
#if CPUINFO_ARCH_X86
		printf("\t3dnow! Geode: %s\n", cpuinfo_isa.three_d_now_geode ? "yes" : "no");
		printf("\tDAZ: %s\n", cpuinfo_isa.daz ? "yes" : "no");
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
		printf("\tAVX512F: %s\n", cpuinfo_isa.avx512f ? "yes" : "no");
		printf("\tAVX512PF: %s\n", cpuinfo_isa.avx512pf ? "yes" : "no");
		printf("\tAVX512ER: %s\n", cpuinfo_isa.avx512er ? "yes" : "no");
		printf("\tAVX512CD: %s\n", cpuinfo_isa.avx512cd ? "yes" : "no");
		printf("\tAVX512DQ: %s\n", cpuinfo_isa.avx512dq ? "yes" : "no");
		printf("\tAVX512BW: %s\n", cpuinfo_isa.avx512bw ? "yes" : "no");
		printf("\tAVX512VL: %s\n", cpuinfo_isa.avx512vl ? "yes" : "no");
		printf("\tAVX512IFMA: %s\n", cpuinfo_isa.avx512ifma ? "yes" : "no");
		printf("\tAVX512VBMI: %s\n", cpuinfo_isa.avx512vbmi ? "yes" : "no");
		printf("\tAVX512VPOPCNTDQ: %s\n", cpuinfo_isa.avx512vpopcntdq ? "yes" : "no");
		printf("\tAVX512_4VNNIW: %s\n", cpuinfo_isa.avx512_4vnniw ? "yes" : "no");
		printf("\tAVX512_4FMAPS: %s\n", cpuinfo_isa.avx512_4fmaps ? "yes" : "no");


	printf("Multi-threading extensions:\n");
		printf("\tMONITOR/MWAIT: %s\n", cpuinfo_isa.mwait ? "yes" : "no");
		printf("\tMONITORX/MWAITX: %s\n", cpuinfo_isa.mwaitx ? "yes" : "no");
#if CPUINFO_ARCH_X86
		printf("\tCMPXCHG8B: %s\n", cpuinfo_isa.cmpxchg8b ? "yes" : "no");
#endif
		printf("\tCMPXCHG16B: %s\n", cpuinfo_isa.cmpxchg16b ? "yes" : "no");
		printf("\tHLE: %s\n", cpuinfo_isa.hle ? "yes" : "no");
		printf("\tRTM: %s\n", cpuinfo_isa.rtm ? "yes" : "no");
		printf("\tXTEST: %s\n", cpuinfo_isa.xtest ? "yes" : "no");
		printf("\tRDPID: %s\n", cpuinfo_isa.rdpid ? "yes" : "no");


	printf("Cryptography extensions:\n");
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


	printf("Profiling instructions:\n");
#if CPUINFO_ARCH_X86
		printf("\tRDTSC: %s\n", cpuinfo_isa.rdtsc ? "yes" : "no");
#endif
		printf("\tRDTSCP: %s\n", cpuinfo_isa.rdtscp ? "yes" : "no");
		printf("\tLWP: %s\n", cpuinfo_isa.lwp ? "yes" : "no");
		printf("\tMPX: %s\n", cpuinfo_isa.mpx ? "yes" : "no");


	printf("System instructions:\n");
		printf("\tSYSENTER/SYSEXIT: %s\n", cpuinfo_isa.sysenter ? "yes" : "no");
#if CPUINFO_ARCH_X86
		printf("\tSYSCALL/SYSRET: %s\n", cpuinfo_isa.syscall ? "yes" : "no");
#endif
		printf("\tRDMSR/WRMSR: %s\n", cpuinfo_isa.msr ? "yes" : "no");
		printf("\tCLFLUSH: %s\n", cpuinfo_isa.clflush ? "yes" : "no");
		printf("\tCLFLUSHOPT: %s\n", cpuinfo_isa.clflushopt ? "yes" : "no");
		printf("\tCLWB: %s\n", cpuinfo_isa.clwb ? "yes" : "no");
		printf("\tFXSAVE/FXSTOR: %s\n", cpuinfo_isa.fxsave ? "yes" : "no");
		printf("\tXSAVE/XSTOR: %s\n", cpuinfo_isa.xsave ? "yes" : "no");
		printf("\tFS/GS Base: %s\n", cpuinfo_isa.fs_gs_base ? "yes" : "no");

#endif /* CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64 */

}
