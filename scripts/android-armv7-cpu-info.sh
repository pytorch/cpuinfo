#!/usr/bin/env bash

set -e

adb push libs/armeabi-v7a/cpu-info /data/local/tmp/cpu-info
adb shell /data/local/tmp/cpu-info
