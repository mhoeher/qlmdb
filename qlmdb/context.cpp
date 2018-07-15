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
#include "context.h"
#include "contextprivate.h"

namespace QLMDB {


/**
 * @defgroup QLMDB_Core_Context_OpenFlags Open Flags for a Context.
 * @{
 */

/**
 * @brief Use a fixed address for the memory map location.
 */
const unsigned int Context::FixedMap = MDB_FIXEDMAP;

/**
 * @brief Store the data in a single file instead of in a directory structure.
 */
const unsigned int Context::NoSubDir = MDB_NOSUBDIR;

/**
 * @brief Open the environment read-only.
 */
const unsigned int Context::ReadOnly = MDB_RDONLY;

/**
 * @brief Use a writeable memory map unless the ReadOnly flag is set.
 */
const unsigned int Context::WriteMap = MDB_WRITEMAP;

/**
 * @brief Flush system buffers to disk only once per transaction.
 */
const unsigned int Context::NoMetaSync = MDB_NOMETASYNC;

/**
 * @brief Don't flush system buffers to disk when committing a transaction.
 */
const unsigned int Context::NoSync = MDB_NOSYNC;

/**
 * @brief When using WriteMap, use asynchronous flushes to disk
 */
const unsigned int Context::MapAsync = MDB_MAPASYNC;

/**
 * @brief Don't use Thread-Local Storage.
 */
const unsigned int Context::NoTLS = MDB_NOTLS;

/**
 * @brief Don't do any locking.
 */
const unsigned int Context::NoLock = MDB_NOLOCK;

/**
 * @brief Turn off readahead.
 */
const unsigned int Context::NoReadAhead = MDB_NORDAHEAD;

/**
 * @brief Don't initialize malloc'd memory before writing to unused spaces in the data file.
 */
const unsigned int Context::NoMemInit = MDB_NOMEMINIT;

/** @} */


/**
 * @class QLMDB::Context
 * @brief A LMDB context.
 *
 * The Context class is a wrapper around LMDB's contexts. It acts as
 * RAII wrapper: Each instance of this class owns one LMDB context, which is
 * freed as soon as the object is destroyed.
 *
 * A single Context represents a kind of "database", which in turn can contain
 * several tables. The context is used to determine important attributes
 * of the database, like the maximum data size, if the database is opened in
 * read-only mode or if the database is represented as a single file on
 * disk or if it is a directory with several files in it.
 *
 * ## Creating a Context
 *
 * To create or open an LMDB database, create a Context object and pass
 * in the path you want to access:
 *
 * ```
 * Context ctx;
 * ctx.setPath("/path/to/lmdb-db");
 * if (ctx.open()) {
 *     // Context has been opened successfully, we can now
 *     // start to create transactions, databases and so on...
 * }
 * ```
 *
 * To tweak the behavior of the context, you can set several attributes between
 * constructing a new Context object and calling open(). In particular:
 *
 * ```
 * Context ctx;
 *
 * // Set the maximum size of the context in bytes:
 * ctx.setMapSize(1 * 1024 * 1024);
 *
 * // Set the maximum number of readers allowed in parallel:
 * ctx.setMaxReaders(10);
 *
 * // Set the maximum number of databases (key-value stores):
 * ctx.setMaxDBs(100);
 *
 * // Set the UNIX file permissions to set on created environment:
 * ctx.setMode(0600);
 *
 * // Change some further attributes by changing the flags with which the
 * // enviornment will be opened:
 * ctx.setFlags(Context::NoSubDir | Context::ReadOnly);
 *
 * // Now, open/create the environment:
 * if (ctx.open()) {
 *     // Read/write data...
 * }
 * ```
 *
 * ## Notes About Multi-Threading
 *
 * After a context is configured and successfully opened, it may be used from
 * multiple threads to create further classes like a Transaction. However,
 * note that you must not call any non-const member function of the context as
 * there is no locking to ensure write access is serialized.
 *
 * It is important to note, that a context (i.e. a path on disk) must
 * not be opened multiple times from within the same process. If you need
 * to access a context more than once, open it in one thread and then
 * pass around the Context object.
 *
 */

/**
 * @brief Constructor.
 */
Context::Context() :
    d_ptr(new ContextPrivate)
{
}


/**
 * @brief Destructor.
 */
Context::~Context()
{
}


/**
 * @brief Get the code of the last error that happened.
 *
 * @sa QLMDB::Core::Errors
 */
int Context::lastError() const
{
    const Q_D(Context);
    return d->lastError;
}


/**
 * @brief Get a textual description of the last error.
 *
 * This method returns a description of the last error that occurred.
 */
QString Context::lastErrorString() const
{
    const Q_D(Context);
    return d->lastErrorString;
}


/**
 * @brief Clear the last error that happened.
 */
void Context::clearLastError()
{
    Q_D(Context);
    d->clearLastError();
}


/**
 * @brief The path where to store data.
 *
 * This property holds the path to where the data shall be stored.
 * By default, this is expected to point to a directory, however, it
 * is also possible to store the database in a single file.
 */
QString Context::path() const
{
    const Q_D(Context);
    return d->path;
}


/**
 * @brief Set the path to where data shall be stored.
 */
void Context::setPath(const QString &path)
{
    Q_D(Context);
    d->path = path;
}


/**
 * @brief Customize how the environment shall be opened.
 *
 * These flags influence how the environment is opened. This should
 * either be 0 or an OR-ed combination of the following values:
 *
 * - #FixedMap
 * - #NoSubDir
 * - #ReadOnly
 * - #WriteMap
 * - #NoMetaSync
 * - #NoSync
 * - #MapAsync
 * - #NoTLS
 * - #NoLock
 * - #NoReadAhead
 * - #NoMemInit
 */
unsigned int Context::flags() const
{
    const Q_D(Context);
    return d->flags;
}


/**
 * @brief Set the @p flags used when opening the environment.
 */
void Context::setFlags(unsigned int flags)
{
    Q_D(Context);
    d->flags = flags;
}


/**
 * @brief The UNIX file permissions to use for created files.
 *
 * This property is only used on UNIX operating systems. On Windows,
 * it has no affect. The default value is 0644.
 */
unsigned int Context::mode() const
{
    const Q_D(Context);
    return d->mode;
}


/**
 * @brief Set the UNIX file permissions to apply when creating files.
 */
void Context::setMode(unsigned int mode)
{
    Q_D(Context);
    d->mode = mode;
}


/**
 * @brief The maximum number of named databases allowed in the context.
 *
 * This property holds the maximum number of named databased allowed
 * within a context. Very simple applications needing only a single
 * key-value table can avoid setting this. More complex applications
 * might set this to a value greater than zero to allow using
 * multiple named databases.
 *
 * Note that choosing a very high number for this can decrease
 * performance.
 *
 * By default, this property is set to 0, which means the default
 * value is used.
 */
unsigned int Context::maxDBs() const
{
    const Q_D(Context);
    return d->maxDBs;
}


/**
 * @brief Set the @p maxDBs allowed within a context.
 */
void Context::setMaxDBs(unsigned int maxDBs)
{
    Q_D(Context);
    d->maxDBs = maxDBs;
}


/**
 * @brief The maximum number of readers allowed in the environment.
 *
 * This property holds the maximum number of readers allowed in
 * the environment.
 *
 * Setting it to 0 uses a default value (usually 126) of the underlying
 * library.
 */
unsigned int Context::maxReaders() const
{
    const Q_D(Context);
    unsigned int result = d->maxReaders;
    if (d->open) {
        if (mdb_env_get_maxreaders(d->env, &result) != 0) {
            result = 0;
        }
    }
    return result;
}


/**
 * @brief Set the @p maxReaders of the environment.
 */
void Context::setMaxReaders(unsigned int maxReaders)
{
    Q_D(Context);
    d->maxReaders = maxReaders;
}


/**
 * @brief The map size of the environment.
 *
 * This property holds the map size of the environment in
 * bytes. This size also is the maximum size of data the
 * environment can hold, so if a database grows over time,
 * it might become necessary to re-open it with a larger map
 * size.
 *
 * Settings this value to 0 causes the environment to be
 * opened with the default map size (usually 10 MByte) or
 * the previously used map size.
 */
size_t Context::mapSize() const
{
    const Q_D(Context);
    return d->mapSize;
}


/**
 * @brief Set the @p mapSize of the context.
 */
void Context::setMapSize(size_t mapSize)
{
    Q_D(Context);
    d->mapSize = mapSize;
}


/**
 * @brief Indicates if the environment is open.
 *
 * This property indicates if the environment has been opened.
 */
bool Context::isOpen() const
{
    const Q_D(Context);
    return d->open;
}


/**
 * @brief Open the environment.
 *
 * This opens the environment specified by the given path(). The
 * environment is opened using the configured flags(), mode(),
 * maxDBs(), maxReaders() and mapSize().
 *
 * If opening suceeds, this method returns true and calling isOpen() on
 * the Context should return true afterwards as well. Otherwise, this method
 * returns false. In this case, use lastError() and lastErrorString() to
 * find out why opening failed.
 */
bool Context::open()
{
    Q_D(Context);
    bool result = false;
    if (!d->open) {
        auto env = d->env;
        if (env != nullptr) {
            if (d->setMapSize() &&
                    d->setMaxDBs() &&
                    d->setMaxReaders() &&
                    d->openEnv()) {
                d->open = true;
                result = true;
            }
        }
    }
    return result;
}

} // namespace QLMDB
