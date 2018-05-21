#ifndef ERRORS_H
#define ERRORS_H

#include "lmdb.h"

namespace Errors
{
    extern const int NoError;
    extern const int KeyExists;
    extern const int NotFound;
    extern const int PageNotFound;
    extern const int Corrupted;
    extern const int Panic;
    extern const int VersionMismatch;
    extern const int Invalid;
    extern const int MapFull;
    extern const int DBsFull;
    extern const int ReadersFull;
    extern const int TLSFull;
    extern const int TooManyTransactions;
    extern const int CursorFull;
    extern const int PageFull;
    extern const int MapResized;
    extern const int Incompatible;
    extern const int BadReaderSlot;
    extern const int BadTransaction;
    extern const int BadValueSize;
    extern const int BadDBI;
}

#endif // ERRORS_H
