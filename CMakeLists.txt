cmake_minimum_required(VERSION 3.1.0)
file(READ version.txt QLMDB_VERSION)
project(qlmdb VERSION ${QLMDB_VERSION})

# Generate build commands JSON file:
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include required CMake libraries:
include(GNUInstallDirs)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Search for find files also in local folder:
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Define options users can set to change details of the build:
## Linking against required modules:
option(QLMDB_USE_SYSTEM_LIBRARIES "Use system libraries by default." OFF)
option(QLMDB_WITH_SYSTEM_LMDB "Use the system version of LMDB." ${QLMDB_USE_SYSTEM_LIBRARIES})
## Build fine tuning:
option(QLMDB_WITH_STATIC_LIBS "Build QLMDB as static library." OFF)
option(QLMDB_WITHOUT_TESTS "Do not build unit tests." OFF)

# Find the QtCore library
set(QLMDB_REQUIRED_QT_DEPENDENCIES Core)
set(QLMDB_REQUIRED_BUILD_QT_DEPENDENCIES ${QLMDB_REQUIRED_QT_DEPENDENCIES} Test)
find_package(QT NAMES Qt6 Qt5 COMPONENTS ${QLMDB_REQUIRED_QT_QLMDB_REQUIRED_BUILD_QT_DEPENDENCIESDEPENDENCIES} REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QLMDB_REQUIRED_BUILD_QT_DEPENDENCIES} REQUIRED)
find_package (Threads)


if(QLMDB_WITH_SYSTEM_LMDB)
    find_package(LMDB REQUIRED)
endif()


# Enable testing
enable_testing()

add_subdirectory(qlmdb)

if(NOT QLMDB_WITHOUT_TESTS)
    add_subdirectory(tests)
endif()


# Export targets:
install(
    EXPORT
        QLMDBTargets
    NAMESPACE
        QLMDB::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/QLMDB-qt${QT_VERSION_MAJOR}
)


# Export library interface:
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    QLMDB-qt${QT_VERSION_MAJOR}ConfigVersion.cmake
    VERSION
        ${QLMDB_VERSION}
    COMPATIBILITY
        SameMajorVersion
)
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/QLMDB-qt${QT_VERSION_MAJOR}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/QLMDB-qt${QT_VERSION_MAJOR}ConfigVersion.cmake
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/QLMDB-qt${QT_VERSION_MAJOR}
)
configure_package_config_file(
    cmake/QLMDBConfig.cmake QLMDB-qt${QT_VERSION_MAJOR}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QLMDB-qt${QT_VERSION_MAJOR}
)


# Also create a *.pri file and install it, so qmake based projects
# can more easily build against the library:
find_package(ECM)
if(ECM_FOUND)
    list(APPEND CMAKE_MODULE_PATH "${ECM_MODULE_DIR}")
    include(ECMGeneratePriFile)
    set(PROJECT_VERSION_STRING ${QLMDB_VERSION})
    unset(PRI_DEPS)
    foreach(dep ${QLMDB_REQUIRED_QT_DEPENDENCIES})
        string(TOLOWER ${dep} dep)
        if(PRI_DEPS)
            set(PRI_DEPS "${PRI_DEPS} ${dep}")
        else()
            set(PRI_DEPS ${dep})
        endif()
    endforeach()
    
    ecm_generate_pri_file(
        BASE_NAME
            QLMDB-qt${QT_VERSION_MAJOR}
        LIB_NAME
            qlmdb-qt${QT_VERSION_MAJOR}
        DEPS
            ${PRI_DEPS}
        FILENAME_VAR
            QLMDB_PRI_FILE
    )
    install(
        FILES 
            ${QLMDB_PRI_FILE} 
        DESTINATION 
            ${ECM_MKSPECS_INSTALL_DIR}
    )
endif()
