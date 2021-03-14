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

if(NOT QLMDB_WITH_SYSTEM_LMDB)
    set(
        LMDB_SOURCES
        mdb.c midl.c
    )
    set(
        LMDB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/liblmdb
    )
else()
    set(
        LMDB_LIBS
        ${LMDB_LIBRARIES}
    )

endif()

add_library(
    qlmdb-qt${QT_VERSION_MAJOR}
    ${QLMDB_LIB_MODE}
    ${QLMDB_HEADERS}
    ${QLMDB_SOURCES}
    ${LMDB_SOURCES}
)

target_link_libraries(qlmdb-qt${QT_VERSION_MAJOR} PUBLIC Qt${QT_VERSION_MAJOR}::Core ${LMDB_LIBS})

if(Threads_FOUND)
    target_link_libraries (qlmdb-qt${QT_VERSION_MAJOR} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif()


target_include_directories(
    qlmdb-qt${QT_VERSION_MAJOR}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<INSTALL_INTERFACE:include/qlmdb-qt${QT_VERSION_MAJOR}>
    PRIVATE
        ${LMDB_INCLUDE_DIR}
)

target_compile_definitions(
    qlmdb-qt${QT_VERSION_MAJOR} 

    PRIVATE 
        QLMDB_LIBRARY

    PUBLIC
        # Request export of symbols only if we don't build a static library
        $<IF:$<STREQUAL:$<TARGET_PROPERTY:qlmdb-qt${QT_VERSION_MAJOR},TYPE>,STATIC_LIBRARY>,QLMDB_LIBRARY_STATIC,QLMDB_LIBRARY_SHARED>
)

set_target_properties(
    qlmdb-qt${QT_VERSION_MAJOR}
    PROPERTIES
        SOVERSION ${QLMDB_VERSION}
)


if(ANDROID)
    target_compile_definitions(qlmdb-qt${QT_VERSION_MAJOR} PRIVATE ANDROID)
endif()

install(
    TARGETS
        qlmdb-qt${QT_VERSION_MAJOR}
    EXPORT
        QLMDBTargets
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES
        ${QLMDB_PUBLIC_HEADERS}
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/qlmdb-qt${QT_VERSION_MAJOR}/qlmdb
)


# Add an alias target, so that including the project in source form in another
# project, is possible as well without writing additional code in the host
# project:
add_library(QLMDB::qlmdb-qt${QT_VERSION_MAJOR} ALIAS qlmdb-qt${QT_VERSION_MAJOR})