#!/usr/bin/env bash

set -e

adb push libs/armeabi-v7a/blu-r1-hd-test /data/local/tmp/blu-r1-hd-test
adb push libs/armeabi-v7a/moto-e-gen1-test /data/local/tmp/moto-e-gen1-test
adb push libs/armeabi-v7a/moto-g-gen2-test /data/local/tmp/moto-g-gen2-test
adb push libs/armeabi-v7a/moto-g-gen3-test /data/local/tmp/moto-g-gen3-test
adb push libs/armeabi-v7a/nexus4-test /data/local/tmp/nexus4-test
adb push libs/armeabi-v7a/nexus5x-test /data/local/tmp/nexus5x-test
adb push libs/armeabi-v7a/nexus6-test /data/local/tmp/nexus6-test
adb push libs/armeabi-v7a/nexus6p-test /data/local/tmp/nexus6p-test
adb push libs/armeabi-v7a/nexus9-test /data/local/tmp/nexus9-test
adb push libs/armeabi-v7a/meizu-pro-6s-test /data/local/tmp/meizu-pro-6s-test
adb push libs/armeabi-v7a/galaxy-grand-prime-value-edition-test /data/local/tmp/galaxy-grand-prime-value-edition-test
adb push libs/armeabi-v7a/galaxy-s3-us-test /data/local/tmp/galaxy-s3-us-test
adb push libs/armeabi-v7a/galaxy-s4-us-test /data/local/tmp/galaxy-s4-us-test
adb push libs/armeabi-v7a/galaxy-s6-test /data/local/tmp/galaxy-s6-test
adb push libs/armeabi-v7a/galaxy-s7-us-test /data/local/tmp/galaxy-s7-us-test
adb push libs/armeabi-v7a/galaxy-s7-global-test /data/local/tmp/galaxy-s7-global-test
adb push libs/armeabi-v7a/galaxy-s8-us-test /data/local/tmp/galaxy-s8-us-test
adb push libs/armeabi-v7a/galaxy-s8-global-test /data/local/tmp/galaxy-s8-global-test

# adb shell "/data/local/tmp/blu-r1-hd-test --gtest_color=yes"
# adb shell "/data/local/tmp/moto-e-gen1-test --gtest_color=yes"
# adb shell "/data/local/tmp/moto-g-gen2-test --gtest_color=yes"
# adb shell "/data/local/tmp/moto-g-gen3-test --gtest_color=yes"
# adb shell "/data/local/tmp/nexus4-test --gtest_color=yes"
# adb shell "/data/local/tmp/nexus5x-test --gtest_color=yes"
# adb shell "/data/local/tmp/nexus6-test --gtest_color=yes"
# adb shell "/data/local/tmp/nexus6p-test --gtest_color=yes"
# adb shell "/data/local/tmp/nexus9-test --gtest_color=yes"
# adb shell "/data/local/tmp/meizu-pro-6s-test --gtest_color=yes"
# adb shell "/data/local/tmp/galaxy-grand-prime-test --gtest_color=yes"
# adb shell "/data/local/tmp/galaxy-s3-us-test --gtest_color=yes"
# adb shell "/data/local/tmp/galaxy-s4-us-test --gtest_color=yes"
# adb shell "/data/local/tmp/galaxy-s6-test --gtest_color=yes"
adb shell "/data/local/tmp/galaxy-s7-us-test --gtest_color=yes"
# adb shell "/data/local/tmp/galaxy-s7-global-test --gtest_color=yes"
# adb shell "/data/local/tmp/galaxy-s8-us-test --gtest_color=yes"
# adb shell "/data/local/tmp/galaxy-s8-global-test --gtest_color=yes"
