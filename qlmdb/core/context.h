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
#ifndef CONTEXT_H
#define CONTEXT_H

#include <QtGlobal>
#include <QScopedPointer>

#include "qlmdb_global.h"


namespace QLMDB {
namespace Core {

class ContextPrivate;
class Transaction;
class Database;

class QLMDBSHARED_EXPORT Context
{
    friend class Transaction;
    friend class Database;
    friend class DatabasePrivate;
public:
    static const unsigned int FixedMap;
    static const unsigned int NoSubDir;
    static const unsigned int ReadOnly;
    static const unsigned int WriteMap;
    static const unsigned int NoMetaSync;
    static const unsigned int NoSync;
    static const unsigned int MapAsync;
    static const unsigned int NoTLS;
    static const unsigned int NoLock;
    static const unsigned int NoReadAhead;
    static const unsigned int NoMemInit;

    Context();
    virtual ~Context();

    int lastError() const;
    QString lastErrorString() const;
    void clearLastError();

    QString path() const;
    void setPath(const QString &path);

    unsigned int flags() const;
    void setFlags(unsigned int flags);

    unsigned int mode() const;
    void setMode(unsigned int mode);

    unsigned int maxDBs() const;
    void setMaxDBs(unsigned int maxDBs);

    unsigned int maxReaders() const;
    void setMaxReaders(unsigned int maxReaders);

    size_t mapSize() const;
    void setMapSize(size_t mapSize);

    bool isOpen() const;
    bool open();

private:

    QScopedPointer<ContextPrivate> d_ptr;

    Q_DECLARE_PRIVATE(Context)
};

} // namespace Core
} // namespace QLMDB

#endif // CONTEXT_H
