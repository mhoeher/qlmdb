add_executable(
    tst_context
    tst_context_test.cpp
)

target_link_libraries(
    tst_context
    Qt5::Core
    Qt5::Test
    qlmdb
)

add_test(NAME context COMMAND tst_context)
