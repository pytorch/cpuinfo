#!/usr/bin/env bash

set -e

adb push libs/armeabi-v7a/auxv-dump /data/local/tmp/auxv-dump
adb shell /data/local/tmp/auxv-dump
