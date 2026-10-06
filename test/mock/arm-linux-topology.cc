#include <gtest/gtest.h>

#include <cpuinfo-mock.h>
#include <cpuinfo.h>

#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

struct test_cpu {
	uint32_t part;
	cpuinfo_uarch uarch;
	uint32_t frequency;
	uint32_t group;
};

static std::vector<test_cpu> input;
static std::vector<uint32_t> expected_linux_ids;
static std::vector<uint32_t> expected_cluster_sizes;
static uint32_t expected_uarchs;
static uint32_t missing_midr = UINT32_MAX;
static bool mixed_caches;

template <class T>
static bool in_array(const T* pointer, const T* array, uint32_t count) {
	const uintptr_t offset = reinterpret_cast<uintptr_t>(pointer) - reinterpret_cast<uintptr_t>(array);
	return pointer != nullptr && array != nullptr && offset % sizeof(T) == 0 && offset / sizeof(T) < count;
}

class ARM_LINUX_TOPOLOGY : public ::testing::Test {
	void SetUp() override {
		ASSERT_EQ(input.size(), cpuinfo_get_processors_count());
		ASSERT_EQ(input.size(), cpuinfo_get_cores_count());
	}
};

TEST_F(ARM_LINUX_TOPOLOGY, per_cpu_identity_and_order) {
	ASSERT_EQ(expected_uarchs, cpuinfo_get_uarchs_count());
	for (uint32_t i = 0; i < input.size(); i++) {
		const auto* processor = cpuinfo_get_processor(i);
		const uint32_t linux_id = expected_linux_ids[i];
		ASSERT_EQ(linux_id, processor->linux_id);
		ASSERT_EQ(cpuinfo_get_core(i), processor->core);
		EXPECT_EQ(input[linux_id].uarch, processor->core->uarch);
		EXPECT_EQ(UINT32_C(0x410F0001) | (input[linux_id].part << 4), processor->core->midr);
		EXPECT_EQ(cpuinfo_vendor_arm, processor->core->vendor);
	}
}

TEST_F(ARM_LINUX_TOPOLOGY, homogeneous_cluster_ranges) {
	ASSERT_EQ(expected_cluster_sizes.size(), cpuinfo_get_clusters_count());
	ASSERT_EQ(expected_cluster_sizes.size(), cpuinfo_get_package(0)->cluster_count);
	uint32_t start = 0;
	for (uint32_t i = 0; i < expected_cluster_sizes.size(); i++) {
		const auto* cluster = cpuinfo_get_cluster(i);
		ASSERT_EQ(i, cluster->cluster_id);
		ASSERT_EQ(start, cluster->processor_start);
		ASSERT_EQ(start, cluster->core_start);
		ASSERT_EQ(expected_cluster_sizes[i], cluster->processor_count);
		ASSERT_EQ(expected_cluster_sizes[i], cluster->core_count);
		ASSERT_LE(start + cluster->processor_count, input.size());
		for (uint32_t j = start; j < start + cluster->processor_count; j++) {
			ASSERT_EQ(cluster, cpuinfo_get_processor(j)->cluster);
			ASSERT_EQ(cluster, cpuinfo_get_core(j)->cluster);
			EXPECT_EQ(cluster->uarch, cpuinfo_get_core(j)->uarch);
			EXPECT_EQ(cluster->midr, cpuinfo_get_core(j)->midr);
		}
		start += cluster->processor_count;
	}
	EXPECT_EQ(input.size(), start);
}

