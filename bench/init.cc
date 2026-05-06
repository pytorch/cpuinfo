#include <benchmark/benchmark.h>

#include <cpuinfo.h>

static void cpuinfo_initialize_bench(benchmark::State& state) {
	while (state.KeepRunning()) {
		cpuinfo_initialize();
	}
}
BENCHMARK(cpuinfo_initialize_bench)->Iterations(1)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
