#include <QString>
#include <QTemporaryDir>
#include <QtTest>

#include "core/context.h"
#include "core/transaction.h"
#include "core/errors.h"

using namespace QLMDB::Core;


class Core_Transaction_Test : public QObject
{
    Q_OBJECT

public:
    Core_Transaction_Test();

private Q_SLOTS:
    void init();
    void cleanup();
    void constructor();
    void commit();
    void abort();

private:

    QTemporaryDir *tmpDir;
};

Core_Transaction_Test::Core_Transaction_Test() : tmpDir(nullptr)
{
}

void Core_Transaction_Test::init()
{
    tmpDir = new QTemporaryDir;
}

void Core_Transaction_Test::cleanup()
{
    delete tmpDir;
}

void Core_Transaction_Test::constructor()
{
    Context context;
    context.setPath(tmpDir->path());
    QVERIFY(context.open());

    Transaction txn(context);
    QVERIFY(txn.isValid());
    QCOMPARE(txn.lastError(), Errors::NoError);
    QVERIFY(txn.lastErrorString().isEmpty());

    Transaction innerTxn(txn);
    QVERIFY(innerTxn.isValid());
    QCOMPARE(innerTxn.lastError(), Errors::NoError);
    QVERIFY(innerTxn.lastErrorString().isEmpty());
}

void Core_Transaction_Test::commit()
{
    Context context;
    context.setPath(tmpDir->path());
    QVERIFY(context.open());

    Transaction txn(context);
    QVERIFY(txn.isValid());

    QVERIFY(txn.commit());
    QCOMPARE(txn.lastError(), Errors::NoError);
    QVERIFY(txn.lastErrorString().isEmpty());

    QVERIFY(!txn.commit());
    // Note: Operations on invalid transaction trigger no last error
    QCOMPARE(txn.lastError(), Errors::NoError);
    QVERIFY(txn.lastErrorString().isEmpty());
}

void Core_Transaction_Test::abort()
{
    Context context;
    context.setPath(tmpDir->path());
    QVERIFY(context.open());

    Transaction txn(context);
    QVERIFY(txn.isValid());

    QVERIFY(txn.abort());
    QCOMPARE(txn.lastError(), Errors::NoError);
    QVERIFY(txn.lastErrorString().isEmpty());

    QVERIFY(!txn.abort());
    // Note: Operations on invalid transaction trigger no last error
    QCOMPARE(txn.lastError(), Errors::NoError);
    QVERIFY(txn.lastErrorString().isEmpty());
}

QTEST_APPLESS_MAIN(Core_Transaction_Test)

#include "tst_core_transaction_test.moc"
