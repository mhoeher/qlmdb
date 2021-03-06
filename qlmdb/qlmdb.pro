#-------------------------------------------------
#
# Project created by QtCreator 2018-04-21T20:17:21
#
#-------------------------------------------------

QT       -= gui

TARGET = qlmdb
TEMPLATE = lib

DEFINES += QLMDB_LIBRARY

CONFIG *= create_prl c++11

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
    context.cpp \
    contextprivate.cpp \
    errors.cpp \
    transaction.cpp \
    transactionprivate.cpp \
    database.cpp \
    databaseprivate.cpp \
    cursor.cpp \
    cursorprivate.cpp

PUBLIC_HEADERS = \
    qlmdb_global.h \
    context.h \
    errors.h \
    transaction.h \
    database.h \
    cursor.h \

PRIVATE_HEADERS = \
    contextprivate.h \
    transactionprivate.h \
    databaseprivate.h \
    cursorprivate.h \

HEADERS += $$PRIVATE_HEADERS $$PUBLIC_HEADERS


qlmdb_with_builtin_lmdb {
    # Use built-in version of LMDB
    INCLUDEPATH += ../3rdparty/liblmdb
    SOURCES += mdb.c midl.c

    # https://github.com/SUIRON/CLMDB/issues/2
    ios:DEFINES += MDB_USE_POSIX_SEM
} else {
    # Use system LMDB
    LIBS += -llmdb
}

isEmpty(INSTALL_PREFIX) {
    target.path = $$[QT_INSTALL_LIBS]
} else {
    win32 {
        target.path = $$INSTALL_PREFIX/bin
    } else {
        target.path = $$INSTALL_PREFIX/lib
    }
}
INSTALLS += target

headers.files = $$PUBLIC_HEADERS
isEmpty(INSTALL_PREFIX) {
    headers.path = $$[QT_INSTALL_HEADERS]/include/qlmdb
} else {
    headers.path = $$INSTALL_PREFIX/include/qlmdb
}

INSTALLS += headers

OTHER_FILES += mainpage.dox namespaces.dox
