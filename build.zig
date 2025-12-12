const std = @import("std");
const builtin = @import("builtin");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const os_tag = if (target.query.os_tag) |os_tag| os_tag else builtin.target.os.tag;
    const cpu_arch = if (target.query.cpu_arch) |cpu_arch| cpu_arch else builtin.target.cpu.arch;
    const abi = if (target.query.abi) |abi| abi else builtin.target.abi;

    const lib = b.addLibrary(.{
        .name = "cpuinfo",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        }),
        .linkage = if (os_tag == .macos and cpu_arch == .x86_64) .dynamic else .static,
    });

    lib.addIncludePath(b.path("include"));
    lib.addIncludePath(b.path("src"));

    var cflags: std.ArrayList([]const u8) = .empty;

    if (target.query.os_tag != .windows) {
        cflags.append(b.allocator, "-std=gnu99") catch unreachable; // gnu99, not c99
        cflags.append(b.allocator, "-Wno-vla") catch unreachable;
        cflags.append(b.allocator, "-D_GNU_SOURCE=1") catch unreachable; // CPU_SETSIZE
        cflags.append(b.allocator, "-DCPUINFO_INTERNAL=") catch unreachable;
        cflags.append(b.allocator, "-DCPUINFO_PRIVATE=") catch unreachable;
    }

    cflags.append(b.allocator, "-DCPUINFO_LOG_LEVEL=2") catch unreachable;
    cflags.append(b.allocator, "-Iinclude") catch unreachable;
    cflags.append(b.allocator, "-Isrc") catch unreachable;

    const COMMON_SRCS = [_][]const u8{
        "src/api.c",
        "src/cache.c",
        "src/init.c",
        "src/log.c",
    };

    const X86_SRCS = [_][]const u8{
        "src/x86/cache/descriptor.c",
        "src/x86/cache/deterministic.c",
        "src/x86/cache/init.c",
        "src/x86/info.c",
        "src/x86/init.c",
        "src/x86/isa.c",
        "src/x86/name.c",
        "src/x86/topology.c",
        "src/x86/uarch.c",
        "src/x86/vendor.c",
    };

    const ARM_SRCS = [_][]const u8{
        "src/arm/cache.c",
        "src/arm/uarch.c",
    };

    const RISCV_SRCS = [_][]const u8{
        "src/riscv/uarch.c",
    };

    const LINUX_SRCS = [_][]const u8{
        "src/linux/cpulist.c",
        "src/linux/multiline.c",
        "src/linux/processors.c",
        "src/linux/smallfile.c",
    };

    const MOCK_LINUX_SRCS = [_][]const u8{
        "src/linux/mockfile.c",
    };
    _ = MOCK_LINUX_SRCS;

    const MACH_SRCS = [_][]const u8{
        "src/mach/topology.c",
    };

    const FREEBSD_SRCS = [_][]const u8{
        "src/freebsd/topology.c",
    };

    const EMSCRIPTEN_SRCS = [_][]const u8{
        "src/emscripten/init.c",
    };

    const LINUX_X86_SRCS = [_][]const u8{
        "src/x86/linux/cpuinfo.c",
        "src/x86/linux/init.c",
    };

    const LINUX_ARM_SRCS = [_][]const u8{
        "src/arm/linux/chipset.c",
        "src/arm/linux/clusters.c",
        "src/arm/linux/cpuinfo.c",
        "src/arm/linux/hwcap.c",
        "src/arm/linux/init.c",
        "src/arm/linux/midr.c",
    };
    _ = LINUX_ARM_SRCS;

    const LINUX_ARM32_SRCS = [_][]const u8{
        // LINUX_ARM_SRCS +
        "src/arm/linux/chipset.c",
        "src/arm/linux/clusters.c",
        "src/arm/linux/cpuinfo.c",
        "src/arm/linux/hwcap.c",
        "src/arm/linux/init.c",
        "src/arm/linux/midr.c",
        "src/arm/linux/aarch32-isa.c",
    };

    const LINUX_ARM64_SRCS = [_][]const u8{
        // LINUX_ARM_SRCS +
        "src/arm/linux/chipset.c",
        "src/arm/linux/clusters.c",
        "src/arm/linux/cpuinfo.c",
        "src/arm/linux/hwcap.c",
        "src/arm/linux/init.c",
        "src/arm/linux/midr.c",
        "src/arm/linux/aarch64-isa.c",
    };

    const LINUX_RISCV_SRCS = [_][]const u8{
        "src/riscv/linux/init.c",
        "src/riscv/linux/riscv-isa.c",
        "src/riscv/linux/riscv-hw.c",
    };

    const ANDROID_ARM_SRCS = [_][]const u8{
        "src/arm/android/properties.c",
    };

    const WINDOWS_X86_SRCS = [_][]const u8{
        "src/x86/windows/init.c",
    };

    const WINDOWS_ARM_SRCS = [_][]const u8{
        "src/arm/windows/init-by-logical-sys-info.c",
        "src/arm/windows/init.c",
    };

    const MACH_X86_SRCS = [_][]const u8{
        "src/x86/mach/init.c",
    };

    const MACH_ARM_SRCS = [_][]const u8{
        "src/arm/mach/init.c",
    };

    const FREEBSD_X86_SRCS = [_][]const u8{
        "src/x86/freebsd/init.c",
    };

    lib.addCSourceFiles(.{
        .files = &COMMON_SRCS,
        .flags = cflags.items,
    });

    switch (os_tag) {
        .linux => {
            lib.addCSourceFiles(.{
                .files = &LINUX_SRCS,
                .flags = cflags.items,
            });

            switch (cpu_arch) {
                .x86_64 => {
                    // :linux_x86_64
                    lib.addCSourceFiles(.{
                        .files = &X86_SRCS,
                        .flags = cflags.items,
                    });
                    lib.addCSourceFiles(.{
                        .files = &LINUX_X86_SRCS,
                        .flags = cflags.items,
                    });
                },
                .arm, .thumb => {
                    // linux_arm* (arm, armhf, armv7a, armeabi, etc.)
                    lib.addCSourceFiles(.{
                        .files = &ARM_SRCS,
                        .flags = cflags.items,
                    });
                    lib.addCSourceFiles(.{
                        .files = &LINUX_ARM32_SRCS,
                        .flags = cflags.items,
                    });
                },
                .aarch64 => {
                    // :linux_aarch64
                    lib.addCSourceFiles(.{
                        .files = &ARM_SRCS,
                        .flags = cflags.items,
                    });
                    lib.addCSourceFiles(.{
                        .files = &LINUX_ARM64_SRCS,
                        .flags = cflags.items,
                    });
                },
                .riscv64, .riscv32 => {
                    // :linux_riscv32 / :linux_riscv64
                    lib.addCSourceFiles(.{
                        .files = &RISCV_SRCS,
                        .flags = cflags.items,
                    });
                    lib.addCSourceFiles(.{
                        .files = &LINUX_RISCV_SRCS,
                        .flags = cflags.items,
                    });
                },
                .mips64 => {
                    // :linux_mips64
                    // COMMON_SRCS + LINUX_SRCS already added
                },
                .s390x => {
                    // :linux_s390x
                    // COMMON_SRCS + LINUX_SRCS already added
                },
                else => {},
            }
        },

        .windows => {
            switch (cpu_arch) {
                .x86_64 => {
                    // :windows_x86_64
                    lib.addCSourceFiles(.{
                        .files = &X86_SRCS,
                        .flags = cflags.items,
                    });
                    lib.addCSourceFiles(.{
                        .files = &WINDOWS_X86_SRCS,
                        .flags = cflags.items,
                    });
                },
                .aarch64 => {
                    // :windows_arm64
                    lib.addCSourceFiles(.{
                        .files = &ARM_SRCS,
                        .flags = cflags.items,
                    });
                    lib.addCSourceFiles(.{
                        .files = &WINDOWS_ARM_SRCS,
                        .flags = cflags.items,
                    });
                },
                else => {},
            }
        },

        .macos => {
            lib.addCSourceFiles(.{
                .files = &MACH_SRCS,
                .flags = cflags.items,
            });

            switch (cpu_arch) {
                .x86_64 => {
                    // :macos_x86_64 / :macos_x86_64_legacy
                    lib.addCSourceFiles(.{
                        .files = &X86_SRCS,
                        .flags = cflags.items,
                    });
                    lib.addCSourceFiles(.{
                        .files = &MACH_X86_SRCS,
                        .flags = cflags.items,
                    });
                },
                .aarch64 => {
                    // :macos_arm64
                    lib.addCSourceFiles(.{
                        .files = &MACH_ARM_SRCS,
                        .flags = cflags.items,
                    });
                },
                else => {},
            }
        },

        .ios => {
            lib.addCSourceFiles(.{
                .files = &MACH_SRCS,
                .flags = cflags.items,
            });

            switch (cpu_arch) {
                .x86_64, .x86 => {
                    // :ios_x86, :ios_x86_64
                    lib.addCSourceFiles(.{
                        .files = &X86_SRCS,
                        .flags = cflags.items,
                    });
                    lib.addCSourceFiles(.{
                        .files = &MACH_X86_SRCS,
                        .flags = cflags.items,
                    });
                },
                .arm, .aarch64 => {
                    // :ios_armv7, :ios_arm64, :ios_arm64e, :ios_sim_arm64
                    lib.addCSourceFiles(.{
                        .files = &MACH_ARM_SRCS,
                        .flags = cflags.items,
                    });
                },
                else => {},
            }
        },

        .watchos => {
            lib.addCSourceFiles(.{
                .files = &MACH_SRCS,
                .flags = cflags.items,
            });

            switch (cpu_arch) {
                .x86_64, .x86 => {
                    // :watchos_x86, :watchos_x86_64
                    lib.addCSourceFiles(.{
                        .files = &X86_SRCS,
                        .flags = cflags.items,
                    });
                    lib.addCSourceFiles(.{
                        .files = &MACH_X86_SRCS,
                        .flags = cflags.items,
                    });
                },
                .arm, .aarch64 => {
                    // :watchos_armv7k, :watchos_arm64_32
                    lib.addCSourceFiles(.{
                        .files = &MACH_ARM_SRCS,
                        .flags = cflags.items,
                    });
                },
                else => {},
            }
        },

        .tvos => {
            lib.addCSourceFiles(.{
                .files = &MACH_SRCS,
                .flags = cflags.items,
            });

            switch (cpu_arch) {
                .x86_64 => {
                    // :tvos_x86_64
                    lib.addCSourceFiles(.{
                        .files = &X86_SRCS,
                        .flags = cflags.items,
                    });
                    lib.addCSourceFiles(.{
                        .files = &MACH_X86_SRCS,
                        .flags = cflags.items,
                    });
                },
                .aarch64 => {
                    // :tvos_arm64
                    lib.addCSourceFiles(.{
                        .files = &MACH_ARM_SRCS,
                        .flags = cflags.items,
                    });
                },
                else => {},
            }
        },

        .freebsd => {
            if (cpu_arch == .x86_64) {
                // :freebsd_x86_64
                lib.addCSourceFiles(.{
                    .files = &X86_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &FREEBSD_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &FREEBSD_X86_SRCS,
                    .flags = cflags.items,
                });
            }
        },

        .emscripten => {
            // :emscripten_wasm / :emscripten_wasmsimd / :emscripten_asmjs
            // We just key off OS here; cpuinfo upstream selects via cpu+features.
            lib.addCSourceFiles(.{
                .files = &EMSCRIPTEN_SRCS,
                .flags = cflags.items,
            });
        },

        else => {
            // Other OSes not covered in the Bazel file are left with just COMMON_SRCS.
        },
    }

    if (abi.isAndroid()) {
        // Map Bazel's android_* configs based on arch.
        switch (cpu_arch) {
            .arm, .thumb => {
                // :android_armv7
                lib.addCSourceFiles(.{
                    .files = &ARM_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &LINUX_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &LINUX_ARM32_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &ANDROID_ARM_SRCS,
                    .flags = cflags.items,
                });
            },
            .aarch64 => {
                // :android_arm64
                lib.addCSourceFiles(.{
                    .files = &ARM_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &LINUX_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &LINUX_ARM64_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &ANDROID_ARM_SRCS,
                    .flags = cflags.items,
                });
            },
            .x86, .x86_64 => {
                // :android_x86 / :android_x86_64
                lib.addCSourceFiles(.{
                    .files = &X86_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &LINUX_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &LINUX_X86_SRCS,
                    .flags = cflags.items,
                });
            },
            .riscv64 => {
                // :android_riscv64
                lib.addCSourceFiles(.{
                    .files = &RISCV_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &LINUX_SRCS,
                    .flags = cflags.items,
                });
                lib.addCSourceFiles(.{
                    .files = &LINUX_RISCV_SRCS,
                    .flags = cflags.items,
                });
            },
            else => {},
        }
    }

    b.installArtifact(lib);
    const install_headers = b.addInstallDirectory(.{
        .source_dir = b.path("include"),
        .install_dir = .header,
        .install_subdir = "include",
        .exclude_extensions = &.{},
    });
    b.getInstallStep().dependOn(&install_headers.step);
}
