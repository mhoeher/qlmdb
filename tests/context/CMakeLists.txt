add_executable(
    tst_context
    tst_context_test.cpp
)

target_link_libraries(
    tst_context
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    qlmdb
)

add_test(NAME context COMMAND tst_context)
