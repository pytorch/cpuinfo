#!/usr/bin/env bash

set -e

SYSTEMNAME=`uname`
if [[ "$SYSTEMNAME" == 'Darwin' ]]
then
   PROCESSORS=`sysctl -n hw.ncpu`
else
   PROCESSORS=`nproc`
fi

ndk-build APP_ABI=arm64-v8a -j$PROCESSORS
