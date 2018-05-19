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
#include "liblmdb/lmdb.h"

#include "collection.h"
#include "collectionprivate.h"

#include <QDebug>

namespace QLMDB {


/**
 * @brief Constructor.
 *
 * Creates a new, invalid Collection object. The resulting object
 * will not be valid (and cannot be made valid except by assigning
 * it from another valid collection.
 *
 * @sa isValid()
 */
Collection::Collection() :
    d_ptr(new CollectionPrivate)
{

}


/**
 * @brief Create a collection from the @p other one.
 *
 * Note that both collection objects will share the same state.
 */
Collection::Collection(const Collection &other) :
    d_ptr(other.d_ptr)
{

}


/**
 * @brief Assign the @p other Collection to this one.
 *
 * Note that both collection objects will share the same state.
 */
Collection &Collection::operator =(const Collection &other)
{
    d_ptr = other.d_ptr;
    return *this;
}


/**
 * @brief The name of the collection.
 *
 * This property holds the name of the collection which is
 * accessed.
 */
QByteArray Collection::name() const
{
    const Q_D(Collection);
    return d->name;
}


/**
 * @brief Indicates if the Collection is valid.
 *
 * The valid property indicates whether or not the Collection is valid.
 * Only a valid collection can be used to actually read and write
 * data to or from a database.
 */
bool Collection::isValid() const
{
    const Q_D(Collection);
    return !d->database.isNull();
}


/**
 * @brief Insert the @p value using the given @p key into the collection.
 *
 * This method behaves slightly differently depending on the mode the
 * collection has been opened with. By default, there is at most
 * one value stored per key. Calling this function twice using the same
 * key hence overrides the previously stored value:
 *
 * ```
 * collection.insert("foo", "test"); // "foo" now has the value "test"
 * collection.insert("foo", "changed"); // "foo" now has the value "changed"
 * ```
 *
 * If the Collection has been opened using the Collection::MultiValues
 * option, each insert will add the value to the key:
 *
 * ```
 * collection.insert("foo", "1st"); // "foo" is now {"1st"}
 * collection.insert("foo", "2nd"); // "foo" is now {"1st", "2nd"}
 * ```
 */
bool Collection::put(QByteArray key, QByteArray value)
{
    bool result = false;
    Q_D(Collection);
    if (!d->database.isNull()) {
        MDB_txn *txn = nullptr;
        auto ret = mdb_txn_begin(
                    d->database->environment,
                    nullptr,
                    0,
                    &txn);
        if (ret == 0) {
            MDB_dbi dbi;
            const char *name = nullptr;
            if (!d->name.isNull()) {
                name = d->name.constData();
            }
            unsigned int flags = MDB_CREATE;
            if (d->openMode.testFlag(MultiValues)) {
                flags |= MDB_DUPSORT;
            }
            ret = mdb_dbi_open(txn, name, flags, &dbi);
            if (ret == 0) {
                MDB_val k, v;
                k.mv_size = static_cast<size_t>(key.size());
                k.mv_data = key.data();
                v.mv_size = static_cast<size_t>(value.size());
                v.mv_data = value.data();
                ret = mdb_put(txn, dbi, &k, &v, 0);
                if (ret == 0) {
                    result = true;
                }
            }
            if (result) {
                mdb_txn_commit(txn);
            } else {
                mdb_txn_abort(txn);
            }
        }
    }
    return result;
}


/**
 * @brief Read a value from the collection.
 *
 * This will read the value registered under the given @p key from the
 * collection and return it. If there is no such key, then
 * @p defaultValue is returned. This can be used to check if the key
 * has been found or not (in the latter case, a null QByteArray is
 * returned by default).
 *
 * Note that if the Collection has been opened with the
 * Collection::MultiValues option, there might be additional
 * values available. In this case, use the getAll() method to retrieve
 * all of them in one go.
 */
QByteArray Collection::get(QByteArray key, const QByteArray &defaultValue)
{
    QByteArray result = defaultValue;
    bool ok = false;
    Q_D(Collection);
    if (!d->database.isNull()) {
        MDB_txn *txn = nullptr;
        auto ret = mdb_txn_begin(
                    d->database->environment,
                    nullptr,
                    MDB_RDONLY,
                    &txn);
        if (ret == 0) {
            MDB_dbi dbi;
            const char *name = nullptr;
            if (!d->name.isNull()) {
                name = d->name.constData();
            }
            unsigned int flags = MDB_CREATE;
            if (d->openMode.testFlag(MultiValues)) {
                flags |= MDB_DUPSORT;
            }
            ret = mdb_dbi_open(txn, name, flags, &dbi);
            if (ret == 0) {
                MDB_val k, v;
                k.mv_size = static_cast<size_t>(key.size());
                k.mv_data = key.data();
                ret = mdb_get(txn, dbi, &k, &v);
                if (ret == 0) {
                    result = QByteArray(
                                reinterpret_cast<const char*>(v.mv_data),
                                static_cast<int>(v.mv_size));
                    ok = true;
                }
            }
            if (ok) {
                mdb_txn_commit(txn);
            } else {
                mdb_txn_abort(txn);
            }
        }
    }
    return result;
}


/**
 * @brief Get all values belonging to the given @p key.
 *
 * This method returns all values belonging to the key. By default,
 * rather use get() instead. This method is only usefule if the
 * collection has been opened with the Collection::MultiValues option.
 */
QByteArrayList Collection::getAll(QByteArray key)
{
    QByteArrayList result;
    bool ok = false;
    Q_D(Collection);
    if (!d->database.isNull()) {
        MDB_txn *txn = nullptr;
        auto ret = mdb_txn_begin(
                    d->database->environment,
                    nullptr,
                    MDB_RDONLY,
                    &txn);
        if (ret == 0) {
            MDB_dbi dbi;
            const char *name = nullptr;
            if (!d->name.isNull()) {
                name = d->name.constData();
            }
            unsigned int flags = MDB_CREATE;
            if (d->openMode.testFlag(MultiValues)) {
                flags |= MDB_DUPSORT;
            }
            ret = mdb_dbi_open(txn, name, flags, &dbi);
            if (ret == 0) {
                MDB_cursor *cursor;
                ret = mdb_cursor_open(txn, dbi, &cursor);
                if (ret == 0) {
                    ok = true;
                    MDB_val k, v;
                    k.mv_size = static_cast<size_t>(key.size());
                    k.mv_data = key.data();
                    ret = mdb_cursor_get(
                                cursor, &k, &v, MDB_SET_KEY);
                    while (ret == 0) {
                        QByteArray value(static_cast<const char*>(v.mv_data),
                                         static_cast<int>(v.mv_size));
                        result << value;
                        ret = mdb_cursor_get(cursor, &k, &v, MDB_NEXT_DUP);
                    }
                    mdb_cursor_close(cursor);
                }
            }
            if (ok) {
                mdb_txn_commit(txn);
            } else {
                mdb_txn_abort(txn);
            }
        }
    }
    return result;
}

void Collection::setName(const QByteArray &name)
{
    Q_D(Collection);
    d->name = name;
}

void Collection::setDatabase(QSharedPointer<DatabasePrivate> database)
{
    Q_D(Collection);
    d->database = database;
}

void Collection::setOpenMode(OpenMode openMode)
{
    Q_D(Collection);
    d->openMode = openMode;
}


} // namespace QLMDB
