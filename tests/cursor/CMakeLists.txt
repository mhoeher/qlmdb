add_executable(
    tst_cursor
    tst_cursor_test.cpp
)

target_link_libraries(
    tst_cursor
    Qt5::Core
    Qt5::Test
    qlmdb
)

add_test(NAME cursor COMMAND tst_cursor)
