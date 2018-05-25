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
#ifndef DATABASE_H
#define DATABASE_H

#include <QScopedPointer>
#include <QString>

namespace QLMDB {
namespace Core {

class DatabasePrivate;
class Context;
class Transaction;

class Database
{
public:
    static const unsigned int ReverseKey;
    static const unsigned int MultiValues;
    static const unsigned int IntegerKeys;
    static const unsigned int FixedSizeMultiValues;
    static const unsigned int IntegerKeysMultiValues;
    static const unsigned int ReverseKeyMultiValues;
    static const unsigned int Create;

    Database(Context &context,
             const QString &name = QString(),
             unsigned int flags = Create);
    Database(Transaction &transaction,
             const QString &name = QString(),
             unsigned int flags = Create);
    virtual ~Database();

    bool isValid() const;
    int lastError() const;
    QString lastErrorString() const;
    void clearLastError();


private:

    QScopedPointer<DatabasePrivate> d_ptr;
    Q_DECLARE_PRIVATE(Database)

};

} // namespace Core
} // namespace QLMDB

#endif // DATABASE_H
