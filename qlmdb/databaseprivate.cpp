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
#include <QDir>

#include "database.h"
#include "databaseprivate.h"

namespace QLMDB {

DatabasePrivate::DatabasePrivate() :
    q_ptr(nullptr),
    directory(),
    lastErrorString(),
    environment(nullptr),
    maxTables(Database::DefaultMaxTables),
    maxDatabaseSize(Database::DefaultMaxDatabaseSize)
{

}

DatabasePrivate::~DatabasePrivate()
{
    if (environment != nullptr) {
        mdb_env_close(environment);
    }
}

bool DatabasePrivate::createEnvironment()
{
    auto ret = mdb_env_create(&environment);
    if (ret != 0) {
        lastErrorString = "Failed to create environment";
        return false;
    }
    ret = mdb_env_set_maxdbs(environment, static_cast<MDB_dbi>(maxTables));
    if (ret != 0) {
        lastErrorString = "Failed to set number of maximum named tables";
        mdb_env_close(environment);
        return false;
    }
    ret = mdb_env_set_mapsize(environment, maxDatabaseSize);
    if (ret != 0) {
        lastErrorString = "Failed to set maximum database size";
        mdb_env_close(environment);
        return false;
    }
    ret = mdb_env_open(
                environment,
                QDir::toNativeSeparators(
                    directory).toStdString().c_str(),
                0,
                0644);
    if (ret != 0) {
        lastErrorString = "Failed to open environment";
        mdb_env_close(environment);
        return false;
    }
    return true;
}

} // namespace QLMDB
