add_executable(
    tst_cursor
    tst_cursor_test.cpp
)

target_link_libraries(
    tst_cursor
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    qlmdb-qt${QT_VERSION_MAJOR}
)

add_test(NAME cursor COMMAND tst_cursor)
