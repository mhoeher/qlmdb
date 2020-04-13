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
#include <QObject>

#include "contextprivate.h"


namespace QLMDB {

ContextPrivate::ContextPrivate() :
    env(nullptr),
    lastError(0),
    lastErrorString(),
    path(),
    flags(0),
    mode(0644),
    maxDBs(0),
    maxReaders(0),
    mapSize(0),
    open(false)
{
    lastError = mdb_env_create(&env);
    if (lastError != 0) {
        lastErrorString = QObject::tr("Failed to create environment");
    }
}

ContextPrivate::~ContextPrivate()
{
    if (env != nullptr) {
        mdb_env_close(env);
    }
}

} // namespace QLMDB
