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
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QScopedPointer>
#include <QtGlobal>

#include "qlmdb_global.h"

namespace QLMDB {

class Context;
class Cuesor;
class Database;
class TransactionPrivate;
class DatabasePrivate;

class QLMDBSHARED_EXPORT Transaction
{
    friend class Cursor;
    friend class Database;
    friend class DatabasePrivate;
public:
    static const unsigned int ReadOnly;

    explicit Transaction(Context &context, unsigned int flags = 0);
    explicit Transaction(Transaction &parent, unsigned int flags = 0);
    virtual ~Transaction();

    Transaction& operator =(const Transaction &other) = delete;

    bool isValid() const;
    int lastError() const;
    QString lastErrorString() const;

    bool commit();
    bool abort();


private:
    QScopedPointer<TransactionPrivate> d_ptr;

    Q_DECLARE_PRIVATE(Transaction)

};

} // namespace QLMDB

#endif // TRANSACTION_H
