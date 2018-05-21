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

/** } */


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

} // namespace Core
} // namespace QLMDB
