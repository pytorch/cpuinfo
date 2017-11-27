#!/usr/bin/env python

import os
import sys
import string
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
    "/sys/devices/system/cpu/online",
    "/sys/devices/system/cpu/offline",
    "/sys/devices/system/cpu/modalias",
    "/sys/devices/system/cpu/cpufreq/all_time_in_state",
    "/sys/devices/system/cpu/cpufreq/current_in_state",
    "/sys/devices/system/cpu/cpuidle/current_driver",
    "/sys/devices/system/cpu/cpuidle/current_governor_ro",
    "/sys/devices/system/cpu/cputopo/cpus_per_cluster",
    "/sys/devices/system/cpu/cputopo/big_cpumask",
    "/sys/devices/system/cpu/cputopo/glbinfo",
    "/sys/devices/system/cpu/cputopo/is_big_little",
    "/sys/devices/system/cpu/cputopo/is_multi_cluster",
    "/sys/devices/system/cpu/cputopo/little_cpumask",
    "/sys/devices/system/cpu/cputopo/nr_clusters",
    "/sys/devices/system/b.L/big_threads",
    "/sys/devices/system/b.L/boot_cluster",
    "/sys/devices/system/b.L/core_status",
    "/sys/devices/system/b.L/little_threads",
    "/sys/devices/system/b.L/down_migrations",
    "/sys/devices/system/b.L/up_migrations",
    "/sys/devices/system/cpu/clusterhotplug/cur_hstate",
    "/sys/devices/system/cpu/clusterhotplug/down_freq",
    "/sys/devices/system/cpu/clusterhotplug/down_tasks",
    "/sys/devices/system/cpu/clusterhotplug/down_threshold",
    "/sys/devices/system/cpu/clusterhotplug/sampling_rate",
    "/sys/devices/system/cpu/clusterhotplug/time_in_state",
    "/sys/devices/system/cpu/clusterhotplug/up_freq",
    "/sys/devices/system/cpu/clusterhotplug/up_tasks",
    "/sys/devices/system/cpu/clusterhotplug/up_threshold",
]

CPU_FILES = [
    "current_driver",
    "current_governor_ro",
    "cpuidle/driver/name",
    "cpufreq/affected_cpus",
    "cpufreq/cpuinfo_max_freq",
    "cpufreq/cpuinfo_min_freq",
    "cpufreq/cpuinfo_transition_latency",
    "cpufreq/related_cpus",
    "cpufreq/scaling_available_frequencies",
    "cpufreq/scaling_available_governors",
    "cpufreq/scaling_cur_freq",
    "cpufreq/scaling_driver",
    "cpufreq/scaling_governor",
    "cpufreq/scaling_max_freq",
    "cpufreq/scaling_min_freq",
    "cpufreq/sched/down_throttle_nsec",
    "cpufreq/sched/up_throttle_nsec",
    "cpufreq/stats/time_in_state",
    "cpufreq/stats/total_trans",
    "cpufreq/stats/trans_table",
    "topology/core_id",
    "topology/core_siblings",
    "topology/core_siblings_list",
    "topology/cpu_capacity",
    "topology/max_cpu_capacity",
    "topology/physical_package_id",
    "topology/thread_siblings",
    "topology/thread_siblings_list",
]

CACHE_FILES = [
    "allocation_policy",
    "coherency_line_size",
    "level",
    "number_of_sets",
    "shared_cpu_list",
    "shared_cpu_map",
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

def adb_shell(commands):
    env = os.environ.copy()
    env["LC_ALL"] = "C"

    adb = subprocess.Popen(["adb", "shell"] + commands, env=env, stdout=subprocess.PIPE)
    stdout, _ = adb.communicate()
    if adb.returncode == 0:
        return stdout

def adb_getprop():
    properties = adb_shell(["getprop"])
    properties_list = list()
    for prop in properties.splitlines():
        prop = prop.strip()
        if prop:
            key, value = tuple(map(string.strip, prop.split(":", 1)))
            assert key.startswith("[") and key.endswith("]")
            key = key[1:-1]
            assert value.startswith("[") and value.endswith("]")
            value = value[1:-1]
            properties_list.append((key, value))
    return properties_list

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
                return content
    finally:
        if os.path.exists(temp_path):
            os.remove(temp_path)


def main(args):
    options = parser.parse_args(args)

    dmesg_content = adb_shell(["dmesg"])
    if dmesg_content is not None and dmesg_content.strip() == "klogctl: Operation not permitted":
        dmesg_content = None
    if dmesg_content is not None:
        with open(os.path.join("test", "dmesg", options.prefix + ".log"), "w") as dmesg_dump:
            dmesg_dump.write(dmesg_content)

    build_prop_content = None
    proc_cpuinfo_content = None
    kernel_max = 0
    with open(os.path.join("test", "mock", options.prefix + ".h"), "w") as file_header:
        file_header.write("struct cpuinfo_mock_file filesystem[] = {\n")
        for path in SYSTEM_FILES:
            content = dump_device_file(file_header, path)
            if content is not None:
                if path == "/proc/cpuinfo":
                    proc_cpuinfo_content = content
                elif path == "/system/build.prop":
                    build_prop_content = content
                elif path == "/sys/devices/system/cpu/kernel_max":
                    kernel_max = int(content.strip())
        for cpu in range(kernel_max + 1):
            for filename in CPU_FILES:
                path = "/sys/devices/system/cpu/cpu%d/%s" % (cpu, filename)
                dump_device_file(file_header, path)
            for index in range(5):
                for filename in CACHE_FILES:
                    path = "/sys/devices/system/cpu/cpu%d/cache/index%d/%s" % (cpu, index, filename)
                    dump_device_file(file_header, path)
        file_header.write("\t{ NULL },\n")
        file_header.write("};\n")
        file_header.write("#ifdef __ANDROID__\n")
        file_header.write("struct cpuinfo_mock_property properties[] = {\n")
        for key, value in adb_getprop():
            file_header.write("\t{\n")
            file_header.write("\t\t.key = \"%s\",\n" % c_escape(key))
            file_header.write("\t\t.value = \"%s\",\n" % c_escape(value))
            file_header.write("\t},\n")
        file_header.write("\t{ NULL },\n")
        file_header.write("};\n")
        file_header.write("#endif /* __ANDROID__ */\n")

    if proc_cpuinfo_content is not None:
        with open(os.path.join("test", "cpuinfo", options.prefix + ".log"), "w") as proc_cpuinfo_dump:
            proc_cpuinfo_dump.write(proc_cpuinfo_content)
    if build_prop_content is not None:
        with open(os.path.join("test", "build.prop", options.prefix + ".log"), "w") as build_prop_dump:
            build_prop_dump.write(build_prop_content)

if __name__ == "__main__":
    main(sys.argv[1:])
