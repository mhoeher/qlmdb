#!/bin/bash

set -e

SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"
cd "$SCRIPT_DIR"
cd ..
cd ..

if [ -n "$CI" ]; then
    dnf install -y cppcheck
fi

cppcheck  \
    --enable=warning,style,performance,portability,missingInclude \
    --error-exitcode=1 \
    --inline-suppr \
    qlmdb/ tests/
