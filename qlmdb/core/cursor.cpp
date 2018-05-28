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

#include "cursor.h"
#include "cursorprivate.h"
#include "database.h"
#include "databaseprivate.h"
#include "errors.h"
#include "transaction.h"
#include "transactionprivate.h"


namespace QLMDB {
namespace Core {


/**
 * @class Cursor
 * @brief Store, retrieve and delete data.
 *
 * Cursor objects are created within a Transaction and are used on one
 * Database. They can be used to efficiently write data to, read data from
 * and delete data from their connected database.
 */


/**
 * @brief Replace the item at the current cursor position.
 *
 * This flag is used to repace the item the cursor currently points to.
 * The key specified must match the one the cursor points to. For
 * databases supporting duplicate entries per key, the data must sort
 * into the same position, otherwise, a delete followed by an insert is
 * done.
 */
const unsigned int Cursor::ReplaceCurrent = MDB_CURRENT;

/**
 * @brief Insert the key/value pair only if it is not already present.
 *
 * This flag may only be used with databases supporting multiple
 * values per key. It causes the key/value pair to be inserted only if
 * there is not already such a pair in the database.
 */
const unsigned int Cursor::NoDuplicateData = MDB_NODUPDATA;

/**
 * @brief EInsert the key/value pair only if the key is not already present.
 *
 * If this flag is set, the new key/value pair is only inserted if the key
 * is not yes present in the database. This applies to databases both with
 * and without multiple values per key.
 */
const unsigned int Cursor::NoOverrideKey = MDB_NOOVERWRITE;

/**
 * @brief Reserve space in the database.
 *
 * If specified, as much space as occupied by the value is reserved in
 * the database. This is useful if the actual data is generated later.
 * No copying of the data in the value to the database occurs.
 */
const unsigned int Cursor::Reserve = MDB_RESERVE;

/**
 * @brief Append key/value pairs to the end of the database.
 *
 * This flag can be used to append data to the end of a database with
 * single values per key.
 * If you try to insert a key which is not larger than the one at the end,
 * an error occurs.
 */
const unsigned int Cursor::Append = MDB_APPEND;

/**
 * @brief Append key/value pairs to the end of the database.
 *
 * This flag can be used to append data to the end of a database with
 * multiple values per key.
 * If you try to insert a key/value pair which is not larger than the one at
 * the end, an error occurs.
 */
const unsigned int Cursor::AppendDuplicate = MDB_APPENDDUP;


/**
 * @brief Constructor.
 *
 * To create a cursor, a @p transaction and a @p database are required. The
 * cursor will operator on the given database within the transaction and
 * can be used to store, retrieve and delete records from the database.
 *
 * The database must be open and the transaction valid. Use the isValid()
 * property to check if the cursor is valid after creating and refer
 * to lastError() and lastErrorString() to find out why creating the cursor
 * may have failed.
 */
Cursor::Cursor(Transaction &transaction, Database &database) :
    d_ptr(new CursorPrivate)
{
    Q_D(Cursor);
    if (transaction.isValid() && database.isValid()) {
        d->lastError = mdb_cursor_open(transaction.d_ptr->txn,
                                       database.d_ptr->db,
                                       &d->cursor);
        if (d->lastError == 0) {
            d->valid = true;
        } else if (d->lastError == Errors::InvalidParameter) {
            d->lastErrorString = QObject::tr("Invalid parameters encountered "
                                             "when creating Cursor");
        } else {
            d->lastErrorString = QObject::tr("Unexpected error creating "
                                             "Cursor");
        }
    } else {
        d->lastError = Errors::InvalidParameter;
        d->lastErrorString = QObject::tr("Creating a Cursor requires a valid "
                                         "Transaction and Database");
    }
}


/**
 * @brief Destructor.
 */
Cursor::~Cursor()
{
    Q_D(Cursor);
    if (d->valid) {
        mdb_cursor_close(d->cursor);
    }
}


/**
 * @brief Indicate if the cursor is valid.
 *
 * This property is true if the cursor is valid or false if some error occurred
 * when creating the cursor.
 */
bool Cursor::isValid() const
{
    const Q_D(Cursor);
    return d->valid;
}


/**
 * @brief The last error that occurred.
 *
 * @sa Errors
 */
int Cursor::lastError() const
{
    const Q_D(Cursor);
    return d->lastError;
}


/**
 * @brief A textual description of the last error that occurred.
 */
QString Cursor::lastErrorString() const
{
    const Q_D(Cursor);
    return d->lastErrorString;
}


/**
 * @brief Clear the last error that occurred.
 */
void Cursor::clearLastError()
{
    Q_D(Cursor);
    d->lastError = Errors::NoError;
    d->lastErrorString.clear();
}

} // namespace Core
} // namespace QLMDB
