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
 * @class QLMDB::Core::Context
 * @brief A LMDB context.
 *
 * The Context class is a wrapper around LMDB's contexts. It acts as
 * RAII wrapper: Each instance of this class owns one LMDB context
 * as
 */

/**
 * @brief Constructor.
 */
Context::Context() :
    d_ptr(new ContextPrivate)
{

}

} // namespace Core
} // namespace QLMDB
