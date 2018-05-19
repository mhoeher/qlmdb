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
    Context();

private:

    QScopedPointer<ContextPrivate> d_ptr;

    Q_DECLARE_PRIVATE(Context)
};

} // namespace Core
} // namespace QLMDB

#endif // CONTEXT_H
