#ifndef TRANSACTIONPRIVATE_H
#define TRANSACTIONPRIVATE_H

#include "lmdb.h"

#include <QString>

#include "context.h"

namespace QLMDB {
namespace Core {

//! @private
class TransactionPrivate
{
public:
    explicit TransactionPrivate(Context &context);

    Context &context;
    MDB_txn *txn;
    int lastError;
    QString lastErrorString;
    bool valid;

    void handleOpenError();
};

} // namespace Core
} // namespace QLMDB

#endif // TRANSACTIONPRIVATE_H
