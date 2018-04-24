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
#include "collection.h"
#include "collectionprivate.h"

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
    return !d->database.toStrongRef().isNull();
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


} // namespace QLMDB
