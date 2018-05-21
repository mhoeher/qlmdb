#include <QObject>

#include "contextprivate.h"


namespace QLMDB {
namespace Core {

ContextPrivate::ContextPrivate() :
    env(nullptr),
    lastError(0),
    lastErrorString(),
    path(),
    flags(0),
    mode(0),
    maxDBs(0),
    maxReaders(0),
    mapSize(0),
    open(false)
{
    lastError = mdb_env_create(&env);
    if (lastError != 0) {
        lastErrorString = QObject::tr("Failed to create environment");
    }
}

ContextPrivate::~ContextPrivate()
{
    if (env != nullptr) {
        mdb_env_close(env);
    }
}

} // namespace Core
} // namespace QLMDB
