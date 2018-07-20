#!/bin/bash

set -e

SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"
cd "$SCRIPT_DIR"
cd ..
cd ..

if [ -n "$CI" ]; then
    dnf install -y gcc gcc-c++ make qt5-qtbase-devel cmake
fi

mkdir -p build/fedora-cmake-builtin-lmdb
cd build/fedora-cmake-builtin-lmdb
cmake -DQLMDB_WITH_BUILTIN_LMDB=ON ../..
make -j4
make test

ls qlmdb | grep libqlmdb.so
! (ldd qlmdb/libqlmdb.so | grep liblmdb.so)