TEST_F(ARM_LINUX_TOPOLOGY, cache_pointer_and_range_consistency) {
	const cpuinfo_cache* arrays[] = {
		cpuinfo_get_l1i_caches(), cpuinfo_get_l1d_caches(), cpuinfo_get_l2_caches(), cpuinfo_get_l3_caches()};
	const uint32_t counts[] = {
		cpuinfo_get_l1i_caches_count(),
		cpuinfo_get_l1d_caches_count(),
		cpuinfo_get_l2_caches_count(),
		cpuinfo_get_l3_caches_count()};
	for (uint32_t i = 0; i < input.size(); i++) {
		const auto* processor = cpuinfo_get_processor(i);
		const cpuinfo_cache* caches[] = {
			processor->cache.l1i, processor->cache.l1d, processor->cache.l2, processor->cache.l3};
		for (uint32_t level = 0; level < 4; level++) {
			if (caches[level] != nullptr) {
				ASSERT_TRUE(in_array(caches[level], arrays[level], counts[level]));
				EXPECT_LE(caches[level]->processor_start, i);
				EXPECT_GT(caches[level]->processor_start + caches[level]->processor_count, i);
			}
		}
	}
	for (uint32_t level = 0; level < 4; level++) {
		for (uint32_t i = 0; i < counts[level]; i++) {
			const auto* cache = &arrays[level][i];
			ASSERT_NE(0, cache->processor_count);
			ASSERT_LE(cache->processor_start + cache->processor_count, input.size());
			for (uint32_t j = cache->processor_start; j < cache->processor_start + cache->processor_count;
			     j++) {
				const auto* processor = cpuinfo_get_processor(j);
				const cpuinfo_cache* caches[] = {
					processor->cache.l1i,
					processor->cache.l1d,
					processor->cache.l2,
					processor->cache.l3};
				EXPECT_EQ(cache, caches[level]);
			}
		}
	}
	if (mixed_caches) {
		// The A715 between two modeled core groups has no modeled L3.
		ASSERT_EQ(2, cpuinfo_get_l3_caches_count());
		EXPECT_EQ(cpuinfo_get_l3_cache(0), cpuinfo_get_processor(0)->cache.l3);
		EXPECT_EQ(nullptr, cpuinfo_get_processor(1)->cache.l3);
		EXPECT_EQ(cpuinfo_get_l3_cache(1), cpuinfo_get_processor(2)->cache.l3);
		EXPECT_EQ(cpuinfo_get_l3_cache(1), cpuinfo_get_processor(3)->cache.l3);
	}
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	const std::string mode = argc > 1 ? argv[1] : "pixel-10";
	// Synthetic inputs reproduce the topology patterns in #316. Pixel 10
	// parts and frequencies match the device used for live validation.
	if (mode == "pixel-8") {
		input = {
			{0xD46, cpuinfo_uarch_cortex_a510, 1704000, 0},
			{0xD46, cpuinfo_uarch_cortex_a510, 1704000, 0},
			{0xD46, cpuinfo_uarch_cortex_a510, 1704000, 0},
			{0xD46, cpuinfo_uarch_cortex_a510, 1704000, 0},
			{0xD4D, cpuinfo_uarch_cortex_a715, 2361000, 0},
			{0xD4D, cpuinfo_uarch_cortex_a715, 2361000, 0},
			{0xD4D, cpuinfo_uarch_cortex_a715, 2361000, 0},
			{0xD4D, cpuinfo_uarch_cortex_a715, 2361000, 0},
			{0xD4E, cpuinfo_uarch_cortex_x3, 2914000, 0}};
		expected_linux_ids = {8, 4, 5, 6, 7, 0, 1, 2, 3};
		expected_cluster_sizes = {1, 4, 4};
		expected_uarchs = 3;
	} else if (mode == "pixel-9") {
		input = {
			{0xD80, cpuinfo_uarch_cortex_a520, 1920000, 0},
			{0xD80, cpuinfo_uarch_cortex_a520, 1920000, 0},
			{0xD80, cpuinfo_uarch_cortex_a520, 1920000, 0},
			{0xD80, cpuinfo_uarch_cortex_a520, 1920000, 0},
			{0xD81, cpuinfo_uarch_cortex_a720, 2600000, 0},
			{0xD81, cpuinfo_uarch_cortex_a720, 2600000, 0},
			{0xD81, cpuinfo_uarch_cortex_a720, 2600000, 0},
			{0xD82, cpuinfo_uarch_cortex_x4, 3100000, 0}};
		expected_linux_ids = {7, 4, 5, 6, 0, 1, 2, 3};
		expected_cluster_sizes = {1, 3, 4};
		expected_uarchs = 3;
	} else if (mode == "pixel-10" || mode == "missing-midr") {
		input = {
			{0xD80, cpuinfo_uarch_cortex_a520, 2246000, 0},
			{0xD80, cpuinfo_uarch_cortex_a520, 2246000, 0},
			{0xD87, cpuinfo_uarch_cortex_a725, 3052000, 0},
			{0xD87, cpuinfo_uarch_cortex_a725, 3052000, 0},
			{0xD87, cpuinfo_uarch_cortex_a725, 3052000, 0},
			{0xD87, cpuinfo_uarch_cortex_a725, 3052000, 0},
			{0xD87, cpuinfo_uarch_cortex_a725, 3052000, 0},
			{0xD82, cpuinfo_uarch_cortex_x4, 3782000, 0}};
		expected_linux_ids = {7, 2, 3, 4, 5, 6, 0, 1};
		expected_cluster_sizes = {1, 5, 2};
		expected_uarchs = 3;
		if (mode == "missing-midr") {
			missing_midr = 1;
		}
	} else if (mode == "galaxy-s23") {
		// A710 and A715 share the middle kernel topology group.
		input = {
			{0xD46, cpuinfo_uarch_cortex_a510, 2000000, 0},
			{0xD46, cpuinfo_uarch_cortex_a510, 2000000, 0},
			{0xD46, cpuinfo_uarch_cortex_a510, 2000000, 0},
			{0xD47, cpuinfo_uarch_cortex_a710, 2800000, 1},
			{0xD47, cpuinfo_uarch_cortex_a710, 2800000, 1},
			{0xD4D, cpuinfo_uarch_cortex_a715, 2800000, 1},
			{0xD4D, cpuinfo_uarch_cortex_a715, 2800000, 1},
			{0xD4E, cpuinfo_uarch_cortex_x3, 3360000, 2}};
		expected_linux_ids = {7, 3, 4, 5, 6, 0, 1, 2};
		expected_cluster_sizes = {1, 2, 2, 3};
		expected_uarchs = 4;
	} else if (mode == "frequency-groups") {
		input = {
			{0xD05, cpuinfo_uarch_cortex_a55r0, 1000000, 0},
			{0xD05, cpuinfo_uarch_cortex_a55r0, 1000000, 0},
			{0xD05, cpuinfo_uarch_cortex_a55r0, 2000000, 0},
			{0xD05, cpuinfo_uarch_cortex_a55r0, 2000000, 0}};
		expected_linux_ids = {2, 3, 0, 1};
		expected_cluster_sizes = {2, 2};
		expected_uarchs = 1;
	} else if (mode == "unknown-parts") {
		input = {
			{0xFFE, cpuinfo_uarch_unknown, 1000000, 0},
			{0xFFE, cpuinfo_uarch_unknown, 1000000, 0},
			{0xFFF, cpuinfo_uarch_unknown, 1000000, 0},
			{0xFFF, cpuinfo_uarch_unknown, 1000000, 0}};
		expected_linux_ids = {0, 1, 2, 3};
		expected_cluster_sizes = {2, 2};
		expected_uarchs = 1;
	} else if (mode == "mixed-caches") {
		input = {
			{0xD05, cpuinfo_uarch_cortex_a55r0, 1000000, 0},
			{0xD05, cpuinfo_uarch_cortex_a55r0, 1000000, 0},
			{0xD4D, cpuinfo_uarch_cortex_a715, 2000000, 0},
			{0xD0D, cpuinfo_uarch_cortex_a77, 3000000, 0}};
		expected_linux_ids = {3, 2, 0, 1};
		expected_cluster_sizes = {1, 1, 2};
		expected_uarchs = 3;
		mixed_caches = true;
	} else {
		return 1;
	}

	std::deque<std::string> paths, contents;
	std::vector<cpuinfo_mock_file> files;
	const auto add_file = [&](const std::string& path, const std::string& content) {
		paths.push_back(path);
		contents.push_back(content);
		files.push_back({paths.back().c_str(), contents.back().size(), contents.back().c_str(), 0});
	};
	std::string proc_cpuinfo;
	for (uint32_t i = 0; i < input.size(); i++) {
		proc_cpuinfo += "processor\t: " + std::to_string(i) + "\n";
		if (i != missing_midr) {
			char identity[160];
			std::snprintf(
				identity,
				sizeof(identity),
				"CPU implementer\t: 0x41\nCPU architecture: 8\nCPU variant\t: 0x0\n"
				"CPU part\t: 0x%x\nCPU revision\t: 1\n",
				input[i].part);
			proc_cpuinfo += identity;
		}
		proc_cpuinfo += "\n";
		const std::string prefix = "/sys/devices/system/cpu/cpu" + std::to_string(i);
		uint32_t start = i, end = i;
		while (start > 0 && input[start - 1].group == input[i].group)
			start--;
		while (end + 1 < input.size() && input[end + 1].group == input[i].group)
			end++;
		add_file(prefix + "/topology/physical_package_id", "0\n");
		add_file(
			prefix + "/topology/cluster_cpus_list",
			std::to_string(start) + "-" + std::to_string(end) + "\n");
		add_file(prefix + "/cpufreq/cpuinfo_max_freq", std::to_string(input[i].frequency) + "\n");
	}
	add_file("/proc/cpuinfo", proc_cpuinfo);
	add_file("/sys/devices/system/cpu/kernel_max", std::to_string(input.size() - 1) + "\n");
	add_file("/sys/devices/system/cpu/possible", "0-" + std::to_string(input.size() - 1) + "\n");
	add_file("/sys/devices/system/cpu/present", "0-" + std::to_string(input.size() - 1) + "\n");
	if (mixed_caches) {
		// Also exercise a private L2 with no modeled L3 between the two groups.
		const std::string cache = "/sys/devices/system/cpu/cpu2/cache/index2/";
		add_file(cache + "level", "2\n");
		add_file(cache + "size", "256K\n");
		add_file(cache + "shared_cpu_list", "2\n");
	}
	files.push_back({nullptr, 0, nullptr, 0});
	cpuinfo_mock_filesystem(files.data());
#ifdef __ANDROID__
	cpuinfo_mock_property properties[] = {{nullptr, nullptr}};
	cpuinfo_mock_android_properties(properties);
#endif
	if (!cpuinfo_initialize()) {
		return 1;
	}
	return RUN_ALL_TESTS();
}
