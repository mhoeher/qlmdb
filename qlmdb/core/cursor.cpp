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
 * @brief Remove all key/value pairs of the current key.
 *
 * If this flag is set, all values stored under the current key are removed.
 * Otherwise, only the current key/value pair is.
 *
 * This must only be used with databases that allow multiple values per key.
 */
const unsigned int Cursor::RemoveAll = MDB_NODUPDATA;


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


/**
 * @brief Store data in the database.
 *
 * Store the @p data under the @p key in the database. The way the data is
 * written can be influenced by specifying additional @p flags, which is either
 * zero or a bitwise OR-combination of the following values:
 *
 * - ReplaceCurrent
 * - NoDuplicateData
 * - NoOverrideKey
 * - Reserve
 * - Append
 * - AppendDuplicate
 *
 * If writing of the data was successful, the method returns true and the
 * cursor is positioned on the newly inserted key/value pair. On error,
 * the method returns false; in this case, the cursor is usually positioned
 * somewhere near the place the key/value pair would have been inserted.
 *
 * Note: When using the flags NoDuplicateData or NoOverrideKey, the operation
 * returns false and sets lastError() to Errors::KeyExists to indicate that
 * the data has not been inserted to avoid duplicates.
 */
bool Cursor::put(const QByteArray &key, const QByteArray &data,
                 unsigned int flags)
{
    Q_D(Cursor);
    bool result = false;
    if (isValid()) {
        MDB_val k;
        k.mv_data = const_cast<char*>(key.constData());
        k.mv_size = static_cast<size_t>(key.size());

        MDB_val v;
        v.mv_data = const_cast<char*>(data.constData());
        v.mv_size = static_cast<size_t>(data.size());

        d->lastError = mdb_cursor_put(d->cursor, &k, &v, flags);

        if (d->lastError == Errors::NoError) {
            d->lastErrorString.clear();
            result = true;
        } else if (d->lastError == Errors::MapFull) {
            d->lastErrorString = QObject::tr("No more space in database");
        } else if (d->lastError == Errors::TooManyTransactions) {
            d->lastErrorString = QObject::tr("Transaction has too many dirty "
                                             "pages");
        } else if (d->lastError == Errors::NoAccessToPath) {
            d->lastErrorString = QObject::tr("Cannot write in a readonly "
                                             "transaction");
        } else if (d->lastError == Errors::InvalidParameter) {
            d->lastErrorString = QObject::tr("Invalid parameters when trying "
                                             "to write via Cursor");
        } else if (d->lastError == Errors::KeyExists) {
            d->lastErrorString = QObject::tr("The specified key already exists "
                                             "in the database");
        } else {
            d->lastErrorString = QObject::tr("Unexpected error writing via "
                                             "Cursor");
        }
    }
    return result;
}


/**
 * @brief Get the current key the cursor is positioned on.
 *
 * This returns the key the cursor currently points to. Use one of
 * the methods to position the cursor first and the query the key
 * using this method.
 */
QByteArray Cursor::currentKey()
{
    QByteArray key;
    auto result = current();
    if (result.valid) {
        key = result.key;
    }
    return key;
}


/**
 * @brief Get the current value the cursor is positioned on.
 *
 * This returns the value the cursor currently points to. Use one of
 * the methods to position the cursor first and the query the value
 * using this method.
 */
QByteArray Cursor::currentValue()
{
    QByteArray value;
    auto result = current();
    if (result.valid) {
        value = result.value;
    }
    return value;
}


/**
 * @brief Get the current key/value pair.
 *
 * This returns the key/value pair the cursor currently points
 * to or an empty value if the cursor does not point anywhere.
 */
Cursor::FindResult Cursor::current()
{
    Q_D(Cursor);
    MDB_val key, value;
    return d->get(key, value, MDB_GET_CURRENT);

}


/**
 * @brief Get the first key/value pair in the database.
 */
Cursor::FindResult Cursor::first()
{
    Q_D(Cursor);
    MDB_val key, value;
    return d->get(key, value, MDB_FIRST);
}


/**
 * @brief Get the last key/value pair in the database.
 */
Cursor::FindResult Cursor::last()
{
    Q_D(Cursor);
    MDB_val key, value;
    return d->get(key, value, MDB_LAST);
}


/**
 * @brief Get the first key/value pair for the key the cursor points to.
 *
 * This positions the cursor at the first key for the keys the cursor
 * currently points to and returns the key/value pair there.
 *
 * @note This operation is only valid for databases opened with support for
 * multiple values per key.
 */
Cursor::FindResult Cursor::firstForCurrentKey()
{
    Q_D(Cursor);
    MDB_val key, value;
    // Note: MDB_FIRST_DUP does not update the key, hence we need to do
    // a "get current" if the operation itself succeeded.
    auto ret = d->get(key, value, MDB_FIRST_DUP);
    if (ret.valid) {
        return d->get(key, value, MDB_GET_CURRENT);
    }
    return FindResult();
}


/**
 * @brief Get the last key/value pair for the key the cursor points to.
 *
 * This positions the cursor at the last key for the keys the cursor
 * currently points to and returns the key/value pair there.
 *
 * @note This operation is only valid for databases opened with support for
 * multiple values per key.
 */
Cursor::FindResult Cursor::lastForCurrentKey()
{
    Q_D(Cursor);
    MDB_val key, value;
    // Note: MDB_LAST_DUP does not update the key, hence we need to do
    // a "get current" if the operation itself succeeded.
    auto ret = d->get(key, value, MDB_LAST_DUP);
    if (ret.valid) {
        return d->get(key, value, MDB_GET_CURRENT);
    }
    return FindResult();
}


