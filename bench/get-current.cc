#include <benchmark/benchmark.h>

#include <cpuinfo.h>

static void cpuinfo_get_current_processor_bench(benchmark::State& state) {
	cpuinfo_initialize();
	while (state.KeepRunning()) {
		const cpuinfo_processor* current_processor = cpuinfo_get_current_processor();
		benchmark::DoNotOptimize(current_processor);
	}
}
BENCHMARK(cpuinfo_get_current_processor_bench)->Unit(benchmark::kNanosecond);

static void cpuinfo_get_current_core_bench(benchmark::State& state) {
	cpuinfo_initialize();
	while (state.KeepRunning()) {
		const cpuinfo_core* current_core = cpuinfo_get_current_core();
		benchmark::DoNotOptimize(current_core);
	}
}
BENCHMARK(cpuinfo_get_current_core_bench)->Unit(benchmark::kNanosecond);

static void cpuinfo_get_current_uarch_index_bench(benchmark::State& state) {
	cpuinfo_initialize();
	while (state.KeepRunning()) {
		const uint32_t uarch_index = cpuinfo_get_current_uarch_index();
		benchmark::DoNotOptimize(uarch_index);
	}
}
BENCHMARK(cpuinfo_get_current_uarch_index_bench)->Unit(benchmark::kNanosecond);

static void cpuinfo_get_current_uarch_index_with_default_bench(benchmark::State& state) {
	cpuinfo_initialize();
	while (state.KeepRunning()) {
		const uint32_t uarch_index = cpuinfo_get_current_uarch_index_with_default(0);
		benchmark::DoNotOptimize(uarch_index);
	}
}
BENCHMARK(cpuinfo_get_current_uarch_index_with_default_bench)->Unit(benchmark::kNanosecond);

BENCHMARK_MAIN();
