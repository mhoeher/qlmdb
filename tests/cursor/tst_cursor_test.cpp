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
#include <QTemporaryFile>
#include <QtTest>

#include "qlmdb/context.h"
#include "qlmdb/cursor.h"
#include "qlmdb/database.h"
#include "qlmdb/errors.h"
#include "qlmdb/transaction.h"

using namespace QLMDB;

class Core_Cursor_Test : public QObject
{
    Q_OBJECT

public:
    Core_Cursor_Test();

private Q_SLOTS:
    void init();
    void cleanup();
    void constructor();
    void put();
    void get();
    void remove();

private:
    QTemporaryDir *tmpDir;
};

Core_Cursor_Test::Core_Cursor_Test() : tmpDir(nullptr)
{
}

void Core_Cursor_Test::init()
{
    tmpDir = new QTemporaryDir();
}

void Core_Cursor_Test::cleanup()
{
    delete tmpDir;
}

void Core_Cursor_Test::constructor()
{
    {
        Context ctx;
        ctx.setPath(tmpDir->path());
        ctx.setMapSize(5000);
        QVERIFY(ctx.open());
        Transaction txn(ctx);
        QVERIFY(txn.isValid());
        Database db(txn);
        QVERIFY(db.isValid());
        Cursor cursor(txn, db);
        QVERIFY(cursor.isValid());
        QCOMPARE(cursor.lastError(), Errors::NoError);
        QVERIFY(cursor.lastErrorString().isEmpty());
    }

    {
        Context ctx;
        QVERIFY(!ctx.open());
        Transaction txn(ctx);
        QVERIFY(!txn.isValid());
        Database db(txn);
        QVERIFY(!db.isValid());
        Cursor cursor(txn, db);
        QVERIFY(!cursor.isValid());
        QCOMPARE(cursor.lastError(), Errors::InvalidParameter);
        QVERIFY(!cursor.lastErrorString().isEmpty());
    }
}

void Core_Cursor_Test::put()
{
    {
        Context ctx;
        ctx.setPath(tmpDir->path());
        ctx.setMaxDBs(1);
        ctx.setMapSize(5000);
        QVERIFY(ctx.open());
        {
            Transaction txn(ctx);
            QVERIFY(txn.isValid());
            Database db(txn);
            QVERIFY(db.isValid());
            Cursor cursor(txn, db);
            QVERIFY(cursor.isValid());
            QVERIFY(cursor.put("msg", "Hello World"));
            QCOMPARE(cursor.currentKey(), QByteArray("msg"));
            QCOMPARE(cursor.currentValue(), QByteArray("Hello World"));
            QVERIFY(!cursor.put("msg", "Foo Bar Baz", Cursor::NoOverrideKey));
            QCOMPARE(cursor.lastError(), Errors::KeyExists);
            QCOMPARE(cursor.currentKey(), QByteArray("msg"));
            QCOMPARE(cursor.currentValue(), QByteArray("Hello World"));
            QVERIFY(cursor.put("msg", "Bye!"));
            QCOMPARE(cursor.currentKey(), QByteArray("msg"));
            QCOMPARE(cursor.currentValue(), QByteArray("Bye!"));
        }
        {
            Transaction txn(ctx);
            QVERIFY(txn.isValid());
            Database db(txn, "Test", Database::MultiValues | Database::Create);
            QVERIFY(db.isValid());
            Cursor cursor(txn, db);
            QVERIFY(cursor.isValid());
            QVERIFY(cursor.put("msg", "Hello World"));
            QCOMPARE(cursor.currentKey(), QByteArray("msg"));
            QCOMPARE(cursor.currentValue(), QByteArray("Hello World"));
            QVERIFY(!cursor.put("msg", "Hello World", Cursor::NoDuplicateData));
            QCOMPARE(cursor.lastError(), Errors::KeyExists);
            QCOMPARE(cursor.currentKey(), QByteArray("msg"));
            QCOMPARE(cursor.currentValue(), QByteArray("Hello World"));
            QVERIFY(cursor.put("msg", "Hello World"));
            QCOMPARE(cursor.currentKey(), QByteArray("msg"));
            QCOMPARE(cursor.currentValue(), QByteArray("Hello World"));
            QVERIFY(cursor.put("msg", "Bye!"));
            QCOMPARE(cursor.currentKey(), QByteArray("msg"));
            QCOMPARE(cursor.currentValue(), QByteArray("Bye!"));
        }
    }

    {
        Context ctx;
        QVERIFY(!ctx.open());
        Transaction txn(ctx);
        QVERIFY(!txn.isValid());
        Database db(txn);
        QVERIFY(!db.isValid());
        Cursor cursor(txn, db);
        QVERIFY(!cursor.isValid());
        QVERIFY(!cursor.put("msg", "Hello World"));
    }
}

