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
#ifndef COLLECTION_H
#define COLLECTION_H

#include <QByteArray>
#include <QSharedPointer>

#include "qlmdb_global.h"

namespace QLMDB {

class CollectionPrivate;
class Database;
class DatabasePrivate;


/**
 * @brief Access a collection within a LMDB database.
 *
 * The collection class can be used to access the data within one
 * collection of an LMDB database. LMDB stores data as key-value pairs,
 * hence, a collection is basically what a table is in relational
 * database systems.
 *
 * Access to a single collection can be read-write or read-only.
 * In addition, a collection can be "single valued" (i.e. for a particular
 * key, there exists at most one value) or "multi valued" (i.e.
 * for a particular key, there can be arbitrary many values).
 *
 * Both keys and values are byte arrays, however, there are convenience
 * methods that allow to store QVariant and JSON data directly.
 */
class QLMDBSHARED_EXPORT Collection
{
    friend class Database;
public:
    explicit Collection();
    Collection(const Collection &other);
    Collection& operator =(const Collection &other);

    QByteArray name() const;
    bool isValid() const;

private:
    QSharedPointer<CollectionPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Collection)

    void setValid(bool valid);
    void setName(const QByteArray &name);
    void setDatabase(QSharedPointer<DatabasePrivate> database);

};

} // namespace QLMDB

#endif // COLLECTION_H
