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
#ifndef TRANSACTIONPRIVATE_H
#define TRANSACTIONPRIVATE_H

#include "lmdb.h"

#include <QString>

#include "context.h"

namespace QLMDB {

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

} // namespace QLMDB

#endif // TRANSACTIONPRIVATE_H
