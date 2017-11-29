#!/usr/bin/env bash

set -e

adb push libs/arm64-v8a/auxv-dump /data/local/tmp/auxv-dump
adb shell /data/local/tmp/auxv-dump
