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
#ifndef CURSOR_H
#define CURSOR_H

#include <optional>

#include <QByteArray>
#include <QPair>
#include <QString>
#include <QScopedPointer>

#include "qlmdb_global.h"

namespace QLMDB {

class Transaction;
class Database;
class CursorPrivate;

class QLMDBSHARED_EXPORT Cursor
{
public:
    // Flags for data insertion:
    static const unsigned int ReplaceCurrent;
    static const unsigned int NoDuplicateData;
    static const unsigned int NoOverrideKey;
    static const unsigned int Reserve;
    static const unsigned int Append;
    static const unsigned int AppendDuplicate;

    // Flags for data deletion:
    static const unsigned int RemoveAll;


    /**
     * @brief Represents a data item retrieved via the cursor.
     */
    struct FindResult {
        /**
         * @brief The key of the result or a null byte array for invalid ones.
         */
        QByteArray key;

        /**
         * @brief The value of the result or a null byte array for invalid ones.
         */
        QByteArray value;

        /**
         * @brief True if the result is valid or false otherwise.
         */
        bool valid;

        inline FindResult();
        inline FindResult(const QByteArray &key, const QByteArray &value);
        inline bool operator ==(const FindResult &other) const;
        inline bool operator !=(const FindResult &other) const;
    };

    explicit Cursor(Transaction &transaction, Database &database);
    virtual ~Cursor();

    bool isValid() const;
    int lastError() const;
    QString lastErrorString() const;
    void clearLastError();

    bool put(const QByteArray &key, const QByteArray &data,
             unsigned int flags = 0);
    QByteArray currentKey();
    QByteArray currentValue();
    FindResult current();
    FindResult first();
    FindResult last();
    FindResult firstForCurrentKey();
    FindResult lastForCurrentKey();
    FindResult find(QByteArray key, QByteArray value);
    FindResult findNearest(QByteArray key, QByteArray value);
    FindResult findKey(QByteArray key);
    FindResult findFirstAfter(QByteArray key);
    FindResult next();
    FindResult nextForCurrentKey();
    FindResult nextKey();
    FindResult previous();
    FindResult previousForCurrentKey();
    FindResult previousKey();
    bool remove(unsigned int flags = 0);

private:
    QScopedPointer<CursorPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Cursor)
};


/**
 * @brief Constructs an invalid FindResult.
 */
Cursor::FindResult::FindResult() :
    key(),
    value(),
    valid(false)
{
}


/**
 * @brief Constructs a FindResult from the given @p key and @p value.
 */
Cursor::FindResult::FindResult(const QByteArray &key, const QByteArray &value) :
    key(key),
    value(value),
    valid(true)
{
}


/**
 * @brief Check if this FindResult is equal to the @p other one.
 */
bool Cursor::FindResult::operator ==(const Cursor::FindResult &other) const
{
    return valid == other.valid &&
            key == other.key &&
            value == other.value;
}


/**
 * @brief Check if this FindResult is unequal to the @p other one.
 */
bool Cursor::FindResult::operator !=(const Cursor::FindResult &other) const
{
    return !(*this == other);
}

char *toString(const Cursor::FindResult &result);

} // namespace QLMDB

#endif // CURSOR_H
