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
#ifndef DATABASEPRIVATE_H
#define DATABASEPRIVATE_H

#include "liblmdb/lmdb.h"

#include <QString>

namespace QLMDB {

class Database;

class DatabasePrivate
{
public:
    Database* q_ptr;

    QString directory;
    QString lastErrorString;

    MDB_env *environment;

    int maxTables;

    explicit DatabasePrivate();
    virtual ~DatabasePrivate();
    bool createEnvironment();

};

} // namespace QLMDB

#endif // DATABASEPRIVATE_H
