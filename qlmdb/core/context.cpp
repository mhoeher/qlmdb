#include "context.h"
#include "contextprivate.h"

namespace QLMDB {

/**
 * @brief Low-level wrappers around the liblmdb core library.
 *
 * This namespace provides classes that wrap the core primitives from
 * liblmdb and make them available via a Qt-fied interface. These classes
 * can be used directly and closely follow the structure of liblmdb itself.
 * In addition, these classes are used by higher level classes in QLMDB.
 *
 * ## Usage
 *
 * Use the classes in this namespace if you want to work on a low level with
 * LMDB databases. The classes are usually 1:1 wrappers around the primitives
 * found in the LMDB library itself, so if you are familiar with it,
 * using these classes should be fairly easy.
 *
 * Note that QLMDB also provides higher level classes, which are intended
 * to make working with LMDB even easier for average applications. However,
 * for applications with high performance requirements, it still might be
 * better to work with the Core classes instead.
 *
 *
 * ## %Core Classes as RAII Wrappers
 *
 * The classes in the Core namespace are implemented using the RAII
 * (resource acquisition is initialization) technique. This means that each
 * instance of a class (unless otherwise documented) maintains one LMDB
 * object. This object is freed as soon as the Core class object is destroyed.
 */
namespace Core {


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
 * @class QLMDB::Core::Context
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
 */

/**
 * @brief Constructor.
 */
Context::Context() :
    d_ptr(new ContextPrivate)
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
 * it has no affect. The default value is 0.
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

} // namespace Core
} // namespace QLMDB
