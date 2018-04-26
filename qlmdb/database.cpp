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
#include <QFileInfo>

#include "database.h"
#include "databaseprivate.h"


/**
 * @brief The QLMDB namespace.
 *
 * This namespace contains all of the symbols belonging to the QLMDN library.
 */
namespace QLMDB {


/**
 * @brief The default number of (named) tables to support.
 */
const int Database::DefaultMaxTables;


/**
 * @brief The default maximum database size.
 *
 * This value is the default maximum size of a database in Bytes.
 */
const size_t Database::DefaultMaxDatabaseSize;


/**
 * @brief Constructor.
 *
 * Creates a new database. Initially, a database is closed and must be
 * opened first to read or write data.
 */
Database::Database() :
    d_ptr(new DatabasePrivate)
{
    Q_D(Database);
    d->q_ptr = this;
}


/**
 * @brief Copy constructor.
 *
 * Creates a Database from another one. Note that the resulting Database
 * object will share the same state as @p other one we copy from -
 * e.g. closing one will also close the other.
 */
Database::Database(const Database &other) :
    d_ptr(other.d_ptr)
{
}


/**
 * @brief Destructor.
 */
Database::~Database()
{
}


/**
 * @brief Assignment operator.
 *
 * Copy the @p other database onto this one. Note that afterwards both will
 * share the same underlying state, i.e. closing one will also close
 * the other.
 */
Database &Database::operator =(const Database &other)
{
    d_ptr = other.d_ptr;
    return *this;
}


/**
 * @brief The directory the database is stored in.
 *
 * The directory property holds the path to the directory which holds the
 * actual data of the database. This property must be set to a valid
 * directory path before opening a database:
 *
 * @code
 * Database db;
 * db.setDirectory("./my_db/");
 * if (db.open()) {
 *    // Read or write data.
 * }
 * @endcode
 *
 * @note If the given directory does not yet create, it will be created when
 *       opening the database.
 */
QString Database::directory() const
{
    const Q_D(Database);
    return d->directory;
}


/**
 * @brief Set the @p directory holding the database data.
 */
void Database::setDirectory(const QString &directory)
{
    Q_D(Database);
    d->directory = directory;
}


/**
 * @brief Indicates if the database is open.
 *
 * This property indicates if the database is open. It is true when
 * the database is open and false otherwise.
 */
bool Database::isOpen() const
{
    const Q_D(Database);
    return d->environment != nullptr;
}


/**
 * @brief The maximum number of tables to support within a database.
 *
 * This property holds the maximum number of (named) tables within one
 * database that is supported. The default value is Database::DefaultMaxTables.
 * To use another value, this property must be set before calling open().
 */
int Database::maxTables() const
{
    const Q_D(Database);
    return d->maxTables;
}


/**
 * @brief Set the maximum number of tables to @p maxTables.
 */
void Database::setMaxTables(int maxTables)
{
    Q_D(Database);
    if (isOpen()) {
        d->lastErrorString = "Cannot change Database::maxTables() of open "
                             "database";
    } else if (maxTables < 0) {
        d->lastErrorString = "Database::maxTables() must be creater or equal "
                             "to 0";
    } else {
        d->maxTables = maxTables;
    }
}


/**
 * @brief The maximum database size in bytes.
 *
 * This property holds the maximum database size in bytes. The default
 * value is Database::DefaultMaxDatabaseSize.
 *
 * This property must be set before calling open() to open the database.
 * Note that the set space will not be occupied immediately. Instead, the
 * database will grow over time until it reaches the given maximum
 * value.
 *
 * Note that the size can be increased, which requires the current Database
 * to be closed and a new instance to be opened with the increased size being
 * set before calling open().
 */
size_t Database::maxDatabaseSize() const
{
    const Q_D(Database);
    return d->maxDatabaseSize;
}


/**
 * @brief Set the maximum database @p size in bytes.
 */
void Database::setMaxDatabaseSize(size_t size)
{
    Q_D(Database);
    if (isOpen()) {
        d->lastErrorString = "Cannot change Database::maxDatabaseSize() of "
                             "open database";
    } else {
        d->maxDatabaseSize = size;
    }
}


/**
 * @brief Open a database.
 * @return
 */
bool Database::open()
{
    Q_D(Database);
    auto directory = d->directory;
    if (directory.isEmpty()) {
        d->lastErrorString = "Database::directory() must not be empty";
        return false;
    } else {
        QFileInfo fi(directory);
        if (!fi.exists()) {
            QDir dir(directory);
            if (!dir.mkpath(".")) {
                d->lastErrorString = "Failed to create directory";
                return false;
            }
        } else if (!fi.isDir()) {
            d->lastErrorString = "Database::directory() must point to a "
                                 "directory";
            return false;
        }
        return d->createEnvironment();
    }
}


/**
 * @brief Open a database.
 *
 * This is an overloaded version of open(). It first sets the @p directory
 * property before calling the open() method.
 */
bool Database::open(const QString &directory)
{
    setDirectory(directory);
    return open();
}


/**
 * @brief Indicates if there was an error in the database.
 *
 * This method returns true if an error occurred in the database.
 *
 * @sa lastErrorString(), clearError()
 */
bool Database::hasError() const
{
    const Q_D(Database);
    return !d->lastErrorString.isEmpty();
}


/**
 * @brief Get the last error string.
 *
 * This method returns the message of the last error that occurred.
 */
QString Database::lastErrorString() const
{
    const Q_D(Database);
    return d->lastErrorString;
}


/**
 * @brief Clear errors.
 *
 * This method can be called to clear the last error that occurred. Afterwards,
 * the hasError() method will return true.
 */
void Database::clearError()
{
    Q_D(Database);
    d->lastErrorString.clear();
}


/**
 * @brief Get a Collection.
 *
 * Returns the connection object called @p name. A "null" QByteArray can
 * be passed in to get the default collection.
 *
 * Collections are key-value stores
 * within the database (and hence resemble what a table or relation would
 * be in a relational data base system).
 *
 * The @p mode parameter can be used to fine tune how the collection
 * works.
 */
Collection Database::collection(const QByteArray &name,
                                Collection::OpenMode mode)
{
    Collection c;
    c.setDatabase(d_ptr);
    c.setName(name);
    c.setOpenMode(mode);
    return c;
}

} // namespace QLMDB
