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
 * along with QLMDB.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "transaction.h"
#include "transactionprivate.h"
#include "context.h"
#include "contextprivate.h"
#include "errors.h"

namespace QLMDB {

/**
 * @class Transaction
 * @brief A transaction within an LMDB environment.
 *
 * Transactions are used to provide a consistent view on the data
 * within an LMDB environment. They are used for both reading and writing
 * data.
 *
 * ## Creating a Transaction
 *
 * Transactions are created from an opened Context:
 *
 * ```
 * Context ctx;
 * ctx.setPath("/tmp/my_ctx/");
 * if (ctx.open()) {
 *     Transaction txn(ctx);
 * }
 * ```
 *
 * Note that the transaction is bound to this context; you must ensure that
 * the context remains valid while the transaction is alive.
 *
 * Alternatively, a transaction can be nested to another one:
 *
 * ```
 * Context ctx;
 * ctx.setPath("/tmp/my_ctx/");
 * if (ctx.open()) {
 *     Transaction parentTxn(ctx);
 *     Transaction childTxn(parentTxn);
 * }
 * ```
 *
 *
 * ## Notes About Multi-Threading
 *
 * A transaction (and any Cursor created in it) must only be used by one
 * thread, i.e. do not create a Transaction in one thread and use it
 * in another one.
 *
 * Additionally, every thread must have at most one active transaction
 * at a time.
 */


/**
 * @brief Create a read-only transaction.
 *
 * This flag causes a newly created transaction to be opened read-only.
 * The transaction can only be used to retrieve data from the context,
 * but not write to it.
 */
const unsigned int Transaction::ReadOnly = MDB_RDONLY;


/**
 * @brief Create a transaction in a context.
 *
 * This creates a new transaction in the given @p context. The context
 * must be open (@sa Context::isOpen()) before calling this.
 *
 * If creating the transaction was successfull, the isValid() property
 * will be false. If the context is valid but an error happened while
 * creating the transaction, you can check the lastError() and
 * lastErrorString() properties to learn why the operation failed.
 *
 * The @p flags value should be either 0 (the default value) or a bitwise
 * OR-combination of the following values:
 *
 * - #ReadOnly
 */
Transaction::Transaction(Context &context, unsigned int flags) :
    d_ptr(new TransactionPrivate(context))
{
    Q_D(Transaction);
    if (context.isOpen()) {
        d->lastError = mdb_txn_begin(
                    context.d_ptr->env, nullptr, flags, &d->txn);
        d->handleOpenError();
    }
}


/**
 * @brief Create a nested transaction.
 *
 * This is an alternative constructor which takes a @p parent transaction.
 * The newly created transaction will be nested in the parent one. The
 * @p flags have the same meaning as the constructor which takes a
 * Context instead.
 */
Transaction::Transaction(Transaction &parent, unsigned int flags) :
    d_ptr(new TransactionPrivate(parent.d_ptr->context))
{
    Q_D(Transaction);
    if (d->context.isOpen()) {
        d->lastError = mdb_txn_begin(
                    d->context.d_ptr->env,
                    parent.d_ptr->txn,
                    flags, &d->txn);
        d->handleOpenError();
    }
}


/**
 * @brief Destructor.
 *
 * Destroys the transaction. If the transaction is still active (i.e.
 * isValid() is true), commit() will be called to finish the transaction
 * and write any data back to the environment.
 *
 * Either commit() or abort() can be called before the descrutor is called
 * to close the transaction earlier.
 */
Transaction::~Transaction()
{
    Q_D(Transaction);
    if (d->valid) {
        commit();
    }
}


/**
 * @brief Indicates if the transaction is valid.
 *
 * This property is true if the transaction is valid, i.e. it has been
 * created with an opened Context or a valid Transaction as parent.
 */
bool Transaction::isValid() const
{
    const Q_D(Transaction);
    return d->valid;
}


/**
 * @brief The last error that happened within the transaction.
 *
 * This property holds the last error that happened. If no error
 * occurred, this is zero. For a list of error codes, please refer to
 * the QLMDB::Core::Errors namespace.
 */
int Transaction::lastError() const
{
    const Q_D(Transaction);
    return d->lastError;
}


/**
 * @brief A textual description of the last error.
 *
 * This property holds a textual description of the last error that occurred.
 */
QString Transaction::lastErrorString() const
{
    const Q_D(Transaction);
    return d->lastErrorString;
}


/**
 * @brief Commit the transaction.
 *
 * This will write any changes made in the transaction to the environment.
 * If the transaction is read-write, any open Cursor will become invalid and
 * must not be reused. For read-only transactions, cursors stay valid.
 *
 * The method returns true of committing was successful or false otherwise.
 * Check the value of lastError() to learn what error occurred otherwise.
 */
bool Transaction::commit()
{
    bool result = false;
    Q_D(Transaction);
    if (d->valid) {
        d->lastError = mdb_txn_commit(d->txn);
        d->valid = false;
        if (d->lastError == 0) {
            result = true;
            d->lastErrorString.clear();
        } else if (d->lastError == Errors::InvalidParameter) {
            d->lastErrorString = QObject::tr("Invalid parameters passed when "
                                             "committing transaction");
        } else if (d->lastError == Errors::OutOfDiskSpace) {
            d->lastErrorString = QObject::tr("No more space on disk");
        } else if (d->lastError == Errors::IOError) {
            d->lastErrorString = QObject::tr("Low-level I/O error occurred "
                                             "during transaction commit");
        } else if (d->lastError == Errors::OutOfMemory) {
            d->lastErrorString = QObject::tr("Not enough free memory to "
                                             "commit transaction");
        }
    }
    return result;
}


/**
 * @brief Abort the transaction.
 *
 * The transaction is aborted and none of the changes (in case of read-write
 * transactions) are written to disk.
 *
 * If the transaction is read-write, any Cursor belonging to the transaction
 * becomes invalid. For read-only transactions, cursors can be renewed by
 * attaching them to another read-only transaction.
 */
bool Transaction::abort()
{
    bool result = false;
    Q_D(Transaction);
    if (d->valid) {
        mdb_txn_abort(d->txn);
        result = true;
        d->valid = false;
    }
    return result;
}

} // namespace QLMDB
