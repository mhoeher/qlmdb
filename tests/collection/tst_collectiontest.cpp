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
    void initTestCase();
    void cleanupTestCase();
    void defaultConstructor();
    void collectionFromDatabase();
private:
    QTemporaryDir *tempDir;
};

CollectionTest::CollectionTest()
{
}

void CollectionTest::initTestCase()
{
    tempDir = new QTemporaryDir();
}

void CollectionTest::cleanupTestCase()
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

QTEST_APPLESS_MAIN(CollectionTest)

#include "tst_collectiontest.moc"
