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

#include <QString>
#include <QScopedPointer>

#include "qlmdb_global.h"

namespace QLMDB {
namespace Core {

class Transaction;
class Database;
class CursorPrivate;

class QLMDBSHARED_EXPORT Cursor
{
public:
    static const unsigned int ReplaceCurrent;
    static const unsigned int NoDuplicateData;
    static const unsigned int NoOverrideKey;
    static const unsigned int Reserve;
    static const unsigned int Append;
    static const unsigned int AppendDuplicate;


    explicit Cursor(Transaction &transaction, Database &database);
    virtual ~Cursor();

    bool isValid() const;
    int lastError() const;
    QString lastErrorString() const;
    void clearLastError();

private:
    QScopedPointer<CursorPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Cursor)
};

} // namespace Core
} // namespace QLMDB

#endif // CURSOR_H
