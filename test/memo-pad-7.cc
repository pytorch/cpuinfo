#include <gtest/gtest.h>

#include <cpuinfo.h>
#include <cpuinfo-mock.h>


TEST(PROCESSORS, count) {
	ASSERT_EQ(4, cpuinfo_processors_count);
}

TEST(PROCESSORS, non_null) {
	ASSERT_TRUE(cpuinfo_processors);
}

TEST(PROCESSORS, smt_id) {
	for (uint32_t i = 0; i < cpuinfo_processors_count; i++) {
		ASSERT_EQ(0, cpuinfo_processors[i].smt_id);
	}
}

TEST(PROCESSORS, core) {
	for (uint32_t i = 0; i < cpuinfo_processors_count; i++) {
		ASSERT_EQ(&cpuinfo_cores[i], cpuinfo_processors[i].core);
	}
}

TEST(PROCESSORS, package) {
	for (uint32_t i = 0; i < cpuinfo_processors_count; i++) {
		ASSERT_EQ(&cpuinfo_packages[0], cpuinfo_processors[i].package);
	}
}

TEST(PROCESSORS, linux_id) {
	for (uint32_t i = 0; i < cpuinfo_processors_count; i++) {
		ASSERT_EQ(i, cpuinfo_processors[i].linux_id);
	}
}

TEST(PROCESSORS, l1i) {
	for (uint32_t i = 0; i < cpuinfo_processors_count; i++) {
		ASSERT_EQ(&cpuinfo_get_l1i_cache().instances[i], cpuinfo_processors[i].cache.l1i);
	}
}

TEST(PROCESSORS, l1d) {
	for (uint32_t i = 0; i < cpuinfo_processors_count; i++) {
		ASSERT_EQ(&cpuinfo_get_l1d_cache().instances[i], cpuinfo_processors[i].cache.l1d);
	}
}

TEST(PROCESSORS, l2) {
	for (uint32_t i = 0; i < cpuinfo_processors_count; i++) {
		ASSERT_EQ(&cpuinfo_get_l2_cache().instances[i / 2], cpuinfo_processors[i].cache.l2);
	}
}

TEST(PROCESSORS, l3) {
	for (uint32_t i = 0; i < cpuinfo_processors_count; i++) {
		ASSERT_FALSE(cpuinfo_processors[i].cache.l3);
	}
}

TEST(PROCESSORS, l4) {
	for (uint32_t i = 0; i < cpuinfo_processors_count; i++) {
		ASSERT_FALSE(cpuinfo_processors[i].cache.l4);
	}
}

TEST(CORES, count) {
	ASSERT_EQ(4, cpuinfo_cores_count);
}

TEST(CORES, non_null) {
	ASSERT_TRUE(cpuinfo_cores);
}

TEST(CORES, processor_start) {
	for (uint32_t i = 0; i < cpuinfo_cores_count; i++) {
		ASSERT_EQ(i, cpuinfo_cores[i].processor_start);
	}
}

TEST(CORES, processor_count) {
	for (uint32_t i = 0; i < cpuinfo_cores_count; i++) {
		ASSERT_EQ(1, cpuinfo_cores[i].processor_count);
	}
}

TEST(CORES, core_id) {
	for (uint32_t i = 0; i < cpuinfo_cores_count; i++) {
		ASSERT_EQ(i, cpuinfo_cores[i].core_id);
	}
}

TEST(CORES, package) {
	for (uint32_t i = 0; i < cpuinfo_cores_count; i++) {
		ASSERT_EQ(&cpuinfo_packages[0], cpuinfo_cores[i].package);
	}
}

TEST(CORES, vendor) {
	for (uint32_t i = 0; i < cpuinfo_cores_count; i++) {
		ASSERT_EQ(cpuinfo_vendor_intel, cpuinfo_cores[i].vendor);
	}
}

TEST(CORES, uarch) {
	for (uint32_t i = 0; i < cpuinfo_cores_count; i++) {
		ASSERT_EQ(cpuinfo_uarch_silvermont, cpuinfo_cores[i].uarch);
	}
}

TEST(CORES, cpuid) {
	for (uint32_t i = 0; i < cpuinfo_cores_count; i++) {
		ASSERT_EQ(UINT32_C(0x00030678), cpuinfo_cores[i].cpuid);
	}
}

TEST(PACKAGES, count) {
	ASSERT_EQ(1, cpuinfo_packages_count);
}

TEST(PACKAGES, name) {
	for (uint32_t i = 0; i < cpuinfo_packages_count; i++) {
		ASSERT_EQ("Atom Z3745",
			std::string(cpuinfo_packages[i].name,
				strnlen(cpuinfo_packages[i].name, CPUINFO_PACKAGE_NAME_MAX)));
	}
}

