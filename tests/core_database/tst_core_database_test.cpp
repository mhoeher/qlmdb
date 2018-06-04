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
    void put();
    void get();
    void operatorArraySubscript();
    void getAll();

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

void Core_Database_Test::put()
{
    Context ctx;
    ctx.setPath(tmpDir->path());
    ctx.setMaxDBs(1);
    QVERIFY(ctx.open());

    Database db(ctx);

    QVERIFY(db.put("a", "foo"));
    QVERIFY(db.put<int>(1, "Test"));

    {
        Transaction txn(ctx);
        QVERIFY(db.put(txn, "b", "bar"));
        QVERIFY(db.put<int>(txn, 2, "abcde"));
    }


    Database mdb(ctx, "multi", Database::MultiValues | Database::Create);

    QVERIFY(mdb.put("a", "foo1"));
    QVERIFY(mdb.put("a", "foo2"));
}

void Core_Database_Test::get()
{
    Context ctx;
    ctx.setPath(tmpDir->path());
    ctx.setMaxDBs(1);
    QVERIFY(ctx.open());

    Database db(ctx);

    QVERIFY(db.put("a", "foo"));
    QVERIFY(db.put<int>(1, "Test"));
    QCOMPARE(db.get("a"), QByteArray("foo"));
    QCOMPARE(db.get<int>(1), QByteArray("Test"));
    QCOMPARE(db.get("b"), QByteArray());
    QCOMPARE(db.get<int>(2), QByteArray());

    {
        Transaction txn(ctx);
        QVERIFY(db.put(txn, "b", "bar"));
        QVERIFY(db.put<int>(txn, 2, "abcde"));
        QCOMPARE(db.get(txn, "b"), QByteArray("bar"));
        QCOMPARE(db.get<int>(txn, 2), QByteArray("abcde"));
        QCOMPARE(db.get(txn, "c"), QByteArray());
        QCOMPARE(db.get<int>(txn, 3), QByteArray());
    }


    Database mdb(ctx, "multi", Database::MultiValues | Database::Create);

    QVERIFY(mdb.put("a", "foo1"));
    QVERIFY(mdb.put("a", "foo2"));
    QCOMPARE(mdb.get("a"), QByteArray("foo1"));
}

void Core_Database_Test::operatorArraySubscript()
{
    Context ctx;
    ctx.setPath(tmpDir->path());
    ctx.setMaxDBs(1);
    QVERIFY(ctx.open());

    Database db(ctx);

    QVERIFY(db.put("a", "foo"));
    QVERIFY(db.put<int>(1, "Test"));
    QCOMPARE(db["a"], QByteArray("foo"));
    QCOMPARE(db["b"], QByteArray());

    Database mdb(ctx, "multi", Database::MultiValues | Database::Create);
    QVERIFY(mdb.put("a", "foo1"));
    QVERIFY(mdb.put("a", "foo2"));
    QCOMPARE(mdb["a"], QByteArray("foo1"));
}

void Core_Database_Test::getAll()
{
    Context ctx;
    ctx.setPath(tmpDir->path());
    ctx.setMaxDBs(1);
    QVERIFY(ctx.open());

    Database mdb(ctx, "multi", Database::MultiValues | Database::Create);

    QVERIFY(mdb.put("a", "foo1"));
    QVERIFY(mdb.put("a", "foo2"));
    QVERIFY(mdb.put("a", "foo3"));
    QVERIFY(mdb.put("b", "bar1"));
    QVERIFY(mdb.put<int>(1, "baz1"));
    QVERIFY(mdb.put<int>(1, "baz2"));
    QCOMPARE(mdb.getAll("a"), QByteArrayList({"foo1", "foo2", "foo3"}));
    QCOMPARE(mdb.getAll("b"), QByteArrayList({"bar1"}));
    QCOMPARE(mdb.getAll("c"), QByteArrayList());
    QCOMPARE(mdb.getAll<int>(1), QByteArrayList({"baz1", "baz2"}));
    QCOMPARE(mdb.getAll<int>(2), QByteArrayList());

    {
        Transaction txn(ctx);
        QCOMPARE(mdb.getAll(txn, "a"), QByteArrayList({"foo1", "foo2", "foo3"}));
        QCOMPARE(mdb.getAll(txn, "b"), QByteArrayList({"bar1"}));
        QCOMPARE(mdb.getAll(txn, "c"), QByteArrayList());
        QCOMPARE(mdb.getAll<int>(txn, 1), QByteArrayList({"baz1", "baz2"}));
        QCOMPARE(mdb.getAll<int>(txn, 2), QByteArrayList());
    }
}

QTEST_APPLESS_MAIN(Core_Database_Test)

#include "tst_core_database_test.moc"
