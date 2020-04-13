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
#ifndef CURSOR_H
#define CURSOR_H

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

    class FindResultPrivate;

    /**
     * @brief Represents a data item retrieved via the cursor.
     */
    class QLMDBSHARED_EXPORT FindResult {
    public:
        FindResult();
        explicit FindResult(const QByteArray &key, const QByteArray &value);
        FindResult(const FindResult &other);
        virtual ~FindResult();
        FindResult& operator =(const FindResult &other);
        bool operator ==(const FindResult &other) const;
        bool operator !=(const FindResult &other) const;
        QByteArray key() const;
        QByteArray value() const;
        bool isValid() const;

    private:

        QScopedPointer<FindResultPrivate> d_ptr;
        Q_DECLARE_PRIVATE(FindResult)
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

} // namespace QLMDB

#endif // CURSOR_H
