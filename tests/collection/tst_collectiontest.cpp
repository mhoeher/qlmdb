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

#include "collection.h"
#include "database.h"

class CollectionTest : public QObject
{
    Q_OBJECT

public:
    CollectionTest();

private Q_SLOTS:
    void init();
    void cleanup();
    void defaultConstructor();
    void collectionFromDatabase();
    void put();
    void get();
    void multiPut();
    void getAll();
private:
    QTemporaryDir *tempDir;
};

CollectionTest::CollectionTest()
{
}

void CollectionTest::init()
{
    tempDir = new QTemporaryDir();
}

void CollectionTest::cleanup()
{
    delete tempDir;
}

void CollectionTest::defaultConstructor()
{
    QLMDB::Collection c;
    QVERIFY(!c.isValid());
}

void CollectionTest::collectionFromDatabase()
{
    QLMDB::Database db;
    QVERIFY(db.open(tempDir->path()));
    auto c = db.collection("test");
    QVERIFY(c.isValid());
    QCOMPARE(c.name(), QByteArray("test"));
}

void CollectionTest::put()
{
    QLMDB::Database db;
    QVERIFY(db.open(tempDir->path()));
    auto c = db.collection("test");
    QVERIFY(c.isValid());
    QVERIFY(c.put("foo", "bar"));
    QVERIFY(c.put("Hello World", "Hurray!"));
}

void CollectionTest::get()
{
    QLMDB::Database db;
    QVERIFY(db.open(tempDir->path()));
    auto c = db.collection("test");
    QVERIFY(c.isValid());
    QVERIFY(c.put("foo", "bar"));
    QVERIFY(c.put("Hello World", "Hurray!"));
    QCOMPARE(c.get("foo"), QByteArray("bar"));
    QCOMPARE(c.get("Hello World"), QByteArray("Hurray!"));
    QVERIFY((c.get("bar").isNull()));
    QCOMPARE(c.get("baz", "none"), QByteArray("none"));
    QVERIFY(c.put("foo", "foot"));
    QCOMPARE(c.get("foo"), QByteArray("foot"));
}

void CollectionTest::multiPut()
{
    QLMDB::Database db;
    QVERIFY(db.open(tempDir->path()));
    auto c = db.collection("test", QLMDB::Collection::MultiValues);
    QVERIFY(c.isValid());
    QVERIFY(c.put("foo", "1"));
    QVERIFY(c.put("foo", "2"));
    QVERIFY(c.put("foo", "3"));
    QVERIFY(c.put("bar", "A"));
    QVERIFY(c.put("bar", "B"));
}

void CollectionTest::getAll()
{
    QLMDB::Database db;
    QVERIFY(db.open(tempDir->path()));
    auto c = db.collection("test", QLMDB::Collection::MultiValues);
    QVERIFY(c.isValid());
    QVERIFY(c.put("foo", "1"));
    QVERIFY(c.put("foo", "2"));
    QVERIFY(c.put("foo", "3"));
    QVERIFY(c.put("bar", "A"));
    QVERIFY(c.put("bar", "B"));
    QCOMPARE(c.getAll("foo"),
             QByteArrayList({"1", "2", "3"})
             );
    QCOMPARE(c.getAll("bar"),
             QByteArrayList({"A", "B"})
             );
    QCOMPARE(c.getAll("baz"),
             QByteArrayList()
             );
}

QTEST_APPLESS_MAIN(CollectionTest)

#include "tst_collectiontest.moc"
