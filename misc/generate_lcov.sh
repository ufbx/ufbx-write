#!/usr/bin/env bash
set -x
set -e

mkdir -p build

LLVM_COV="${LLVM_COV:-llvm-cov}"
LLVM_GCOV=$(realpath misc/llvm_gcov.sh)
chmod +x misc/llvm_gcov.sh
UFBX_WRITE_GCDA=$(find build-test/CMakeFiles/ufbx_write.dir -name 'ufbx_write.c.gcda' -print -quit)

$LLVM_COV gcov "$UFBX_WRITE_GCDA" -b
lcov --directory build-test --base-directory . --gcov-tool $LLVM_GCOV --config-file misc/lcovrc --capture -o coverage.lcov
