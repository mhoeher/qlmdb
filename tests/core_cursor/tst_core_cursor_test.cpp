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

#include "core/context.h"
#include "core/cursor.h"
#include "core/database.h"
#include "core/errors.h"
#include "core/transaction.h"

using namespace QLMDB::Core;

class Core_Cursor_Test : public QObject
{
    Q_OBJECT

public:
    Core_Cursor_Test();

private Q_SLOTS:
    void init();
    void cleanup();
    void constructor();

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

QTEST_APPLESS_MAIN(Core_Cursor_Test)

#include "tst_core_cursor_test.moc"
