#ifndef CONTEXT_H
#define CONTEXT_H

#include <QtGlobal>
#include <QScopedPointer>


namespace QLMDB {
namespace Core {

class ContextPrivate;

class Context
{
public:
    static const unsigned int FixedMap;
    static const unsigned int NoSubDir;
    static const unsigned int ReadOnly;
    static const unsigned int WriteMap;
    static const unsigned int NoMetaSync;
    static const unsigned int NoSync;
    static const unsigned int MapAsync;
    static const unsigned int NoTLS;
    static const unsigned int NoLock;
    static const unsigned int NoReadAhead;
    static const unsigned int NoMemInit;

    Context();

    int lastError() const;
    QString lastErrorString() const;
    void clearLastError();

    QString path() const;
    void setPath(const QString &path);

    unsigned int flags() const;
    void setFlags(unsigned int flags);

    unsigned int mode() const;
    void setMode(unsigned int mode);

private:

    QScopedPointer<ContextPrivate> d_ptr;

    Q_DECLARE_PRIVATE(Context)
};

} // namespace Core
} // namespace QLMDB

#endif // CONTEXT_H
