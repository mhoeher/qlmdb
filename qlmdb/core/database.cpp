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
#include "lmdb.h"

#include "context.h"
#include "database.h"
#include "databaseprivate.h"
#include "transaction.h"
#include "transactionprivate.h"


namespace QLMDB {
namespace Core {

/**
 * @class Database
 * @brief Represents a Database within a Context.
 *
 * The Database class is used to represent a single database (i.e. a key-value
 * store) within a Context.
 *
 * ## Creating a Database
 *
 * Databases can be created either from a Context or a Transaction. The
 * recommended way is to create them once when creating the Context
 * and reuse them across the process:
 *
 * ```
 * Context ctx;
 * ctx.setPath("/tmp/db/");
 * ctx.setMaxDBs(10);
 * if (ctx.open()) {
 *     // Open database "test":
 *     Database testDb(ctx, "test);
 * }
 * ```
 *
 * Alternatively, a Database can be created from a Transaction. However,
 * note that you must guarantee that there is no other Transaction
 * ongoing when doing so:
 *
 * ```
 * Context ctx;
 * ctx.setPath("/tmp/db/");
 * ctx.setMaxDBs(10);
 * if (ctx.open()) {
 *     // Create a transaction:
 *     Transaction txn(context);
 *     // Open database "test":
 *     Database testDb(ctx, "test);
 *     // Commit the transaction - afterwards, the database
 *     // can be reused in any transaction:
 *     txn.commit();
 *     // ...
 * }
 * ```
 *
 * ## Database Names and Types
 *
 * Within an LMDB context, multiple databases can be used. Each context
 * as a default database, which can be opened by passing an empty string
 * as name into the Database constructors. Note that in order to use
 * named databases, you have to use Context::setMaxDBs() before opening
 * a Context.
 *
 * By default, a database is a plain key-value store (where both keys and
 * values can be arbitrary byte arrays). The flags parameter of the
 * constructors can be used to finetune how a database behaves. There are
 * several flags, which can be used to:
 *
 * - Allow one key in a database to have multiple values.
 * - Tell that keys are integers (in native byte order).
 * - Compare keys as strings but start comparing them from the end.
 * - And several others.
 *
 * Note that in order to create a Database, the Database::Create flag has
 * to be passed to the constructor when accessing the database for the
 * very first time. This is the case with the default value, but when using
 * own flags, make sure you set the Create flag or handle the failed opening
 * otherwise.
 *
 * When a database is created, the settings are saved in the environment,
 * so when opening an existing database, flags passed into the
 * constructor might be ignored. To change the flags, drop() the database
 * and re-create it.
 */

/**
 * @brief Interpret keys as strings and compare them starting from the end.
 */
const unsigned int Database::ReverseKey = MDB_REVERSEKEY;

/**
 * @brief Allow multiple values to be saved under one key.
 */
const unsigned int Database::MultiValues = MDB_DUPSORT;

/**
 * @brief Keys are binary integers in native byte order.
 */
const unsigned int Database::IntegerKeys = MDB_INTEGERKEY;

/**
 * @brief All keys have the same size.
 *
 * This implies Database::MultiValues.
 */
const unsigned int Database::FixedSizeMultiValues = MDB_DUPFIXED | MDB_DUPSORT;

/**
 * @brief Like IntegerKeys but allow multiple values to be saved per key.
 */
const unsigned int Database::IntegerKeysMultiValues = MDB_INTEGERDUP;

/**
 * @brief Compare keys as strings beginning at the end.
 *
 * This is useful when using the MultiValues option.
 */
const unsigned int Database::ReverseKeyMultiValues = MDB_REVERSEDUP;

/**
 * @brief Create the database if it does not exist already.
 */
const unsigned int Database::Create = MDB_CREATE;


/**
 * @brief Open a Database.
 *
 * This opens the database with the specified @p name in the @p context. The
 * database is opened using the given @p flags.
 *
 * Flags is a bitwise or-combination of the following:
 *
 * - ReverseKey
 * - MultiValues
 * - IntegerKeys
 * - FixedSizeMultiValues
 * - IntegerKeysMultiValues
 * - ReverseKeyMultiValues
 * - Create
 *
 * By default, flags is set to Create, which causes the database to be
 * created if it does not exist. If the name is an empty string, the default
 * database is opened.
 *
 * If opening the database suceeded, the isValid() property is true.
 * Otherwise, use lastError() to find out what went wrong. Opening a
 * database on a Context which is not open will also cause the Database
 * to be invalid, but no error to be set.
 *
 * ## Notes on Multi-Threading
 *
 * If you use this constructor, make sure there is no active Transaction
 * ongoing.
 */
Database::Database(Context &context, const QString &name, unsigned int flags) :
    d_ptr(new DatabasePrivate)
{
    Q_D(Database);
    d->initFromContext(context, nullptr, name, flags);
}


/**
 * @brief Open a Database.
 *
 * This is an overloaded constructor. It takes a @p transaction and opens
 * the database named @p name with the given @p flags.
 *
 * ## Notes on Multi-Threading
 *
 * Use this constructor only if there is no other transaction running.
 * The resulting Database object can be used by other Transactions
 * after the current transaction has either been comitted or aborted.
 */
Database::Database(Transaction &transaction, const QString &name,
                   unsigned int flags) :
    d_ptr(new DatabasePrivate)
{
    if (transaction.isValid()) {
        Q_D(Database);
        d->initFromContext(transaction.d_ptr->context, &transaction,
                           name, flags);
    }
}


/**
 * @brief Destructor.
 */
Database::~Database()
{
    Q_D(Database);
    if (d->valid) {
        mdb_dbi_close(d->env, d->db);
    }
}


/**
 * @brief Is the database valid.
 *
 * This property is true if the database is valid.
 */
bool Database::isValid() const
{
    const Q_D(Database);
    return d->valid;
}


/**
 * @brief The last error code that occurred.
 *
 * This property holds the last error that occurred in the database.
 *
 * @sa Errors
 */
int Database::lastError() const
{
    const Q_D(Database);
    return d->lastError;
}


/**
 * @brief A description of the last error that occurred.
 *
 * This property holds a textual description of the last error that has
 * occurred.
 */
QString Database::lastErrorString() const
{
    const Q_D(Database);
    return d->lastErrorString;
}


/**
 * @brief Clear the last error.
 *
 * Calling this resets the lastError() and lastErrorString() properties.
 */
void Database::clearLastError()
{
    Q_D(Database);
    d->lastError = Errors::NoError;
    d->lastErrorString.clear();
}

} // namespace Core
} // namespace QLMDB
