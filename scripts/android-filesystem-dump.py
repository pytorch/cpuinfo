#!/usr/bin/env python

import os
import sys
import argparse
import subprocess
import tempfile


parser = argparse.ArgumentParser(description='Android system files extractor')
parser.add_argument("-p", "--prefix", metavar="NAME", required=True,
                    help="Prefix for stored files, e.g. galaxy-s7-us")


SYSTEM_FILES = [
    "/proc/cpuinfo",
    "/system/build.prop",
    "/sys/devices/system/cpu/kernel_max",
    "/sys/devices/system/cpu/possible",
    "/sys/devices/system/cpu/present",
]

CPU_FILES = [
    "cpufreq/cpuinfo_max_freq",
    "cpufreq/cpuinfo_min_freq",
    "topology/physical_package_id",
    "topology/core_siblings_list",
    "topology/core_id",
    "topology/thread_siblings_list",
]

CACHE_FILES = [
    "allocation_policy",
    "coherency_line_size",
    "level",
    "number_of_sets",
    "shared_cpu_list",
    "size",
    "type",
    "ways_of_associativity",
    "write_policy",
]

def c_escape(string):
    c_string = ""
    for c in string:
        if c == "\\":
            c_string += "\\\\"
        elif c == "\"":
            c_string += "\\\""
        elif c == "\t":
            c_string += "\\t"
        elif c == "\n":
            c_string += "\\n"
        elif c == "\r":
            c_string += "\\r"
        elif ord(c) == 0:
            c_string += "\\0"
        elif 32 <= ord(c) < 127:
            c_string += c
        else:
            c_string += "x%02X" % ord(c)
    return c_string

def adb_pull(device_path, local_path):
    env = os.environ.copy()
    env["LC_ALL"] = "C"

    adb = subprocess.Popen(["adb", "pull", device_path, local_path], env=env)
    adb.communicate()
    return adb.returncode == 0


def dump_device_file(stream, path):
    temp_fd, temp_path = tempfile.mkstemp()
    os.close(temp_fd)
    try:
        if adb_pull(path, temp_path):
            with open(temp_path, "rb") as temp_file:
                content = temp_file.read()
                stream.write("\t{\n")
                stream.write("\t\t.path = \"%s\",\n" % path)
                stream.write("\t\t.size = %d,\n" % len(content))
                if len(content.splitlines()) > 1:
                    stream.write("\t\t.content =")
                    for line in content.splitlines(True):
                        stream.write("\n\t\t\t\"%s\"" % c_escape(line))
                    stream.write(",\n")
                else:
                    stream.write("\t\t.content = \"%s\",\n" % c_escape(content))
                stream.write("\t},\n")
                return True
    finally:
        os.remove(temp_path)


def main(args):
    options = parser.parse_args(args)

    # with open(os.path.join("test", "dmesg", options.prefix + ".log"), "w") as dmesg_log:
    #     dmesg_log.write(device.Shell("dmesg"))
    with open(os.path.join("test", options.prefix + ".h"), "w") as file_header:
        file_header.write("struct cpuinfo_mock_file filesystem[] = {\n")
        for path in SYSTEM_FILES:
            dump_device_file(file_header, path)
        for cpu in range(16):
            for filename in CPU_FILES:
                path = "/sys/devices/system/cpu/cpu%d/%s" % (cpu, filename)
                dump_device_file(file_header, path)
            for index in range(10):
                for filename in CACHE_FILES:
                    path = "/sys/devices/system/cpu/cpu%d/cache/index%d/%s" % (cpu, index, filename)
                    dump_device_file(file_header, path)
        file_header.write("\t{ NULL },\n")
        file_header.write("};\n")
    adb_pull("/proc/cpuinfo",
        os.path.join("test", "cpuinfo", options.prefix + ".log"))
    adb_pull("/system/build.prop",
        os.path.join("test", "build.prop", options.prefix + ".log"))

if __name__ == "__main__":
    main(sys.argv[1:])