TEST(PACKAGES, processor_start) {
	for (uint32_t i = 0; i < cpuinfo_packages_count; i++) {
		ASSERT_EQ(0, cpuinfo_packages[i].processor_start);
	}
}

TEST(PACKAGES, processor_count) {
	for (uint32_t i = 0; i < cpuinfo_packages_count; i++) {
		ASSERT_EQ(4, cpuinfo_packages[i].processor_count);
	}
}

TEST(PACKAGES, core_start) {
	for (uint32_t i = 0; i < cpuinfo_packages_count; i++) {
		ASSERT_EQ(0, cpuinfo_packages[i].core_start);
	}
}

TEST(PACKAGES, core_count) {
	for (uint32_t i = 0; i < cpuinfo_packages_count; i++) {
		ASSERT_EQ(4, cpuinfo_packages[i].core_count);
	}
}

TEST(ISA, rdtsc) {
	ASSERT_TRUE(cpuinfo_isa.rdtsc);
}

TEST(ISA, sysenter) {
	ASSERT_TRUE(cpuinfo_isa.sysenter);
}

TEST(ISA, syscall) {
	ASSERT_FALSE(cpuinfo_isa.syscall);
}

TEST(ISA, msr) {
	ASSERT_TRUE(cpuinfo_isa.msr);
}

TEST(ISA, clzero) {
	ASSERT_FALSE(cpuinfo_isa.clzero);
}

TEST(ISA, clflush) {
	ASSERT_TRUE(cpuinfo_isa.clflush);
}

TEST(ISA, clflushopt) {
	ASSERT_FALSE(cpuinfo_isa.clflushopt);
}

TEST(ISA, mwait) {
	ASSERT_TRUE(cpuinfo_isa.mwait);
}

TEST(ISA, mwaitx) {
	ASSERT_FALSE(cpuinfo_isa.mwaitx);
}

TEST(ISA, emmx) {
	ASSERT_FALSE(cpuinfo_isa.emmx);
}

TEST(ISA, fxsave) {
	ASSERT_TRUE(cpuinfo_isa.fxsave);
}

TEST(ISA, xsave) {
	ASSERT_FALSE(cpuinfo_isa.xsave);
}

TEST(ISA, fpu) {
	ASSERT_TRUE(cpuinfo_isa.fpu);
}

TEST(ISA, mmx) {
	ASSERT_TRUE(cpuinfo_isa.mmx);
}

TEST(ISA, mmx_plus) {
	ASSERT_TRUE(cpuinfo_isa.mmx_plus);
}

TEST(ISA, three_d_now) {
	ASSERT_FALSE(cpuinfo_isa.three_d_now);
}

TEST(ISA, three_d_now_plus) {
	ASSERT_FALSE(cpuinfo_isa.three_d_now_plus);
}

TEST(ISA, three_d_now_geode) {
	ASSERT_FALSE(cpuinfo_isa.three_d_now_geode);
}

TEST(ISA, prefetch) {
	ASSERT_FALSE(cpuinfo_isa.prefetch);
}

TEST(ISA, prefetchw) {
	ASSERT_TRUE(cpuinfo_isa.prefetchw);
}

TEST(ISA, prefetchwt1) {
	ASSERT_FALSE(cpuinfo_isa.prefetchwt1);
}

TEST(ISA, daz) {
	ASSERT_TRUE(cpuinfo_isa.daz);
}

TEST(ISA, sse) {
	ASSERT_TRUE(cpuinfo_isa.sse);
}

TEST(ISA, sse2) {
	ASSERT_TRUE(cpuinfo_isa.sse2);
}

TEST(ISA, sse3) {
	ASSERT_TRUE(cpuinfo_isa.sse3);
}

TEST(ISA, ssse3) {
	ASSERT_TRUE(cpuinfo_isa.ssse3);
}

TEST(ISA, sse4_1) {
	ASSERT_TRUE(cpuinfo_isa.sse4_1);
}

TEST(ISA, sse4_2) {
	ASSERT_TRUE(cpuinfo_isa.sse4_2);
}

TEST(ISA, sse4a) {
	ASSERT_FALSE(cpuinfo_isa.sse4a);
}

TEST(ISA, misaligned_sse) {
	ASSERT_FALSE(cpuinfo_isa.misaligned_sse);
}

TEST(ISA, avx) {
	ASSERT_FALSE(cpuinfo_isa.avx);
}

TEST(ISA, fma3) {
	ASSERT_FALSE(cpuinfo_isa.fma3);
}

