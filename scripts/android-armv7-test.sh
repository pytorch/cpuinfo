#!/usr/bin/env bash

set -e

adb push libs/armeabi-v7a/init-test /data/local/tmp/init-test
adb shell /data/local/tmp/init-test --gtest_color=yes
