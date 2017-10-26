#!/usr/bin/env bash

set -e

adb push libs/armeabi-v7a/cache-info /data/local/tmp/cache-info
adb shell /data/local/tmp/cache-info
