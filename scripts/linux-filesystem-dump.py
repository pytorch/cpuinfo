#!/usr/bin/env python3

"""Dumps the Linux files that cpuinfo reads into a mock filesystem header.

The header defines `filesystem` for cpuinfo_mock_filesystem(), for a mock test
in test/mock. The values of AT_HWCAP and AT_HWCAP2 to pass to
cpuinfo_set_hwcap() and cpuinfo_set_hwcap2() are printed to stderr.
"""

import argparse
import os
import struct
import sys

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("-p", "--prefix", metavar="NAME", required=True,
                    help="Name of the mock test, e.g. power9-powernv")

SYSTEM_FILES = [
    "/proc/cpuinfo",
    "/sys/devices/system/cpu/kernel_max",
    "/sys/devices/system/cpu/possible",
    "/sys/devices/system/cpu/present",
    "/sys/devices/system/cpu/online",
]

CPU_FILES = [
    "cpufreq/cpuinfo_max_freq",
    "cpufreq/cpuinfo_min_freq",
    "topology/cluster_cpus_list",
    "topology/core_cpus_list",
    "topology/core_id",
    "topology/core_siblings_list",
    "topology/package_cpus_list",
    "topology/physical_package_id",
    "topology/thread_siblings_list",
]

CACHE_FILES = [
    "coherency_line_size",
    "level",
    "number_of_sets",
    "shared_cpu_list",
    "size",
    "type",
    "ways_of_associativity",
]

AT_HWCAP = 16
AT_HWCAP2 = 26


def c_escape(data):
    escaped = ""
    for byte in data:
        c = chr(byte)
        if c == "\\":
            escaped += "\\\\"
        elif c == "\"":
            escaped += "\\\""
        elif c == "\t":
            escaped += "\\t"
        elif c == "\n":
            escaped += "\\n"
        elif c == "\r":
            escaped += "\\r"
        elif byte == 0:
            escaped += "\\0"
        elif 32 <= byte < 127:
            escaped += c
        else:
            escaped += "\\x%02X" % byte
    return escaped


def dump_file(stream, path):
    try:
        with open(path, "rb") as f:
            content = f.read()
    except OSError:
        return
    stream.write("\t{\n")
    stream.write("\t\t.path = \"%s\",\n" % path)
    stream.write("\t\t.size = %d,\n" % len(content))
    lines = content.splitlines(True)
    if len(lines) > 1:
        stream.write("\t\t.content =")
        for line in lines:
            stream.write("\n\t\t\t\"%s\"" % c_escape(line))
        stream.write(",\n")
    else:
        stream.write("\t\t.content = \"%s\",\n" % c_escape(content))
    stream.write("\t},\n")


def parse_cpulist(text):
    cpus = []
    for item in text.strip().split(","):
        if not item:
            continue
        if "-" in item:
            start, end = item.split("-")
            cpus.extend(range(int(start), int(end) + 1))
        else:
            cpus.append(int(item))
    return cpus


def read_hwcaps():
    hwcaps = {}
    with open("/proc/self/auxv", "rb") as f:
        auxv = f.read()
    entry = struct.Struct("@LL")
    for offset in range(0, len(auxv), entry.size):
        key, value = entry.unpack_from(auxv, offset)
        hwcaps[key] = value
    return hwcaps.get(AT_HWCAP, 0), hwcaps.get(AT_HWCAP2, 0)


def main(args):
    options = parser.parse_args(args)

    with open("/sys/devices/system/cpu/possible") as f:
        cpus = parse_cpulist(f.read())

    with open(os.path.join("test", "mock", options.prefix + ".h"), "w") as header:
        header.write("struct cpuinfo_mock_file filesystem[] = {\n")
        for path in SYSTEM_FILES:
            dump_file(header, path)
        for cpu in cpus:
            for name in CPU_FILES:
                dump_file(header, "/sys/devices/system/cpu/cpu%d/%s" % (cpu, name))
            index = 0
            while os.path.exists("/sys/devices/system/cpu/cpu%d/cache/index%d" % (cpu, index)):
                for name in CACHE_FILES:
                    dump_file(header, "/sys/devices/system/cpu/cpu%d/cache/index%d/%s" % (cpu, index, name))
                index += 1
        header.write("\t{NULL},\n")
        header.write("};\n")

    hwcap, hwcap2 = read_hwcaps()
    sys.stderr.write("AT_HWCAP: 0x%08X\nAT_HWCAP2: 0x%08X\n" % (hwcap, hwcap2))


if __name__ == "__main__":
    main(sys.argv[1:])
