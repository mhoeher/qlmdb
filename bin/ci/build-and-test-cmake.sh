#!/bin/bash

set -e

SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"
cd "$SCRIPT_DIR"
cd ..
cd ..

if [ -n "$CI" ]; then
    dnf install -y gcc gcc-c++ make qt5-qtbase-devel lmdb-devel cmake
fi

mkdir -p build/fedora-cmake
cd build/fedora-cmake
cmake ../..
make -j4
make test

ls qlmdb | grep libqlmdb.so
ldd qlmdb/libqlmdb.so | grep liblmdb.so
