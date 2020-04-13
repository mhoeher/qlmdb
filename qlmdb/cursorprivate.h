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
#ifndef CURSORPRIVATE_H
#define CURSORPRIVATE_H

#include <lmdb.h>

#include <QObject>
#include <QString>

#include "cursor.h"
#include "errors.h"


namespace QLMDB {

//! @private
class CursorPrivate
{
public:
    CursorPrivate();

    MDB_cursor *cursor;
    int lastError;
    QString lastErrorString;
    bool valid;

    inline Cursor::FindResult get(
            MDB_val &key, MDB_val &value, MDB_cursor_op op);
};


/**
 * @private
 * @brief Construct a QByteArray from an MDB_val.
 *
 * This function returns a QByteArray which holds the data given in the
 * @p val. Note that the caller must ensure that the MDB_val remains valid
 * for as long as the returned QByteArray is used.
 */
inline QByteArray value_to_bytearray(const MDB_val &val)
{
    return QByteArray::fromRawData(
                static_cast<const char*>(val.mv_data),
                static_cast<int>(val.mv_size));
}


/**
 * @private
 * @brief Convert a byte array to a MDB_val.
 *
 * This initializes the fields of an MDB_val from the given @p byteArray.
 * Note that the caller must ensure that the byte array remains valid for as
 * long as the returned value is in use.
 *
 * In particular, note that this function avoids a copy-on-write on the passed
 * in byte array for performance reasons.
 */
inline MDB_val bytearray_to_value(const QByteArray &byteArray)
{
    MDB_val result;
    // Avoid a deep copy by using constData().
    result.mv_data = const_cast<char*>(byteArray.constData());
    result.mv_size = static_cast<size_t>(byteArray.size());
    return result;
}


/**
 * @brief Retrieve data via the cursor.
 */
Cursor::FindResult CursorPrivate::get(MDB_val &key, MDB_val &value,
                                      MDB_cursor_op op)
{
    Cursor::FindResult result;
    if (valid) {
        lastError = mdb_cursor_get(cursor, &key, &value, op);
        if (lastError == Errors::NoError) {
            lastErrorString.clear();
            result = Cursor::FindResult(
                        value_to_bytearray(key),
                        value_to_bytearray(value));
        } else if (lastError == Errors::NotFound) {
            lastErrorString = QObject::tr("Unable to find key in the database");
        } else if (lastError == Errors::InvalidParameter) {
            lastErrorString = QObject::tr("Invalid parameter passed to "
                                          "cursor get operation");
        }
    }
    return result;
}

} // namespace QLMDB

#endif // CURSORPRIVATE_H
