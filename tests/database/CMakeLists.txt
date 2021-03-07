add_executable(
    tst_database
    tst_database_test.cpp
)

target_link_libraries(
    tst_database
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    qlmdb
)

add_test(NAME database COMMAND tst_database)
