#-------------------------------------------------
#
# Project created by QtCreator 2018-04-21T20:17:21
#
#-------------------------------------------------

QT       -= gui

TARGET = qlmdb
TEMPLATE = lib

DEFINES += QLMDB_LIBRARY

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
    database.cpp \
    databaseprivate.cpp \
    collection.cpp \
    collectionprivate.cpp \
    core/context.cpp \
    core/contextprivate.cpp

HEADERS += \
    database.h \
    qlmdb_global.h \
    databaseprivate.h \
    collection.h \
    collectionprivate.h \
    core/context.h \
    core/contextprivate.h

# Add LMDB library:
INCLUDEPATH += ../3rdparty
SOURCES += \
    ../3rdparty/liblmdb/mdb.c \
    ../3rdparty/liblmdb/midl.c
HEADERS += \
    ../3rdparty/liblmdb/lmdb.h \
    ../3rdparty/liblmdb/midl.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
