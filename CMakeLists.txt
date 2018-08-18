cmake_minimum_required(VERSION 3.1.0)
project(qlmdb)

# Include required CMake libraries:
include(GNUInstallDirs)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Define options users can set to change details of the build:
option(QLMDB_WITH_STATIC_LIBS "Build QLMDB as static library." OFF)
option(QLMDB_WITH_BUILTIN_LMDB "Build QLMDB with a built-in version of LMDB." OFF)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# Find the QtCore library
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Test CONFIG REQUIRED)
find_package (Threads)


# Enable testing
enable_testing()

add_subdirectory(qlmdb)
add_subdirectory(tests)
