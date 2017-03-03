#!/usr/bin/env python


import confu
parser = confu.standard_parser("cpuinfo configuration script")
parser.add_argument("--log", dest="log_level",
    choices=("none", "error", "warning", "info", "debug"), default="error")


def main(args):
    options = parser.parse_args(args)
    build = confu.Build.from_options(options)
    build.add_macro("CPUINFO_LOG_LEVEL",
        value={"none": 0, "error": 1, "warning": 2, "info": 3, "debug": 4}[options.log_level])

    build.export_cpath("include", ["cpuinfo.h"])

    with build.options(source_dir="src", extra_include_dirs="src"):
        sources = ["init.c", "log.c"]
        if build.target.is_x86_64:
            sources += [
                "x86/init.c", "x86/info.c", "x86/vendor.c", "x86/uarch.c", "x86/topology.c",
                "x86/cache/init.c", "x86/cache/descriptor.c", "x86/cache/deterministic.c",
            ]
            if build.target.is_macos:
                sources += ["x86/mach/init.c"]
            sources.append("x86/isa.c" if not build.target.is_nacl else "x86/nacl/isa.c")
        if build.target.is_macos:
            sources += ["mach/topology.c"]
        build.static_library("cpuinfo", map(build.cc, sources))

    with build.options(source_dir="tools", deps=build):
        build.executable("cpu-info", build.cc("cpu-info.c"))
        build.executable("isa-info", build.cc("isa-info.c"))
        build.executable("cache-info", build.cc("cache-info.c"))

    return build


if __name__ == "__main__":
    import sys
    main(sys.argv[1:]).generate()
