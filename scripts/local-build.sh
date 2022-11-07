#!/usr/bin/env bash
#
# Copyright (c) Facebook, Inc. and its affiliates.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.

set -e

CMAKE_ARGS=()

# CMake-level configuration
CMAKE_ARGS+=("-DCMAKE_BUILD_TYPE=Release")
CMAKE_ARGS+=("-DCMAKE_POSITION_INDEPENDENT_CODE=ON")

# If Ninja is installed, prefer it to Make
if [ -x "$(command -v ninja)" ]
then
  CMAKE_ARGS+=("-GNinja")
fi

# Use-specified CMake arguments go last to allow overridding defaults
CMAKE_ARGS+=($@)

# Cross-platform parallel build
if [ "$(uname)" == "Darwin" ]; then
  # Build for arm64
  mkdir -p build/arm64
  CMAKE_ARGS+=("-DCMAKE_OSX_ARCHITECTURES=arm64")
  cd build/arm64 && cmake ../.. \
    "${CMAKE_ARGS[@]}"
  cmake --build . -- "-j$(sysctl -n hw.ncpu)"
  cd ../..

   # Build for x86_64
  mkdir -p build/x86_64
  CMAKE_ARGS+=("-DCMAKE_OSX_ARCHITECTURES=x86_64")
  cd build/x86_64 && cmake ../.. \
    "${CMAKE_ARGS[@]}"
  cmake --build . -- "-j$(sysctl -n hw.ncpu)"
  cd ../..

  # Merge builds into macos_universal
  mkdir -p build/macos_universal
  lipo -create build/arm64/libcpuinfo.dylib build/x86_64/libcpuinfo.dylib -output build/macos_universal/libcpuinfo.dylib

elif [ "$(uname)" == "Linux" ]; then
  cmake --build . -- "-j$(nproc)"
else
  cmake --build .
fi
