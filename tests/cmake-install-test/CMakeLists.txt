cmake_minimum_required(VERSION 3.0)
project(qlmdb-cmake-install-test)

find_package(QLMDB-qt5 REQUIRED)

add_executable(qlmdb-cmake-install-test)
target_sources(qlmdb-cmake-install-test PRIVATE main.cpp)
target_link_libraries(qlmdb-cmake-install-test PRIVATE QLMDB::qlmdb-qt5)
