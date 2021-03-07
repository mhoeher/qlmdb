#!/bin/bash

set -e

SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"
cd "$SCRIPT_DIR"
cd ..
cd ..

if [ -n "$CI" ]; then
    dnf install -y gcc gcc-c++ make qt5-qtbase-devel cmake ninja-build
fi

mkdir -p build/fedora-cmake-builtin-lmdb
cd build/fedora-cmake-builtin-lmdb
cmake \
    -GNinja \
    -DQLMDB_USE_SYSTEM_LIBRARIES=OFF \
    ../..
cmake --build .
cmake --build . --target test

ls qlmdb | grep libqlmdb.so
! (ldd qlmdb/libqlmdb.so | grep liblmdb.so)

DESTDIR=$PWD/_ cmake --build . --target install

cd _
mkdir -p build
cd build
cmake \
    -DCMAKE_INSTALL_PREFIX=$PWD/../usr/local \
    -GNinja \
    ../../../../tests/cmake-install-test
cmake --build .
