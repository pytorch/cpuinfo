#!/usr/bin/env bash

set -e

adb push libs/x86/cpu-info /data/local/tmp/cpu-info
adb shell /data/local/tmp/cpu-info
