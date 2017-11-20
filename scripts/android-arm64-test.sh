#!/usr/bin/env bash

set -e

adb push libs/arm64-v8a/init-test /data/local/tmp/init-test
adb push libs/arm64-v8a/cache-test /data/local/tmp/cache-test
adb shell /data/local/tmp/init-test --gtest_color=yes
adb shell /data/local/tmp/cache-test --gtest_color=yes
