#!/bin/bash

set -e

SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"
cd "$SCRIPT_DIR"
cd ..
cd ..

if [ -n "$CI" ]; then
    dnf install -y gcc gcc-c++ make qt5-qtbase-devel lmdb-devel cmake
fi

mkdir -p build/fedora-cmake-static
cd build/fedora-cmake-static
cmake -DQLMDB_WITH_STATIC_LIBS=ON ../..
make -j4
make test

ls qlmdb | grep libqlmdb.a
