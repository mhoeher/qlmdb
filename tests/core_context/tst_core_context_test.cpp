#include <QString>
#include <QTemporaryDir>
#include <QtTest>

#include "core/context.h"
#include "core/errors.h"

using namespace QLMDB::Core;

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

#include "tst_core_context_test.moc"
