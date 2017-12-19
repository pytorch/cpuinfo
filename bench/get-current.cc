#include <benchmark/benchmark.h>

#include <cpuinfo.h>


static void cpuinfo_get_current_processor(benchmark::State& state) {
	cpuinfo_initialize();
	while (state.KeepRunning()) {
		const cpuinfo_processor* current_processor = cpuinfo_get_current_processor();
		benchmark::DoNotOptimize(current_processor);
	}
}
BENCHMARK(cpuinfo_get_current_processor)->Unit(benchmark::kNanosecond);

static void cpuinfo_get_current_core(benchmark::State& state) {
	cpuinfo_initialize();
	while (state.KeepRunning()) {
		const cpuinfo_core* current_core = cpuinfo_get_current_core();
		benchmark::DoNotOptimize(current_core);
	}
}
BENCHMARK(cpuinfo_get_current_core)->Unit(benchmark::kNanosecond);

BENCHMARK_MAIN();
