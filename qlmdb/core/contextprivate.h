#ifndef CONTEXTPRIVATE_H
#define CONTEXTPRIVATE_H

#include "lmdb.h"

#include <QString>

namespace QLMDB {
namespace Core {

//! @private
class ContextPrivate
{
public:
    ContextPrivate();
    virtual ~ContextPrivate();

    MDB_env *env;
    int lastError;
    QString lastErrorString;
    QString path;
    unsigned int flags;
    unsigned int mode;

    inline void clearLastError() {
        lastError = 0;
        lastErrorString.clear();
    }
};

} // namespace Core
} // namespace QLMDB

#endif // CONTEXTPRIVATE_H