/**
 * @brief Position the cursor at the given key/value pair.
 *
 * This positions the cursor at the @p key and @p value specified and return
 * it as a result.
 *
 * @note This operation is only valid for databases opened with support for
 * multiple values per key.
 */
Cursor::FindResult Cursor::find(QByteArray key, QByteArray value)
{
    Q_D(Cursor);
    MDB_val k = bytearray_to_value(key);
    MDB_val v = bytearray_to_value(value);
    return d->get(k, v, MDB_GET_BOTH);
}


/**
 * @brief Position the cursor at the given key/value pair or somewhere near.
 *
 * This positions the cursor at the given @p key and a value either equal to
 * the @p value or near to it.
 *
 * @note This operation is only valid for databases opened with support for
 * multiple values per key.
 */
Cursor::FindResult Cursor::findNearest(QByteArray key, QByteArray value)
{
    Q_D(Cursor);
    MDB_val k = bytearray_to_value(key);
    MDB_val v = bytearray_to_value(value);
    return d->get(k, v, MDB_GET_BOTH_RANGE);
}


/**
 * @brief Get the key/value pair for the given @p key.
 */
Cursor::FindResult Cursor::findKey(QByteArray key)
{
    Q_D(Cursor);
    MDB_val k = bytearray_to_value(key);
    MDB_val value;
    return d->get(k, value, MDB_SET_KEY);
}


/**
 * @brief Position the cursor on or next to a given key.
 *
 * This position the cursor either at the specified @p key or the
 * one next to it according to sorting.
 */
Cursor::FindResult Cursor::findFirstAfter(QByteArray key)
{
    Q_D(Cursor);
    MDB_val k = bytearray_to_value(key);
    MDB_val value;
    return d->get(k, value, MDB_SET_RANGE);
}


/**
 * @brief Get the next key/value pair.
 */
Cursor::FindResult Cursor::next()
{
    Q_D(Cursor);
    MDB_val key, value;
    return d->get(key, value, MDB_NEXT);
}


/**
 * @brief Position the cursor at the next key/value pair for the current key.
 *
 * @note This operation is only valid for databases opened with support for
 * multiple values per key.
 */
Cursor::FindResult Cursor::nextForCurrentKey()
{
    Q_D(Cursor);
    MDB_val key, value;
    return d->get(key, value, MDB_NEXT_DUP);
}


/**
 * @brief Position the cursor at the first key/value pair of the next key.
 *
 * @note This operation is only valid for databases opened with support for
 * multiple values per key.
 */
Cursor::FindResult Cursor::nextKey()
{
    Q_D(Cursor);
    MDB_val key, value;
    return d->get(key, value, MDB_NEXT_NODUP);
}


/**
 * @brief Get the previous key/value pair.
 */
Cursor::FindResult Cursor::previous()
{
    Q_D(Cursor);
    MDB_val key, value;
    return d->get(key, value, MDB_PREV);
}


/**
 * @brief Get the previous key/value pair for the current key.
 *
 * @note This operation is only valid for databases opened with support for
 * multiple values per key.
 */
Cursor::FindResult Cursor::previousForCurrentKey()
{
    Q_D(Cursor);
    MDB_val key, value;
    return d->get(key, value, MDB_PREV_DUP);
}


/**
 * @brief Position the cursor at the last element of the previous key.
 *
 * @note This operation is only valid for databases opened with support for
 * multiple values per key.
 */
Cursor::FindResult Cursor::previousKey()
{
    Q_D(Cursor);
    MDB_val key, value;
    return d->get(key, value, MDB_PREV_NODUP);
}


/**
 * @brief Remove data.
 *
 * Use this to delete data from the database operated on by the cursor. By
 * default, the current key/value pair is removed (hence, use one of the move
 * operations to position the cursor first).
 *
 * In addition, the @p flags can be set to a bitwise OR-combination of the
 * following to adjust the behaviour:
 *
 * - RemoveAll
 *
 * The method returns true if the deletion was successfull or false otherwise.
 */
bool Cursor::remove(unsigned int flags)
{
    Q_D(Cursor);
    bool result = false;
    if (d->valid) {
        d->lastError = mdb_cursor_del(d->cursor, flags);
        if (d->lastError == Errors::NoError) {
            d->lastErrorString.clear();
            result = true;
        } else if (d->lastError == Errors::NoAccessToPath) {
            d->lastErrorString = QObject::tr("Attempt to remove in readonly "
                                             "environment or transaction");
        } else if (d->lastError == Errors::InvalidParameter) {
            d->lastErrorString = QObject::tr("Invalid parameters passed to "
                                             "Cursor::remove()");
        } else {
            d->lastErrorString = QObject::tr("Unexpected error during "
                                             "Cursor::remove() operation");
        }
    }
    return result;
}


/**
 * @brief Helper function: Converts a Cursor::FindResult to a string.
 *
 * This function takes a find @p result and converts it to a (plain) string.
 * Note that the caller is assumed to take ownership of the returned data;
 * the returned string must be freed using `delete[]`.
 *
 * @note This is for testing purposes and usually should not be required to
 * be used.
 */
char *toString(const Cursor::FindResult &result)
{
    auto str = QString("QLMDB::Core::Cursor::FindResult("
                          "key='%1',value='%2',valid=%3");
    str = str.arg(
                QString(result.key.toPercentEncoding())).arg(
                QString(result.value.toPercentEncoding())).arg(
                result.valid);
    return qstrdup(str.toUtf8().data());
}

} // namespace Core
} // namespace QLMDB