TEST(ISA, fma4) {
	ASSERT_FALSE(cpuinfo_isa.fma4);
}

TEST(ISA, xop) {
	ASSERT_FALSE(cpuinfo_isa.xop);
}

TEST(ISA, f16c) {
	ASSERT_FALSE(cpuinfo_isa.f16c);
}

TEST(ISA, avx2) {
	ASSERT_FALSE(cpuinfo_isa.avx2);
}

TEST(ISA, avx512f) {
	ASSERT_FALSE(cpuinfo_isa.avx512f);
}

TEST(ISA, avx512pf) {
	ASSERT_FALSE(cpuinfo_isa.avx512pf);
}

TEST(ISA, avx512er) {
	ASSERT_FALSE(cpuinfo_isa.avx512er);
}

TEST(ISA, avx512cd) {
	ASSERT_FALSE(cpuinfo_isa.avx512cd);
}

TEST(ISA, avx512dq) {
	ASSERT_FALSE(cpuinfo_isa.avx512dq);
}

TEST(ISA, avx512bw) {
	ASSERT_FALSE(cpuinfo_isa.avx512bw);
}

TEST(ISA, avx512vl) {
	ASSERT_FALSE(cpuinfo_isa.avx512vl);
}

TEST(ISA, avx512ifma) {
	ASSERT_FALSE(cpuinfo_isa.avx512ifma);
}

TEST(ISA, avx512vbmi) {
	ASSERT_FALSE(cpuinfo_isa.avx512vbmi);
}

TEST(ISA, avx512vpopcntdq) {
	ASSERT_FALSE(cpuinfo_isa.avx512vpopcntdq);
}

TEST(ISA, avx512_4vnniw) {
	ASSERT_FALSE(cpuinfo_isa.avx512_4vnniw);
}

TEST(ISA, avx512_4fmaps) {
	ASSERT_FALSE(cpuinfo_isa.avx512_4fmaps);
}

TEST(ISA, hle) {
	ASSERT_FALSE(cpuinfo_isa.hle);
}

TEST(ISA, rtm) {
	ASSERT_FALSE(cpuinfo_isa.rtm);
}

TEST(ISA, xtest) {
	ASSERT_FALSE(cpuinfo_isa.xtest);
}

TEST(ISA, mpx) {
	ASSERT_FALSE(cpuinfo_isa.mpx);
}

TEST(ISA, cmov) {
	ASSERT_TRUE(cpuinfo_isa.cmov);
}

TEST(ISA, cmpxchg8b) {
	ASSERT_TRUE(cpuinfo_isa.cmpxchg8b);
}

TEST(ISA, cmpxchg16b) {
	ASSERT_TRUE(cpuinfo_isa.cmpxchg16b);
}

TEST(ISA, clwb) {
	ASSERT_FALSE(cpuinfo_isa.clwb);
}

TEST(ISA, movbe) {
	ASSERT_TRUE(cpuinfo_isa.movbe);
}

TEST(ISA, fs_gs_base) {
	ASSERT_FALSE(cpuinfo_isa.fs_gs_base);
}

TEST(ISA, lzcnt) {
	ASSERT_FALSE(cpuinfo_isa.lzcnt);
}

TEST(ISA, popcnt) {
	ASSERT_TRUE(cpuinfo_isa.popcnt);
}

TEST(ISA, tbm) {
	ASSERT_FALSE(cpuinfo_isa.tbm);
}

TEST(ISA, bmi) {
	ASSERT_FALSE(cpuinfo_isa.bmi);
}

TEST(ISA, bmi2) {
	ASSERT_FALSE(cpuinfo_isa.bmi2);
}

TEST(ISA, adx) {
	ASSERT_FALSE(cpuinfo_isa.adx);
}

TEST(ISA, aes) {
	ASSERT_TRUE(cpuinfo_isa.aes);
}

TEST(ISA, pclmulqdq) {
	ASSERT_TRUE(cpuinfo_isa.pclmulqdq);
}

TEST(ISA, rdrand) {
	ASSERT_TRUE(cpuinfo_isa.rdrand);
}

TEST(ISA, rdseed) {
	ASSERT_FALSE(cpuinfo_isa.rdseed);
}

TEST(ISA, sha) {
	ASSERT_FALSE(cpuinfo_isa.sha);
}

TEST(ISA, rng) {
	ASSERT_FALSE(cpuinfo_isa.rng);
}

TEST(ISA, ace) {
	ASSERT_FALSE(cpuinfo_isa.ace);
}

TEST(ISA, ace2) {
	ASSERT_FALSE(cpuinfo_isa.ace2);
}

TEST(ISA, phe) {
	ASSERT_FALSE(cpuinfo_isa.phe);
}

