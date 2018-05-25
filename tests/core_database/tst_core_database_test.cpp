#include <QString>
#include <QTemporaryDir>
#include <QtTest>

#include "core/context.h"
#include "core/database.h"
#include "core/errors.h"
#include "core/transaction.h"

using namespace QLMDB::Core;


class Core_Database_Test : public QObject
{
    Q_OBJECT

public:
    Core_Database_Test();

private Q_SLOTS:
    void init();
    void cleanup();
    void fromContext();
    void fromTransaction();

private:

    QTemporaryDir *tmpDir;
};

Core_Database_Test::Core_Database_Test() :
    tmpDir(nullptr)
{
}

void Core_Database_Test::init()
{
    tmpDir = new QTemporaryDir();
}

void Core_Database_Test::cleanup()
{
    delete tmpDir;
}

void Core_Database_Test::fromContext()
{
    Context ctx;
    ctx.setPath(tmpDir->path());
    ctx.setMaxDBs(10);
    QVERIFY(ctx.open());

    Transaction txn(ctx);

    Database defaultDB(txn);
    QVERIFY(defaultDB.isValid());
    QCOMPARE(defaultDB.lastError(), Errors::NoError);
    QCOMPARE(defaultDB.lastErrorString(), QString());

    Database testDB(txn, "test");
    QVERIFY(testDB.isValid());
    QCOMPARE(testDB.lastError(), Errors::NoError);
    QCOMPARE(testDB.lastErrorString(), QString());

    Context foo;
    Transaction fooTxn(foo);
    Database invalidDB(fooTxn);
    QVERIFY(!invalidDB.isValid());
    // Note: DB in invalid context: Not valid but no error either!
    QCOMPARE(invalidDB.lastError(), Errors::NoError);
    QCOMPARE(invalidDB.lastErrorString(), QString());
}

void Core_Database_Test::fromTransaction()
{

}

QTEST_APPLESS_MAIN(Core_Database_Test)

#include "tst_core_database_test.moc"
