add_executable(
    tst_transaction
    tst_transaction_test.cpp
)

target_link_libraries(
    tst_transaction
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    qlmdb-qt${QT_VERSION_MAJOR}
)

add_test(NAME transaction COMMAND tst_transaction)
