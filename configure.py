#!/usr/bin/env python


import confu
parser = confu.standard_parser("cpuinfo configuration script")
parser.add_argument("--log", dest="log_level",
    choices=("none", "error", "warning", "info", "debug"), default="error")
parser.add_argument("--mock", dest="mock", action="store_true")


def main(args):
    options = parser.parse_args(args)
    build = confu.Build.from_options(options)

    macros = {
        "CPUINFO_LOG_LEVEL": {"none": 0, "error": 1, "warning": 2, "info": 3, "debug": 4}[options.log_level],
        "CPUINFO_MOCK": int(options.mock)
    }

    build.export_cpath("include", ["cpuinfo.h"])

    with build.options(source_dir="src", macros=macros, extra_include_dirs="src"):
        sources = ["init.c", "api.c", "log.c"]
        if build.target.is_x86_64:
            sources += [
                "x86/init.c", "x86/info.c", "x86/vendor.c", "x86/uarch.c", "x86/name.c",
                "x86/topology.c",
                "x86/cache/init.c", "x86/cache/descriptor.c", "x86/cache/deterministic.c",
            ]
            if build.target.is_macos:
                sources += ["x86/mach/init.c"]
            elif build.target.is_linux or build.target.is_android:
                sources += [
                    "x86/linux/init.c",
                    "x86/linux/cpuinfo.c",
                ]
            sources.append("x86/isa.c" if not build.target.is_nacl else "x86/nacl/isa.c")
        if build.target.is_arm or build.target.is_arm64:
            sources += ["arm/uarch.c", "arm/cache.c"]
            if build.target.is_linux or build.target.is_android:
                sources += [
                    "arm/linux/init.c",
                    "arm/linux/cpuinfo.c",
                    "arm/linux/clusters.c",
                    "arm/linux/midr.c",
                    "arm/linux/chipset.c",
                    "arm/linux/hwcap.c",
                ]
                if build.target.is_arm:
                    sources.append("arm/linux/aarch32-isa.c")
                elif build.target.is_arm64:
                    sources.append("arm/linux/aarch64-isa.c")
                if build.target.is_android:
                    sources += [
                        "arm/android/gpu.c",
                        "arm/android/properties.c",
                    ]

        if build.target.is_macos:
            sources += ["mach/topology.c"]
        if build.target.is_linux or build.target.is_android:
            sources += [
                "linux/current.c",
                "linux/cpulist.c",
                "linux/smallfile.c",
                "linux/multiline.c",
                "linux/processors.c",
            ]
            if options.mock:
                sources += ["linux/mockfile.c"]
        if build.target.is_android:
            sources.append("linux/gpu.c")
            if options.mock:
                sources.append("gpu/gles2-mock.c")
            else:
                sources.append("gpu/gles2.c")
        build.static_library("cpuinfo", map(build.cc, sources))

    with build.options(source_dir="tools", deps=build):
        build.executable("cpu-info", build.cc("cpu-info.c"))
        build.executable("isa-info", build.cc("isa-info.c"))
        build.executable("cache-info", build.cc("cache-info.c"))

    if build.target.is_x86_64:
        with build.options(source_dir="tools", include_dirs=["src", "include"]):
            build.executable("cpuid-dump", build.cc("cpuid-dump.c"))

    with build.options(source_dir="test", deps=[build, build.deps.googletest]):
        build.smoketest("init-test", build.cxx("init.cc"))
        if build.target.is_linux:
            build.smoketest("get-current-test", build.cxx("get-current.cc"))
        if build.target.is_x86_64:
            build.smoketest("brand-string-test", build.cxx("name/brand-string.cc"))
    if options.mock:
        with build.options(source_dir="test", include_dirs="test", macros="CPUINFO_MOCK", deps=[build, build.deps.googletest]):
            if build.target.is_arm64 and build.target.is_linux:
                build.unittest("scaleway-test", build.cxx("scaleway.cc"))

    if not options.mock:
        with build.options(source_dir="bench", deps=[build, build.deps.googlebenchmark]):
            build.benchmark("init-bench", build.cxx("init.cc"))
            if not build.target.is_macos:
                build.benchmark("get-current-bench", build.cxx("get-current.cc"))

    return build


if __name__ == "__main__":
    import sys
    main(sys.argv[1:]).generate()
