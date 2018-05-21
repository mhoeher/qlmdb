#include "errors.h"

#include "lmdb.h"

/**
 * @brief Error codes.
 *
 * This namespace provides the error codes used in the library. Classes
 * which track error codes usually have a `lastError()` method which
 * returns one of the errors listed in this namespace.
 */
namespace Errors {


/**
 * @brief No error occurred.
 */
const int NoError = MDB_SUCCESS;

/**
 * @brief A key already exists in a table.
 */
const int KeyExists = MDB_KEYEXIST;


/**
 * @brief The given key/value pair has not been found.
 */
const int NotFound = MDB_NOTFOUND;


/**
 * @brief A requested page has not been found.
 *
 * This usually indicates a corruption.
 */
const int PageNotFound = MDB_PAGE_NOTFOUND;


/**
 * @brief A located page has a wrong type.
 */
const int Corrupted = MDB_CORRUPTED;


/**
 * @brief An update of a meta page failed or the environment had a fatal error.
 */
const int Panic = MDB_PANIC;


/**
 * @brief There was a version mismatch in the environment.
 */
const int VersionMismatch = MDB_VERSION_MISMATCH;


/**
 * @brief The selected file is not a valid LMDB file.
 */
const int Invalid = MDB_INVALID;


/**
 * @brief The maximum map size configured in the environment has been reached.
 */
const int MapFull = MDB_MAP_FULL;


/**
 * @brief Maximum number of DBs configured in the environment has been reached.
 */
const int DBsFull = MDB_DBS_FULL;


/**
 * @brief The maximum number of readers has been reached.
 */
const int ReadersFull = MDB_READERS_FULL;


/**
 * @brief Too many TLS keys are in use.
 *
 * This is specific to Windows.
 */
const int TLSFull = MDB_TLS_FULL;


/**
 * @brief There are too many dirty pages.
 */
const int TooManyTransactions = MDB_TXN_FULL;


/**
 * @brief The internal cursor stack is too deep.
 */
const int CursorFull = MDB_CURSOR_FULL;


/**
 * @brief A page has not enough space.
 */
const int PageFull = MDB_PAGE_FULL;


/**
 * @brief The environment map size has been resized by another process.
 */
const int MapResized = MDB_MAP_RESIZED;


/**
 * @brief The operation and DB are incompatible.
 *
 * This can be due to:
 *
 * - Tried to do a multi-get/put in a non-multi-valued DB.
 * - Opening a named DB when the unnamed DB has multi or integer keys.
 * - Accessing a data record as a database, or vice versa.
 * - The database was dropped and recreated with different flags.
 */
const int Incompatible = MDB_INCOMPATIBLE;


/**
 * @brief Invalid reuse of a reader locktable entry.
 */
const int BadReaderSlot = MDB_BAD_RSLOT;


/**
 * @brief The transaction must abort, has a child or is invalid.
 */
const int BadTransaction = MDB_BAD_TXN;


/**
 * @brief Unsupported size of key, DB name, data, or wrong DUPFIXED size.
 */
const int BadValueSize = MDB_BAD_VALSIZE;


/**
 * @brief The specified DBI has changed unexpectedly.
 */
const int BadDBI = MDB_BAD_DBI;
}
