#!/bin/bash

set -e

SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"
cd "$SCRIPT_DIR"
cd ..
cd ..

if [ -n "$CI" ]; then
    dnf install -y gcc gcc-c++ make qt5-qtbase-devel lmdb-devel cmake ninja-build
fi

mkdir -p build/fedora-cmake-static
cd build/fedora-cmake-static
cmake \
    -GNinja \
    -DQLMDB_WITH_STATIC_LIBS=ON \
    -DQLMDB_USE_SYSTEM_LIBRARIES=ON \
    ../..
cmake --build .
cmake --build . --target test

ls qlmdb | grep libqlmdb.a

DESTDIR=$PWD/_ cmake --build . --target install

cd _
mkdir -p build
cd build
cmake \
    -DCMAKE_INSTALL_PREFIX=$PWD/../usr/local \
    -GNinja \
    ../../../../tests/cmake-install-test
cmake --build .
