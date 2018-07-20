add_executable(
    tst_database
    tst_database_test.cpp
)

target_link_libraries(
    tst_database
    Qt5::Core
    Qt5::Test
    qlmdb
)

add_test(NAME database COMMAND tst_database)