TEST(ISA, pmm) {
	ASSERT_FALSE(cpuinfo_isa.pmm);
}

TEST(ISA, lwp) {
	ASSERT_FALSE(cpuinfo_isa.lwp);
}

TEST(ISA, rdtscp) {
	ASSERT_TRUE(cpuinfo_isa.rdtscp);
}

TEST(ISA, rdpid) {
	ASSERT_FALSE(cpuinfo_isa.rdpid);
}

TEST(L1I, count) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	ASSERT_EQ(4, l1i.count);
}

TEST(L1I, non_null) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	ASSERT_TRUE(l1i.instances);
}

TEST(L1I, size) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(32 * 1024, l1i.instances[k].size);
	}
}

TEST(L1I, associativity) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(8, l1i.instances[k].associativity);
	}
}

TEST(L1I, sets) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(l1i.instances[k].size,
			l1i.instances[k].sets * l1i.instances[k].line_size * l1i.instances[k].partitions * l1i.instances[k].associativity);
	}
}

TEST(L1I, partitions) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(1, l1i.instances[k].partitions);
	}
}

TEST(L1I, line_size) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(64, l1i.instances[k].line_size);
	}
}

TEST(L1I, flags) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(0, l1i.instances[k].flags);
	}
}

TEST(L1I, processors) {
	cpuinfo_caches l1i = cpuinfo_get_l1i_cache();
	for (uint32_t k = 0; k < l1i.count; k++) {
		ASSERT_EQ(k, l1i.instances[k].processor_start);
		ASSERT_EQ(1, l1i.instances[k].processor_count);
	}
}

TEST(L1D, count) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	ASSERT_EQ(4, l1d.count);
}

TEST(L1D, non_null) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	ASSERT_TRUE(l1d.instances);
}

TEST(L1D, size) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(24 * 1024, l1d.instances[k].size);
	}
}

TEST(L1D, associativity) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(6, l1d.instances[k].associativity);
	}
}

TEST(L1D, sets) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(l1d.instances[k].size,
			l1d.instances[k].sets * l1d.instances[k].line_size * l1d.instances[k].partitions * l1d.instances[k].associativity);
	}
}

TEST(L1D, partitions) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(1, l1d.instances[k].partitions);
	}
}

TEST(L1D, line_size) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(64, l1d.instances[k].line_size);
	}
}

TEST(L1D, flags) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(0, l1d.instances[k].flags);
	}
}

TEST(L1D, processors) {
	cpuinfo_caches l1d = cpuinfo_get_l1d_cache();
	for (uint32_t k = 0; k < l1d.count; k++) {
		ASSERT_EQ(k, l1d.instances[k].processor_start);
		ASSERT_EQ(1, l1d.instances[k].processor_count);
	}
}

TEST(L2, count) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	ASSERT_EQ(2, l2.count);
}

TEST(L2, non_null) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	ASSERT_TRUE(l2.instances);
}

TEST(L2, size) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(1024 * 1024, l2.instances[k].size);
	}
}

TEST(L2, associativity) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(16, l2.instances[k].associativity);
	}
}

TEST(L2, sets) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(l2.instances[k].size,
			l2.instances[k].sets * l2.instances[k].line_size * l2.instances[k].partitions * l2.instances[k].associativity);
	}
}

TEST(L2, partitions) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(1, l2.instances[k].partitions);
	}
}

TEST(L2, line_size) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(64, l2.instances[k].line_size);
	}
}

TEST(L2, flags) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(CPUINFO_CACHE_UNIFIED, l2.instances[k].flags);
	}
}

TEST(L2, processors) {
	cpuinfo_caches l2 = cpuinfo_get_l2_cache();
	for (uint32_t k = 0; k < l2.count; k++) {
		ASSERT_EQ(k * 2, l2.instances[k].processor_start);
		ASSERT_EQ(2, l2.instances[k].processor_count);
	}
}

TEST(L3, none) {
	cpuinfo_caches l3 = cpuinfo_get_l3_cache();
	ASSERT_EQ(0, l3.count);
	ASSERT_FALSE(l3.instances);
}

TEST(L4, none) {
	cpuinfo_caches l4 = cpuinfo_get_l4_cache();
	ASSERT_EQ(0, l4.count);
	ASSERT_FALSE(l4.instances);
}

#include <memo-pad-7.h>

int main(int argc, char* argv[]) {
	cpuinfo_mock_filesystem(filesystem);
	cpuinfo_mock_set_cpuid(cpuid_dump, sizeof(cpuid_dump) / sizeof(cpuinfo_mock_cpuid));
	cpuinfo_initialize();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
