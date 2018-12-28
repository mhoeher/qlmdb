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

#include "qlmdb/context.h"
#include "qlmdb/errors.h"

using namespace QLMDB;

class Core_Context_Test : public QObject
{
    Q_OBJECT

public:
    Core_Context_Test();

private Q_SLOTS:
    void init();
    void cleanup();
    void constructor();
    void open();
    void open_with_empty_path();
    void clearLastError();

private:

    QTemporaryDir *tmpDir;
};

Core_Context_Test::Core_Context_Test() : tmpDir(nullptr)
{
}

void Core_Context_Test::init()
{
    tmpDir = new QTemporaryDir();
}

void Core_Context_Test::cleanup()
{
    delete tmpDir;
}

void Core_Context_Test::constructor()
{
    Context context;
    QVERIFY(!context.open());
}

void Core_Context_Test::open()
{
    Context context;
    context.setPath(tmpDir->path());
    context.setMapSize(5000);
    QVERIFY(context.open());
    QVERIFY(context.isOpen());
    QCOMPARE(context.lastError(), Errors::NoError);
}

void Core_Context_Test::open_with_empty_path()
{
    Context context;
    QVERIFY(!context.open());
    QCOMPARE(context.lastError(), Errors::InvalidPath);
    QVERIFY(!context.isOpen());
}

void Core_Context_Test::clearLastError()
{
    Context context;
    QVERIFY(!context.open());
    QVERIFY(context.lastError() != Errors::NoError);
    QVERIFY(!context.lastErrorString().isEmpty());
    context.clearLastError();
    QVERIFY(context.lastError() == Errors::NoError);
    QVERIFY(context.lastErrorString().isEmpty());
}

QTEST_APPLESS_MAIN(Core_Context_Test)

#include "tst_context_test.moc"
