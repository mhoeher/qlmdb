#ifndef ERRORS_H
#define ERRORS_H

#include "qlmdb_global.h"

namespace Errors
{
    // LMDB Specific Error Codes:
    extern QLMDBSHARED_EXPORT const int NoError;
    extern QLMDBSHARED_EXPORT const int KeyExists;
    extern QLMDBSHARED_EXPORT const int NotFound;
    extern QLMDBSHARED_EXPORT const int PageNotFound;
    extern QLMDBSHARED_EXPORT const int Corrupted;
    extern QLMDBSHARED_EXPORT const int Panic;
    extern QLMDBSHARED_EXPORT const int VersionMismatch;
    extern QLMDBSHARED_EXPORT const int Invalid;
    extern QLMDBSHARED_EXPORT const int MapFull;
    extern QLMDBSHARED_EXPORT const int DBsFull;
    extern QLMDBSHARED_EXPORT const int ReadersFull;
    extern QLMDBSHARED_EXPORT const int TLSFull;
    extern QLMDBSHARED_EXPORT const int TooManyTransactions;
    extern QLMDBSHARED_EXPORT const int CursorFull;
    extern QLMDBSHARED_EXPORT const int PageFull;
    extern QLMDBSHARED_EXPORT const int MapResized;
    extern QLMDBSHARED_EXPORT const int Incompatible;
    extern QLMDBSHARED_EXPORT const int BadReaderSlot;
    extern QLMDBSHARED_EXPORT const int BadTransaction;
    extern QLMDBSHARED_EXPORT const int BadValueSize;
    extern QLMDBSHARED_EXPORT const int BadDBI;

    // Error Codes from cerrno:
    extern QLMDBSHARED_EXPORT const int InvalidParameter;
    extern QLMDBSHARED_EXPORT const int InvalidPath;
    extern QLMDBSHARED_EXPORT const int NoAccessToPath;
    extern QLMDBSHARED_EXPORT const int TemporarilyNotAvailable;

}

#endif // ERRORS_H
