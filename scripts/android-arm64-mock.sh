#!/usr/bin/env bash

set -e

adb push libs/arm64-v8a/galaxy-c9-pro-test /data/local/tmp/galaxy-c9-pro-test
adb push libs/arm64-v8a/galaxy-j7-tmobile-test /data/local/tmp/galaxy-j7-tmobile-test
adb push libs/arm64-v8a/galaxy-j7-uae-test /data/local/tmp/galaxy-j7-uae-test
adb push libs/arm64-v8a/galaxy-s6-test /data/local/tmp/galaxy-s6-test
adb push libs/arm64-v8a/galaxy-s7-global-test /data/local/tmp/galaxy-s7-global-test
adb push libs/arm64-v8a/galaxy-s7-us-test /data/local/tmp/galaxy-s7-us-test
adb push libs/arm64-v8a/galaxy-s8-global-test /data/local/tmp/galaxy-s8-global-test
adb push libs/arm64-v8a/galaxy-s8-us-test /data/local/tmp/galaxy-s8-us-test
adb push libs/arm64-v8a/huawei-mate-8-test /data/local/tmp/huawei-mate-8-test
adb push libs/arm64-v8a/huawei-mate-9-test /data/local/tmp/huawei-mate-9-test
adb push libs/arm64-v8a/huawei-mate-10-test /data/local/tmp/huawei-mate-10-test
adb push libs/arm64-v8a/huawei-p9-lite-test /data/local/tmp/huawei-p9-lite-test
adb push libs/arm64-v8a/meizu-pro-6-test /data/local/tmp/meizu-pro-6-test
adb push libs/arm64-v8a/meizu-pro-6s-test /data/local/tmp/meizu-pro-6s-test
adb push libs/arm64-v8a/meizu-pro-7-plus-test /data/local/tmp/meizu-pro-7-plus-test
adb push libs/arm64-v8a/nexus5x-test /data/local/tmp/nexus5x-test
adb push libs/arm64-v8a/nexus6p-test /data/local/tmp/nexus6p-test
adb push libs/arm64-v8a/nexus9-test /data/local/tmp/nexus9-test
adb push libs/arm64-v8a/oppo-r9-test /data/local/tmp/oppo-r9-test
adb push libs/arm64-v8a/pixel-c-test /data/local/tmp/pixel-c-test
adb push libs/arm64-v8a/pixel-xl-test /data/local/tmp/pixel-xl-test
adb push libs/arm64-v8a/pixel-test /data/local/tmp/pixel-test
adb push libs/arm64-v8a/xiaomi-mi-5c-test /data/local/tmp/xiaomi-mi-5c-test
adb push libs/arm64-v8a/xiaomi-redmi-note-3-test /data/local/tmp/xiaomi-redmi-note-3-test
adb push libs/arm64-v8a/xperia-c4-dual-test /data/local/tmp/xperia-c4-dual-test

adb shell "/data/local/tmp/galaxy-c9-pro-test --gtest_color=yes"
adb shell "/data/local/tmp/galaxy-j7-tmobile-test --gtest_color=yes"
adb shell "/data/local/tmp/galaxy-j7-uae-test --gtest_color=yes"
adb shell "/data/local/tmp/galaxy-s6-test --gtest_color=yes"
adb shell "/data/local/tmp/galaxy-s7-global-test --gtest_color=yes"
adb shell "/data/local/tmp/galaxy-s7-us-test --gtest_color=yes"
adb shell "/data/local/tmp/galaxy-s8-global-test --gtest_color=yes"
adb shell "/data/local/tmp/galaxy-s8-us-test --gtest_color=yes"
adb shell "/data/local/tmp/huawei-mate-8-test --gtest_color=yes"
adb shell "/data/local/tmp/huawei-mate-9-test --gtest_color=yes"
adb shell "/data/local/tmp/huawei-mate-10-test --gtest_color=yes"
adb shell "/data/local/tmp/huawei-p9-lite-test --gtest_color=yes"
adb shell "/data/local/tmp/meizu-pro-6-test --gtest_color=yes"
adb shell "/data/local/tmp/meizu-pro-6s-test --gtest_color=yes"
adb shell "/data/local/tmp/meizu-pro-7-plus-test --gtest_color=yes"
adb shell "/data/local/tmp/nexus5x-test --gtest_color=yes"
adb shell "/data/local/tmp/nexus6p-test --gtest_color=yes"
adb shell "/data/local/tmp/nexus9-test --gtest_color=yes"
adb shell "/data/local/tmp/oppo-r9-test --gtest_color=yes"
adb shell "/data/local/tmp/pixel-c-test --gtest_color=yes"
adb shell "/data/local/tmp/pixel-xl-test --gtest_color=yes"
adb shell "/data/local/tmp/pixel-test --gtest_color=yes"
adb shell "/data/local/tmp/xiaomi-mi-5c-test --gtest_color=yes"
adb shell "/data/local/tmp/xiaomi-redmi-note-3-test --gtest_color=yes"
adb shell "/data/local/tmp/xperia-c4-dual-test --gtest_color=yes"
