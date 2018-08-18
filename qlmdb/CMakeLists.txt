set(
    QLMDB_PUBLIC_HEADERS
    qlmdb_global.h
    context.h
    errors.h
    transaction.h
    database.h
    cursor.h
)
set(
    QLMDB_HEADERS
    ${QLMDB_PUBLIC_HEADERS}
    contextprivate.h
    databaseprivate.h
    cursorprivate.h
    transactionprivate.h
)

set(
    QLMDB_SOURCES
    cursorprivate.cpp
    context.cpp
    contextprivate.cpp
    errors.cpp
    transactionprivate.cpp
    databaseprivate.cpp
    cursor.cpp
    database.cpp
    transaction.cpp
)

if(QLMDB_WITH_STATIC_LIBS)
    set(QLMDB_LIB_MODE STATIC)
else()
    set(QLMDB_LIB_MODE SHARED)
endif()

if(QLMDB_WITH_BUILTIN_LMDB)
    set(
        LMDB_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/liblmdb/mdb.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/liblmdb/midl.c
    )
    set(
        LMDB_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/liblmdb/lmdb.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/liblmdb/midl.h
    )
    set(
        LMDB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/liblmdb
    )
else()
    set(
        LMDB_LIBS
        lmdb
    )
endif()

add_library(
    qlmdb
    ${QLMDB_LIB_MODE}
    ${QLMDB_HEADERS}
    ${QLMDB_SOURCES}
    ${LMDB_SOURCES}
    ${LMDB_HEADERS}
)

target_link_libraries(qlmdb PUBLIC Qt5::Core ${LMDB_LIBS})

if(Threads_FOUND)
    target_link_libraries (qlmdb PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif()


target_include_directories(
    qlmdb
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_include_directories(
    qlmdb
    PRIVATE
    ${LMDB_INCLUDE_DIRS}
)

install(
    TARGETS
        qlmdb
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES
        ${QLMDB_PUBLIC_HEADERS}
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/qlmdb
)
