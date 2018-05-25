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
