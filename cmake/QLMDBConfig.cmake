@PACKAGE_INIT@

include(CMakeFindDependencyMacro)
find_dependency(
    Qt@QT_VERSION_MAJOR@ @QT_VERSION_MAJOR@ 
    COMPONENTS 
        @QLMDB_REQUIRED_QT_DEPENDENCIES@
)

include("${CMAKE_CURRENT_LIST_DIR}/QLMDBTargets.cmake")
