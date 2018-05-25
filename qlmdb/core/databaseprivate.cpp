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
#include "databaseprivate.h"
#include "errors.h"
#include "transaction.h"
#include "transactionprivate.h"

namespace QLMDB {
namespace Core {

DatabasePrivate::DatabasePrivate() :
    env(nullptr),
    db(),
    lastError(Errors::NoError),
    lastErrorString(),
    valid(false)
{

}

void DatabasePrivate::initFromContext(Context &context, Transaction *txn,
                                      const QString &name, unsigned int flags)
{
    auto stdName = name.toUtf8();
    const char *dbName = nullptr;
    if (!name.isEmpty()) {
        dbName = stdName.constData();
    }
    if (context.isOpen()) {
        {
            if (txn != nullptr) {
                if (txn->isValid()) {
                    lastError = mdb_dbi_open(
                                txn->d_ptr->txn, dbName, flags, &db);
                }
            } else {
                Transaction tmpTxn(context);
                lastError = mdb_dbi_open(
                            tmpTxn.d_ptr->txn, dbName, flags, &db);
            }
            valid = evaluateCreateError(name);
            env = context.d_ptr->env;
        }
    }
}

bool DatabasePrivate::evaluateCreateError(const QString &name)
{
    bool result = false;
    if (lastError == Errors::NoError) {
        lastErrorString.clear();
        result = true;
    } else if (lastError == Errors::NotFound) {
        lastErrorString = QObject::tr("No such database: '%1'").arg(name);
    } else if (lastError == Errors::ReadersFull) {
        lastErrorString = QObject::tr("Maximum number of readers reached");
    }
    return result;
}

} // namespace Core
} // namespace QLMDB
