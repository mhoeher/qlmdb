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
#ifndef QLMDB_DATABASE_H
#define QLMDB_DATABASE_H

#include <QSharedPointer>

#include "qlmdb_global.h"
#include "collection.h"

namespace QLMDB {

class DatabasePrivate;

/**
 * @brief A Database.
 *
 * A Database represents a directory on disk, which contains the actual
 * data. Several processes can access such a database in parallel. Within
 * one process, a database must however be opened at most once (however,
 * different databases can be opened independently).
 *
 * Within one database, several tables can be created (which are called
 * DBs in LMDB). Each database contains a key value list.
 */
class QLMDBSHARED_EXPORT Database
{
public:
    static const int DefaultMaxTables = 100;
    static const size_t DefaultMaxDatabaseSize = 1073741824;

    explicit Database();
    Database(const Database &other);
    virtual ~Database();

    Database& operator =(const Database &other);

    QString directory() const;
    void setDirectory(const QString& directory);

    bool isOpen() const;

    int maxTables() const;
    void setMaxTables(int maxTables);

    size_t maxDatabaseSize() const;
    void setMaxDatabaseSize(size_t size);

    bool open();
    bool open(const QString& directory);
    bool hasError() const;
    QString lastErrorString() const;
    void clearError();

    Collection collection(const QByteArray &name,
                          Collection::OpenMode mode = Collection::OpenMode());

private:
    QSharedPointer<DatabasePrivate> d_ptr;
    Q_DECLARE_PRIVATE(Database)
};


} // namespace QLMDB
#endif // QLMDB_DATABASE_H
