/*
 * This file is part of QLMDB.
 *
 * QLMDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * QLMDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

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

    Database defaultDB(ctx);
    QVERIFY(defaultDB.isValid());
    QCOMPARE(defaultDB.lastError(), Errors::NoError);
    QCOMPARE(defaultDB.lastErrorString(), QString());

    Database testDB(ctx, "test");
    QVERIFY(testDB.isValid());
    QCOMPARE(testDB.lastError(), Errors::NoError);
    QCOMPARE(testDB.lastErrorString(), QString());

    Database nonExistingDB(ctx, "nonExisting", 0);
    QVERIFY(!nonExistingDB.isValid());
    QCOMPARE(nonExistingDB.lastError(), Errors::NotFound);
    QVERIFY(!nonExistingDB.lastErrorString().isEmpty());
}

void Core_Database_Test::fromTransaction()
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

    Database nonExistingDB(txn, "nonExisting", 0);
    QVERIFY(!nonExistingDB.isValid());
    QCOMPARE(nonExistingDB.lastError(), Errors::NotFound);
    QVERIFY(!nonExistingDB.lastErrorString().isEmpty());

    Context foo;
    Transaction fooTxn(foo);
    Database invalidDB(fooTxn);
    QVERIFY(!invalidDB.isValid());
    // Note: DB in invalid context: Not valid but no error either!
    QCOMPARE(invalidDB.lastError(), Errors::NoError);
    QCOMPARE(invalidDB.lastErrorString(), QString());
}

QTEST_APPLESS_MAIN(Core_Database_Test)

#include "tst_core_database_test.moc"
