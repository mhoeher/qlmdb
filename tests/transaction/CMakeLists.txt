add_executable(
    tst_transaction
    tst_transaction_test.cpp
)

target_link_libraries(
    tst_transaction
    Qt5::Core
    Qt5::Test
    qlmdb
)

add_test(NAME transaction COMMAND tst_transaction)
