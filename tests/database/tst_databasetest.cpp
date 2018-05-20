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

#include <database.h>

class DatabaseTest : public QObject
{
    Q_OBJECT

public:
    DatabaseTest();

private Q_SLOTS:
    void init();
    void cleanup();
    void open();
    void maxTables();
    void copyConstructor();
    void assignmentOperator();

private:
    QTemporaryDir *tempDir;
};

DatabaseTest::DatabaseTest() : QObject(),
    tempDir()
{
}

void DatabaseTest::init()
{
    tempDir = new QTemporaryDir();
}

void DatabaseTest::cleanup()
{
    delete tempDir;
}

void DatabaseTest::open()
{
    QLMDB::Database db;
    QVERIFY(!db.isOpen());
    QVERIFY(db.open(tempDir->path()));
    QVERIFY(db.isOpen());
    QVERIFY(!db.hasError());
    QCOMPARE(db.lastErrorString(), QString());

    QLMDB::Database db2;
    QVERIFY(!db2.isOpen());
    QVERIFY(!db2.open(""));
    QVERIFY(!db2.isOpen());
    QVERIFY(db2.hasError());

    QTemporaryDir temp;
    QFile file(temp.path() + "/file");
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();
    QLMDB::Database db3;
    QVERIFY(!db3.isOpen());
    QVERIFY(!db3.open(temp.path() + "/file"));
    QVERIFY(!db3.isOpen());
    QVERIFY(db3.hasError());
}

void DatabaseTest::maxTables()
{
    QLMDB::Database db;
    QCOMPARE(db.maxTables(), QLMDB::Database::DefaultMaxTables);
    db.setMaxTables(-10);
    QCOMPARE(db.maxTables(), QLMDB::Database::DefaultMaxTables);
    QVERIFY(db.hasError());
    db.clearError();
    QVERIFY(!db.hasError());
    db.setMaxTables(20);
    QCOMPARE(db.maxTables(), 20);
    QVERIFY(!db.hasError());
    QVERIFY(db.open(tempDir->path()));
    db.setMaxTables(10);
    QVERIFY(db.hasError());
    QCOMPARE(db.maxTables(), 20);
}

void DatabaseTest::copyConstructor()
{
    QLMDB::Database db;
    QVERIFY(db.open(tempDir->path()));
    QVERIFY(db.isOpen());

    {
        QLMDB::Database db2(db);
        QVERIFY(db2.isOpen());
    }

    QVERIFY(db.isOpen());
}

void DatabaseTest::assignmentOperator()
{
    QLMDB::Database db;
    QVERIFY(db.open(tempDir->path()));
    QVERIFY(db.isOpen());

    {
        QLMDB::Database db2;
        db2 = db;
        QVERIFY(db2.isOpen());
    }

    QVERIFY(db.isOpen());
}

QTEST_APPLESS_MAIN(DatabaseTest)

#include "tst_databasetest.moc"
