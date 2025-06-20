#include <cpuinfo.h>
#include <cpuinfo/internal-api.h>

bool CPUINFO_ABI cpuinfo_has_x86_rdtsc(void) {
#if CPUINFO_ARCH_X86_64
	return true;
#elif CPUINFO_ARCH_X86
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.rdtsc;
#endif
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_rdtscp(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.rdtscp;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_rdpid(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.rdpid;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_clzero(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.clzero;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_mwait(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.mwait;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_mwaitx(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.mwaitx;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_fxsave(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.fxsave;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_xsave(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.xsave;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_fpu(void) {
#if CPUINFO_ARCH_X86_64
	return true;
#elif CPUINFO_ARCH_X86
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.fpu;
#endif
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_mmx(void) {
#if CPUINFO_ARCH_X86_64
	return true;
#elif CPUINFO_ARCH_X86
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.mmx;
#endif
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_mmx_plus(void) {
#if CPUINFO_ARCH_X86_64
	return true;
#elif CPUINFO_ARCH_X86
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.mmx_plus;
#endif
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_3dnow(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.three_d_now;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_3dnow_plus(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.three_d_now_plus;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_3dnow_geode(void) {
#if CPUINFO_ARCH_X86_64
	return false;
#elif CPUINFO_ARCH_X86
#if defined(__ANDROID__)
	return false;
#else
	return cpuinfo_isa.three_d_now_geode;
#endif
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_prefetch(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.prefetch;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_prefetchw(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.prefetchw;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_prefetchwt1(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.prefetchwt1;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_daz(void) {
#if CPUINFO_ARCH_X86_64
	return true;
#elif CPUINFO_ARCH_X86
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.daz;
#endif
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_sse(void) {
#if CPUINFO_ARCH_X86_64
	return true;
#elif CPUINFO_ARCH_X86
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.sse;
#endif
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_sse2(void) {
#if CPUINFO_ARCH_X86_64
	return true;
#elif CPUINFO_ARCH_X86
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.sse2;
#endif
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_sse3(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.sse3;
#endif
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_ssse3(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.ssse3;
#endif
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_sse4_1(void) {
#if CPUINFO_ARCH_X86_64
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.sse4_1;
#endif
#elif CPUINFO_ARCH_X86
	return cpuinfo_isa.sse4_1;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_sse4_2(void) {
#if CPUINFO_ARCH_X86_64
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.sse4_2;
#endif
#elif CPUINFO_ARCH_X86
	return cpuinfo_isa.sse4_2;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_sse4a(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.sse4a;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_misaligned_sse(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.misaligned_sse;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avxvnni(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avxvnni;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_fma3(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.fma3;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_fma4(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.fma4;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_xop(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.xop;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_f16c(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.f16c;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx2(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx2;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512f(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512f;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512pf(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512pf;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512er(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512er;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512cd(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512cd;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512dq(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512dq;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512bw(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512bw;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512vl(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512vl;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512ifma(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512ifma;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512vbmi(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512vbmi;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512vbmi2(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512vbmi2;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512bitalg(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512bitalg;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512vpopcntdq(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512vpopcntdq;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512vnni(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512vnni;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512bf16(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512bf16;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512fp16(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512fp16;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512vp2intersect(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512vp2intersect;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512_4vnniw(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512_4vnniw;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx512_4fmaps(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx512_4fmaps;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_amx_bf16(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.amx_bf16;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_amx_tile(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.amx_tile;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_amx_int8(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.amx_int8;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_amx_fp16(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.amx_fp16;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx_vnni_int8(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx_vnni_int8;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx_vnni_int16(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx_vnni_int16;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx_ne_convert(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx_ne_convert;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx10_1(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx10_1;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_avx10_2(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.avx10_2;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_hle(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.hle;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_rtm(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.rtm;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_xtest(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.xtest;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_mpx(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.mpx;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_cmov(void) {
#if CPUINFO_ARCH_X86_64
	return true;
#elif CPUINFO_ARCH_X86
	return cpuinfo_isa.cmov;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_cmpxchg8b(void) {
#if CPUINFO_ARCH_X86_64
	return true;
#elif CPUINFO_ARCH_X86
	return cpuinfo_isa.cmpxchg8b;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_cmpxchg16b(void) {
#if CPUINFO_ARCH_X86_64
	return cpuinfo_isa.cmpxchg16b;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_clwb(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.clwb;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_movbe(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.movbe;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_lahf_sahf(void) {
#if CPUINFO_ARCH_X86
	return true;
#elif CPUINFO_ARCH_X86_64
	return cpuinfo_isa.lahf_sahf;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_lzcnt(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.lzcnt;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_popcnt(void) {
#if CPUINFO_ARCH_X86_64
#if defined(__ANDROID__)
	return true;
#else
	return cpuinfo_isa.popcnt;
#endif
#elif CPUINFO_ARCH_X86
	return cpuinfo_isa.popcnt;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_tbm(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.tbm;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_bmi(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.bmi;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_bmi2(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.bmi2;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_adx(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.adx;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_aes(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.aes;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_vaes(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.vaes;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_pclmulqdq(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.pclmulqdq;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_vpclmulqdq(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.vpclmulqdq;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_gfni(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.gfni;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_rdrand(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.rdrand;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_rdseed(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.rdseed;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_x86_sha(void) {
#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	return cpuinfo_isa.sha;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_thumb(void) {
#if CPUINFO_ARCH_ARM
	return cpuinfo_isa.thumb;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_thumb2(void) {
#if CPUINFO_ARCH_ARM
	return cpuinfo_isa.thumb2;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_v5e(void) {
#if CPUINFO_ARCH_ARM
	return cpuinfo_isa.armv5e;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_v6(void) {
#if CPUINFO_ARCH_ARM
	return cpuinfo_isa.armv6;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_v6k(void) {
#if CPUINFO_ARCH_ARM
	return cpuinfo_isa.armv6k;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_v7(void) {
#if CPUINFO_ARCH_ARM
	return cpuinfo_isa.armv7;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_v7mp(void) {
#if CPUINFO_ARCH_ARM
	return cpuinfo_isa.armv7mp;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_v8(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.armv8;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_idiv(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.idiv;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_vfpv2(void) {
#if CPUINFO_ARCH_ARM
	return cpuinfo_isa.vfpv2;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_vfpv3(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.vfpv3;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_vfpv3_d32(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.vfpv3 && cpuinfo_isa.d32;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_vfpv3_fp16(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.vfpv3 && cpuinfo_isa.fp16;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_vfpv3_fp16_d32(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.vfpv3 && cpuinfo_isa.fp16 && cpuinfo_isa.d32;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_vfpv4(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.vfpv3 && cpuinfo_isa.fma;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_vfpv4_d32(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.vfpv3 && cpuinfo_isa.fma && cpuinfo_isa.d32;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_fp16_arith(void) {
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	return cpuinfo_isa.fp16arith;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_bf16(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.bf16;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_wmmx(void) {
#if CPUINFO_ARCH_ARM
	return cpuinfo_isa.wmmx;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_wmmx2(void) {
#if CPUINFO_ARCH_ARM
	return cpuinfo_isa.wmmx2;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_neon(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.neon;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_neon_fp16(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.neon && cpuinfo_isa.fp16;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_neon_fma(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.neon && cpuinfo_isa.fma;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_neon_v8(void) {
#if CPUINFO_ARCH_ARM64
	return true;
#elif CPUINFO_ARCH_ARM
	return cpuinfo_isa.neon && cpuinfo_isa.armv8;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_atomics(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.atomics;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_neon_rdm(void) {
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	return cpuinfo_isa.rdm;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_neon_fp16_arith(void) {
#if CPUINFO_ARCH_ARM
	return cpuinfo_isa.neon && cpuinfo_isa.fp16arith;
#elif CPUINFO_ARCH_ARM64
	return cpuinfo_isa.fp16arith;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_fhm(void) {
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	return cpuinfo_isa.fhm;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_neon_dot(void) {
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	return cpuinfo_isa.dot;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_neon_bf16(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.bf16;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_jscvt(void) {
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	return cpuinfo_isa.jscvt;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_fcma(void) {
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	return cpuinfo_isa.fcma;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_i8mm(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.i8mm;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_aes(void) {
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	return cpuinfo_isa.aes;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sha1(void) {
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sha1;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sha2(void) {
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sha2;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_pmull(void) {
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	return cpuinfo_isa.pmull;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_crc32(void) {
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	return cpuinfo_isa.crc32;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sve(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sve;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sve_bf16(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sve && cpuinfo_isa.bf16;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sve2(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sve2;
#else
	return false;
#endif
}

// Function to get the max SVE vector length on ARM CPU's which support SVE.
uint32_t CPUINFO_ABI cpuinfo_get_max_arm_sve_length(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.svelen * 8; // bytes * 8 = bit length(vector length)
#else
	return 0;
#endif
}

// Function to get the max SME vector length on ARM CPU's which support SME.
uint32_t CPUINFO_ABI cpuinfo_get_max_arm_sme_length(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.smelen * 8; // bytes * 8 = bit length(vector length)
#else
	return 0;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sme(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sme;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sme2(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sme2;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sme2p1(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sme2p1;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sme_i16i32(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sme_i16i32;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sme_bi32i32(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sme_bi32i32;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sme_b16b16(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sme_b16b16;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_arm_sme_f16f16(void) {
#if CPUINFO_ARCH_ARM64
	return cpuinfo_isa.sme_f16f16;
#else
	return false;
#endif
}

#if CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
/* This structure is not a part of stable API. Use cpuinfo_has_riscv_* functions
 * instead. */
struct cpuinfo_riscv_isa {
	/**
	 * Keep fields in line with the canonical order as defined by
	 * Section 27.11 Subset Naming Convention.
	 */
	/* RV32I/64I/128I Base ISA. */
	bool i;
#if CPUINFO_ARCH_RISCV32
	/* RV32E Base ISA. */
	bool e;
#endif
	/* Integer Multiply/Divide Extension. */
	bool m;
	/* Atomic Extension. */
	bool a;
	/* Single-Precision Floating-Point Extension. */
	bool f;
	/* Double-Precision Floating-Point Extension. */
	bool d;
	/* Compressed Extension. */
	bool c;
	/* Vector Extension. */
	bool v;
};

extern struct cpuinfo_riscv_isa cpuinfo_isa;
#endif

bool CPUINFO_ABI cpuinfo_has_riscv_i(void) {
#if CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
	return cpuinfo_isa.i;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_riscv_e(void) {
#if CPUINFO_ARCH_RISCV32
	return cpuinfo_isa.e;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_riscv_m(void) {
#if CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
	return cpuinfo_isa.m;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_riscv_a(void) {
#if CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
	return cpuinfo_isa.a;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_riscv_f(void) {
#if CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
	return cpuinfo_isa.f;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_riscv_d(void) {
#if CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
	return cpuinfo_isa.d;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_riscv_g(void) {
	// The 'G' extension is simply shorthand for 'IMAFD'.
	return cpuinfo_has_riscv_i() && cpuinfo_has_riscv_m() && cpuinfo_has_riscv_a() && cpuinfo_has_riscv_f() &&
		cpuinfo_has_riscv_d();
}

bool CPUINFO_ABI cpuinfo_has_riscv_c(void) {
#if CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
	return cpuinfo_isa.c;
#else
	return false;
#endif
}

bool CPUINFO_ABI cpuinfo_has_riscv_v(void) {
#if CPUINFO_ARCH_RISCV32 || CPUINFO_ARCH_RISCV64
	return cpuinfo_isa.v;
#else
	return false;
#endif
}