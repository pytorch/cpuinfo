# CPU INFOrmation library

[![BSD (2 clause) License](https://img.shields.io/badge/License-BSD%202--Clause%20%22Simplified%22%20License-blue.svg)](https://github.com/pytorch/cpuinfo/blob/master/LICENSE)
[![Linux/Mac build status](https://img.shields.io/travis/pytorch/cpuinfo.svg)](https://travis-ci.org/pytorch/cpuinfo)
[![Windows build status](https://ci.appveyor.com/api/projects/status/g5khy9nr0xm458t7/branch/master?svg=true)](https://ci.appveyor.com/project/MaratDukhan/cpuinfo/branch/master)

cpuinfo is a library to detect essential for performance optimization information about host CPU.

## Features

- **Cross-platform** availability:
  - Linux, Windows, macOS, Android, and iOS operating systems
  - x86, x86-64, ARM, and ARM64 architectures
- Modern **C/C++ interface**
  - Thread-safe
  - No memory allocation after initialization
  - No exceptions thrown
- Detection of **supported instruction sets**, up to AVX512 (x86) and ARMv8.3 extensions
- Detection of SoC and core information:
  - **Processor (SoC) name**
  - Vendor and **microarchitecture** for each CPU core
  - ID (**MIDR** on ARM, **CPUID** leaf 1 EAX value on x86) for each CPU core
- Detection of **cache information**:
  - Cache type (instruction/data/unified), size and line size
  - Cache associativity
  - Cores and logical processors (hyper-threads) sharing the cache
- Detection of **topology information** (relative between logical processors, cores, and processor packages)
- Well-tested **production-quality** code:
  - 60+ mock tests based on data from real devices
  - Includes work-arounds for common bugs in hardware and OS kernels
  - Supports systems with heterogenous cores, such as **big.LITTLE** and Max.Med.Min
- Permissive **open-source** license (Simplified BSD)

## Examples

Log processor name:

```c
cpuinfo_initialize();
printf("Running on %s CPU\n", cpuinfo_get_package(0)->name);
```

Detect if target is a 32-bit or 64-bit ARM system:

```c
#if CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
    /* 32-bit ARM-specific code here */
#endif
```

Check if the host CPU support ARM NEON
```c
cpuinfo_initialize();
if (cpuinfo_has_arm_neon()) {
    neon_implementation(arguments);
}
```

Check if the host CPU supports x86 AVX
```c
cpuinfo_initialize();
if (cpuinfo_has_x86_avx()) {
    avx_implementation(arguments);
}
```

Check if the thread runs on a Cortex-A53 core
```c
cpuinfo_initialize();
switch (cpuinfo_get_current_core()->uarch) {
    case cpuinfo_uarch_cortex_a53:
        cortex_a53_implementation(arguments);
        break;
    default:
        generic_implementation(arguments);
        break;
}
```

Get the size of level 1 data cache on the fastest core in the processor (e.g. big core in big.LITTLE ARM systems):
```c
cpuinfo_initialize();
const size_t l1_size = cpuinfo_get_processor(0)->cache.l1d->size;
```

Pin thread to cores sharing L2 cache with the current core (Linux or Android)
```c
cpuinfo_initialize();
cpu_set_t cpu_set;
CPU_ZERO(&cpu_set);
const struct cpuinfo_cache* current_l2 = cpuinfo_get_current_processor()->cache.l2;
for (uint32_t i = 0; i < current_l2->processor_count; i++) {
    CPU_SET(cpuinfo_get_processor(current_l2->processor_start + i)->linux_id, &cpu_set);
}
pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu_set);
```

## Exposed information
- [x] Processor (SoC) name
- [x] Microarchitecture
- [x] Usable instruction sets
- [ ] CPU frequency
- [x] Cache
  - [x] Size
  - [x] Associativity
  - [x] Line size
  - [x] Number of partitions
  - [x] Flags (unified, inclusive, complex hash function)
  - [x] Topology (logical processors that share this cache level)
- [ ] TLB
  - [ ] Number of entries
  - [ ] Associativity
  - [ ] Covered page types (instruction, data)
  - [ ] Covered page sizes
- [x] Topology information
  - [x] Logical processors
  - [x] Cores
  - [x] Packages (sockets)

## Supported environments:
- [x] Android
  - [x] x86 ABI
  - [x] x86_64 ABI
  - [x] armeabi ABI
  - [x] armeabiv7-a ABI
  - [x] arm64-v8a ABI
  - [ ] ~~mips ABI~~
  - [ ] ~~mips64 ABI~~
- [x] Linux
  - [x] x86
  - [x] x86-64
  - [x] 32-bit ARM (ARMv5T and later)
  - [x] ARM64
  - [ ] PowerPC64
- [x] iOS
  - [x] x86 (iPhone simulator)
  - [x] x86-64 (iPhone simulator)
  - [x] ARMv7
  - [x] ARM64
- [x] OS X
  - [x] x86
  - [x] x86-64
- [x] Windows
  - [x] x86
  - [x] x86-64

## Methods

- Processor (SoC) name detection
  - [x] Using CPUID leaves 0x80000002–0x80000004 on x86/x86-64
  - [x] Using `/proc/cpuinfo` on ARM
  - [x] Using `ro.chipname`, `ro.board.platform`, `ro.product.board`, `ro.mediatek.platform`, `ro.arch` properties (Android)
  - [ ] Using kernel log (`dmesg`) on ARM Linux
- Vendor and microarchitecture detection
  - [x] Intel-designed x86/x86-64 cores (up to Kaby Lake, Airmont, and Knights Mill)
  - [x] AMD-designed x86/x86-64 cores (up to Puma/Jaguar and Zen)
  - [ ] VIA-designed x86/x86-64 cores
  - [ ] Other x86 cores (DM&P, RDC, Transmeta, Cyrix, Rise)
  - [x] ARM-designed ARM cores (up to Cortex-A55 and Cortex-A75)
  - [x] Qualcomm-designed ARM cores (up to Kryo, Kryo-280, and Kryo-385)
  - [x] Nvidia-designed ARM cores (Denver)
  - [x] Samsung-designed ARM cores (Mongoose and Meerkat)
  - [x] Intel-designed ARM cores (XScale up to 3rd-gen)
  - [x] Apple-designed ARM cores (up to Hurricane)
  - [x] Cavium-designed ARM cores (ThunderX)
  - [x] AppliedMicro-designed ARM cores (X-Gene)
- Instruction set detection
  - [x] Using CPUID (x86/x86-64)
  - [x] Using dynamic code generation validator (Native Client/x86-64)
  - [x] Using `/proc/cpuinfo` on 32-bit ARM EABI (Linux)
  - [x] Using microarchitecture heuristics on (32-bit ARM)
  - [x] Using `FPSID` and `WCID` registers (32-bit ARM)
  - [x] Using `getauxval` (Linux/ARM)
  - [x] Using `/proc/self/auxv` (Android/ARM)
  - [ ] Using instruction probing on ARM (Linux)
  - [ ] Using CPUID registers on ARM64 (Linux)
- Cache detection
  - [x] Using CPUID leaf 0x00000002 (x86/x86-64)
  - [x] Using CPUID leaf 0x00000004 (non-AMD x86/x86-64)
  - [ ] Using CPUID leaves 0x80000005-0x80000006 (AMD x86/x86-64)
  - [x] Using CPUID leaf 0x8000001D (AMD x86/x86-64)
  - [x] Using `/proc/cpuinfo` (Linux/pre-ARMv7)
  - [x] Using microarchitecture heuristics (ARM)
  - [x] Using chipset name (ARM)
  - [x] Using `sysctlbyname` (Mach)
  - [x] Using sysfs `typology` directories (ARM/Linux)
  - [ ] Using sysfs `cache` directories (Linux)
- TLB detection
  - [x] Using CPUID leaf 0x00000002 (x86/x86-64)
  - [ ] Using CPUID leaves 0x80000005-0x80000006 and 0x80000019 (AMD x86/x86-64)
  - [x] Using microarchitecture heuristics (ARM)
- Topology detection
  - [x] Using CPUID leaf 0x00000001 on x86/x86-64 (legacy APIC ID)
  - [x] Using CPUID leaf 0x0000000B on x86/x86-64 (Intel APIC ID)
  - [ ] Using CPUID leaf 0x8000001E on x86/x86-64 (AMD APIC ID)
  - [x] Using `/proc/cpuinfo` (Linux)
  - [x] Using `host_info` (Mach)
  - [x] Using `GetLogicalProcessorInformationEx` (Windows)
  - [x] Using sysfs (Linux)
  - [x] Using chipset name (ARM/Linux)
