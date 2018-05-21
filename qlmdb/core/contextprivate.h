#ifndef CONTEXTPRIVATE_H
#define CONTEXTPRIVATE_H

#include <lmdb.h>

#include <QObject>
#include <QString>
#include <QDir>

#include "errors.h"

namespace QLMDB {
namespace Core {

//! @private
class ContextPrivate
{
public:
    ContextPrivate();
    virtual ~ContextPrivate();

    MDB_env *env;
    int lastError;
    QString lastErrorString;
    QString path;
    unsigned int flags;
    unsigned int mode;
    unsigned int maxDBs;
    unsigned int maxReaders;
    size_t mapSize;
    bool open;


    inline void clearLastError() {
        lastError = 0;
        lastErrorString.clear();
    }

    inline bool setMapSize() {
        bool result = false;
        if (mapSize > 0) {
            lastError = mdb_env_set_mapsize(env, mapSize);
            if (lastError == 0) {
                lastErrorString.clear();
                result = true;
            } else if (lastError == Errors::InvalidParameter) {
                lastErrorString = QObject::tr("Invalid map size: %1").arg(
                            mapSize);
            } else {
                lastErrorString = QObject::tr("Unknown error setting map size");
            }
        }
        return result;
    }

    inline bool setMaxDBs() {
        bool result = false;
        if (maxDBs > 0) {
            lastError = mdb_env_set_maxdbs(env, maxDBs);
            if (lastError == 0) {
                lastErrorString.clear();
                result = true;
            } else if (lastError == Errors::InvalidParameter) {
                lastErrorString = QObject::tr("Invalid max DBs: %1").arg(
                            maxDBs);
            } else {
                lastErrorString = QObject::tr("Unknown error setting max "
                                              "DBs");
            }
        }
        return result;
    }

    inline bool setMaxReaders() {
        bool result = false;
        if (maxReaders > 0) {
            lastError = mdb_env_set_maxreaders(env, maxReaders);
            if (lastError == 0) {
                lastErrorString.clear();
                result = true;
            } else if (lastError == Errors::InvalidParameter) {
                lastErrorString = QObject::tr("Invalid maximum readers: %1"
                                              ).arg(maxReaders);
            } else {
                lastErrorString = QObject::tr("Unknown error setting max "
                                              "readers");
            }
        }
        return result;
    }

    inline bool openEnv() {
        bool result = false;
        if (path.isEmpty()) {
            lastError = Errors::InvalidPath;
            lastErrorString = QObject::tr("Empty path passed to environment");
        } else {
            auto p = QDir::toNativeSeparators(path).toStdString();
            lastError = mdb_env_open(env, p.c_str(), flags, mode);
            if (lastError == 0) {
                lastErrorString.clear();
                result = true;
            } else if (lastError == Errors::VersionMismatch) {
                lastErrorString = QObject::tr("Version mismatch opening "
                                              "environment");
            } else if (lastError == Errors::Invalid) {
                lastErrorString = QObject::tr("The environment file headers are "
                                              "corrupted");
            } else if (lastError == Errors::InvalidPath) {
                lastErrorString = QObject::tr("Invalid path passed to "
                                              "environment: %1").arg(path);
            } else if (lastError == Errors::NoAccessToPath) {
                lastErrorString = QObject::tr("Cannot access path: %1").arg(
                            path);
            } else if (lastError == Errors::TemporarilyNotAvailable) {
                lastErrorString = QObject::tr("Environment is currently locked "
                                              "by another process: %1").arg(
                            path);
            }
        }
        return result;
    }
};

} // namespace Core
} // namespace QLMDB

#endif // CONTEXTPRIVATE_H
