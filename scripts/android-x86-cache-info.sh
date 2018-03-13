#!/usr/bin/env bash

set -e

adb push libs/x86/cache-info /data/local/tmp/cache-info
adb shell /data/local/tmp/cache-info
