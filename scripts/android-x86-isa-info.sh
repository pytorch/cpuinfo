#!/usr/bin/env bash

set -e

adb push libs/x86/isa-info /data/local/tmp/isa-info
adb shell /data/local/tmp/isa-info
