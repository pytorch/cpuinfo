#!/usr/bin/env bash
# Copyright 2024 Google LLC

set -e

BAZEL_ARGS=()

# Bazel-level configuration
#
# If editing these flags, make sure `local-build.sh` flags are updated.
BAZEL_ARGS+=("-c=\"opt\"")
BAZEL_ARGS+=("-copt=\"fpic\"")

# User-specified Bazel arguments go last to allow overridding defaults
BAZEL_ARGS+=($@)

# Build all targets
bazel build :all