void Core_Cursor_Test::get()
{
    Context ctx;
    ctx.setPath(tmpDir->path());
    ctx.setMaxDBs(1);
    ctx.setMapSize(5000);
    QVERIFY(ctx.open());
    {
        Transaction txn(ctx);
        QVERIFY(txn.isValid());
        Database db(txn);
        QVERIFY(db.isValid());
        Cursor cursor(txn, db);
        QVERIFY(cursor.isValid());
        QVERIFY(cursor.put("a", "foo"));
        QVERIFY(cursor.put("b", "bar"));
        QVERIFY(cursor.put("d", "baz"));
        QCOMPARE(cursor.first(), Cursor::FindResult("a", "foo"));
        QCOMPARE(cursor.last(), Cursor::FindResult("d", "baz"));
        QCOMPARE(cursor.current(), Cursor::FindResult("d", "baz"));
        QCOMPARE(cursor.previous(), Cursor::FindResult("b", "bar"));
        QCOMPARE(cursor.next(), Cursor::FindResult("d", "baz"));
        QCOMPARE(cursor.last(), Cursor::FindResult("d", "baz"));
        QCOMPARE(cursor.findKey("b"), Cursor::FindResult("b", "bar"));
        QCOMPARE(cursor.findFirstAfter("aa"), Cursor::FindResult("b", "bar"));
    }
    {
        Transaction txn(ctx);
        QVERIFY(txn.isValid());
        Database db(txn, "test", Database::MultiValues | Database::Create);
        QVERIFY(db.isValid());
        Cursor cursor(txn, db);
        QVERIFY(cursor.isValid());
        QVERIFY(cursor.put("a", "foo1"));
        QVERIFY(cursor.put("a", "foo2"));
        QVERIFY(cursor.put("a", "foo3"));
        QVERIFY(cursor.put("b", "bar1"));
        QVERIFY(cursor.put("d", "baz1"));
        QVERIFY(cursor.put("d", "baz2"));

        QCOMPARE(cursor.first(), Cursor::FindResult("a", "foo1"));
        QCOMPARE(cursor.nextForCurrentKey(), Cursor::FindResult("a", "foo2"));
        QCOMPARE(cursor.firstForCurrentKey(), Cursor::FindResult("a", "foo1"));
        QCOMPARE(cursor.lastForCurrentKey(), Cursor::FindResult("a", "foo3"));
        QCOMPARE(cursor.previousForCurrentKey(),
                 Cursor::FindResult("a", "foo2"));
        QCOMPARE(cursor.nextKey(), Cursor::FindResult("b", "bar1"));
        QCOMPARE(cursor.find("a", "foo2"), Cursor::FindResult("a", "foo2"));
        QCOMPARE(cursor.findNearest("b", "bar1"),
                 Cursor::FindResult("b", "bar1"));
        // TODO: Why is the cursor not positioned at "b"/"bar2"?
//        QCOMPARE(cursor.findNearest("b", "bar1.5"),
//                 Cursor::FindResult("b", "bar2"));
        QCOMPARE(cursor.findFirstAfter("c"), Cursor::FindResult("d", "baz1"));
    }
}

void Core_Cursor_Test::remove()
{
    Context ctx;
    ctx.setPath(tmpDir->path());
    ctx.setMaxDBs(1);
    ctx.setMapSize(5000);
    QVERIFY(ctx.open());
    {
        Transaction txn(ctx);
        QVERIFY(txn.isValid());
        Database db(txn);
        QVERIFY(db.isValid());
        Cursor cursor(txn, db);
        QVERIFY(cursor.isValid());
        QVERIFY(cursor.put("a", "foo"));
        QVERIFY(cursor.put("b", "bar"));
        QVERIFY(cursor.put("d", "baz"));
        QCOMPARE(cursor.findKey("b"), Cursor::FindResult("b", "bar"));
        QVERIFY(cursor.remove());
        QCOMPARE(cursor.first(), Cursor::FindResult("a", "foo"));
        QCOMPARE(cursor.next(), Cursor::FindResult("d", "baz"));
    }
    {
        Transaction txn(ctx);
        QVERIFY(txn.isValid());
        Database db(txn, "test", Database::MultiValues | Database::Create);
        QVERIFY(db.isValid());
        Cursor cursor(txn, db);
        QVERIFY(cursor.isValid());
        QVERIFY(cursor.put("a", "foo1"));
        QVERIFY(cursor.put("a", "foo2"));
        QVERIFY(cursor.put("a", "foo3"));
        QVERIFY(cursor.put("b", "bar1"));
        QVERIFY(cursor.put("b", "bar2"));
        QVERIFY(cursor.put("d", "baz1"));
        QVERIFY(cursor.put("d", "baz2"));

        QCOMPARE(cursor.find("a", "foo2"), Cursor::FindResult("a", "foo2"));
        QVERIFY(cursor.remove());
        QCOMPARE(cursor.findKey("b"), Cursor::FindResult("b", "bar1"));
        QVERIFY(cursor.remove(Cursor::RemoveAll));
        QCOMPARE(cursor.first(), Cursor::FindResult("a", "foo1"));
        QCOMPARE(cursor.next(), Cursor::FindResult("a", "foo3"));
        QCOMPARE(cursor.next(), Cursor::FindResult("d", "baz1"));
        QCOMPARE(cursor.next(), Cursor::FindResult("d", "baz2"));
        QCOMPARE(cursor.next(), Cursor::FindResult());

    }
}

QTEST_APPLESS_MAIN(Core_Cursor_Test)

#include "tst_cursor_test.moc"
