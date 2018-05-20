#!/bin/bash

set -e

SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"
cd "$SCRIPT_DIR"
cd ..
cd ..

if [ -n "$CI" ]; then
    dnf install -y gcc gcc-c++ make qt5-qtbase-devel lmdb-devel
fi

mkdir -p build/fedora-builtin-lmdb
cd build/fedora-builtin-lmdb
qmake-qt5 CONFIG+=qlmdb_with_builtin_lmdb ../..
make -j4
make check
