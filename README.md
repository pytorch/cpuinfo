# CPU INFOrmation library

cpuinfo is a library to detect essential for performance optimization information about host CPU. Planned features:

- Processor name detection
  - [ ] Using CPUID leaves 0x80000002–0x80000004 on x86/x86-64
  - [ ] Using `/proc/cpuinfo` on ARM
  - [ ] Using kernel log (`dmesg`) on ARM
  - [ ] Using `ro.chipname`, `ro.board.platform`, `ro.product.board` properties (Android)
- Instruction set detection
  - [x] Using CPUID on x86/x86-64 (Linux, Mach)
  - [x] Using dynamic code generation validator on x86-64 (Native Client)
  - [x] Using `/proc/cpuinfo` on 32-bit ARM EABI (Linux)
  - [x] Using microarchitecture heuristics on 32-bit ARM
  - [x] Using `FPSID` and `WCID` registers on 32-bit ARM
  - [ ] Using `getauxval` or `/proc/self/auxv` (Linux)
  - [ ] Using instruction probing on ARM (Linux)
- Cache detection
  - [x] Using CPUID leaf 0x00000002 on x86/x86-64
  - [x] Using CPUID leaf 0x00000004 on non-AMD x86/x86-64
  - [ ] Using CPUID leaves 0x80000005-0x80000006 on AMD x86/x86-64
  - [x] Using CPUID leaf 0x8000001D on AMD x86/x86-64
  - [x] Using `/proc/cpuinfo` on ARMv6 and earlier (Linux)
  - [x] Using microarchitecture heuristics on 32-bit ARM
  - [x] Using `sysctlbyname` (Mach)
  - [ ] Using sysfs (Linux)
  - [ ] Using `clGetDeviceInfo` with `CL_DEVICE_GLOBAL_MEM_CACHE_SIZE`/`CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE` parameters (Android)
- TLB detection
  - [x] Using CPUID leaf 0x00000002 on x86/x86-64
  - [ ] Using CPUID leaves 0x80000005-0x80000006 and 0x80000019 on AMD x86/x86-64
  - [x] Using microarchitecture heuristics on 32-bit ARM
- Topology detection
  - [x] Using CPUID leaf 0x00000001 on x86/x86-64 (legacy APIC ID)
  - [x] Using CPUID leaf 0x0000000B on x86/x86-64 (Intel APIC ID)
  - [ ] Using CPUID leaf 0x8000001E on x86/x86-64 (AMD APIC ID)
  - [x] Using `host_info` (Mach)
  - [ ] Using sysfs (Linux)
