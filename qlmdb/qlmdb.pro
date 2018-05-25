#-------------------------------------------------
#
# Project created by QtCreator 2018-04-21T20:17:21
#
#-------------------------------------------------

QT       -= gui

TARGET = qlmdb
TEMPLATE = lib

DEFINES += QLMDB_LIBRARY

CONFIG *= create_prl

qlmdb_with_static_libs {
    CONFIG += static
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core/context.cpp \
    core/contextprivate.cpp \
    core/errors.cpp \
    core/transaction.cpp \
    core/transactionprivate.cpp \
    core/database.cpp \
    core/databaseprivate.cpp \

PUBLIC_CORE_HEADERS = \
    core/context.h \
    core/errors.h \
    core/transaction.h \
    core/database.h \

PUBLIC_HEADERS = \
    database.h \
    qlmdb_global.h \
    collection.h \

PRIVATE_HEADERS = \
    databaseprivate.h \
    collectionprivate.h \
    core/contextprivate.h \
    core/transactionprivate.h \
    core/databaseprivate.h

HEADERS += $$PRIVATE_HEADERS $$PUBLIC_HEADERS $$PUBLIC_CORE_HEADERS


qlmdb_with_builtin_lmdb {
    # Use built-in version of LMDB
    INCLUDEPATH += ../3rdparty/liblmdb
    SOURCES += \
        ../3rdparty/liblmdb/mdb.c \
        ../3rdparty/liblmdb/midl.c
    HEADERS += \
        ../3rdparty/liblmdb/lmdb.h \
        ../3rdparty/liblmdb/midl.h
} else {
    # Use system LMDB
    LIBS += -llmdb
}

unix {
    target.path = $$[QT_INSTALL_LIBS]
    INSTALLS += target

    headers.files = $$PUBLIC_HEADERS
    headers.path = $$[QT_INSTALL_HEADERS]/qlmdb

    core_headers.files = $$PUBLIC_CORE_HEADERS
    core_headers.path = $$[QT_INSTALL_HEADERS]/qlmdb/core

    INSTALLS += headers core_headers
}
