#include <QObject>

#include "errors.h"
#include "transactionprivate.h"

namespace QLMDB {
namespace Core {

TransactionPrivate::TransactionPrivate(Context &context) :
    context(context),
    txn(nullptr),
    lastError(0),
    lastErrorString(),
    valid(false)
{

}

void TransactionPrivate::handleOpenError()
{
    if (lastError == 0) {
        valid = true;
    } else if (lastError == Errors::Panic) {
        lastErrorString = QObject::tr("Fatal error in environment");
    } else if (lastError == Errors::MapResized) {
        lastErrorString = QObject::tr("The environment map size has "
                                         "been resized by another "
                                         "process");
    } else if (lastError == Errors::ReadersFull) {
        lastErrorString = QObject::tr("Cannot create more readers");
    } else if (lastError == Errors::OutOfMemory) {
        lastErrorString = QObject::tr("No free memory to start "
                                         "transaction");
    } else {
        lastErrorString = QObject::tr("Unexpected error creating transaction");
    }
}

} // namespace Core
} // namespace QLMDB
