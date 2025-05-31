/* Copyright (C) 2024 Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* 
 * Description: cpuinfo-service to initialize and dump cpu info collected
 *              through __cpuid() calls during device boot.
 *              it will delete cpuid.info file if it exits and create new.
 * 
 * Author: Unnithan, Balakrishnan <balakrishnan.unnithan@intel.com>
 */


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <cpuinfo.h>
#include <cpuinfo/common.h>
#include <cpuinfo/log.h>
#include <x86/linux/api.h>

#ifdef __ANDROID__
extern struct cpuinfo_x86_cpuid_info x86_cpuid_info;

CPUINFO_INTERNAL bool write_cpuid_info_file (void) {
    cpuinfo_log_debug("writing cpuid.info file");
    FILE* fd_info = fopen(CPUID_INFO_FILE, "wb");
    if (!fd_info) {
        cpuinfo_log_error("failed to open file %s: %s", CPUID_INFO_FILE, strerror(errno));
        return false;
    }

    int ret = fwrite(&x86_cpuid_info, sizeof(struct cpuinfo_x86_cpuid_info), 1, fd_info);
    if (!ret) {
        cpuinfo_log_error("failed to write cpuid info in %s: %s", CPUID_INFO_FILE, strerror(errno));
    }

    if (fclose(fd_info)) {
        cpuinfo_log_error("failed to close file %s: %s", CPUID_INFO_FILE, strerror(errno));
        return false;
    }

    return ret ? true : false;
}
#endif  // __ANDROID__

int main (void) {
    cpuinfo_log_info("start service");
#ifdef __ANDROID__
    if (remove(CPUID_INFO_FILE)) {
        cpuinfo_log_warning("failed to delete file %s: %s", CPUID_INFO_FILE, strerror(errno));
    }

    if (!cpuinfo_initialize()) {
        cpuinfo_log_error("failed to init cpuinfo lib. exit");
        return 1;
    }

    if (!write_cpuid_info_file()) {
        cpuinfo_log_error("failed to save cpuid info. exit");
        return 1;
    }
#endif  // __ANDROID__
    cpuinfo_log_info("exit service");
    return 0;
}