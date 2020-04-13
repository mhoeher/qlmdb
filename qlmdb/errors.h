/*
 * This file is part of QLMDB.
 *
 * QLMDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * QLMDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QLMDB.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ERRORS_H
#define ERRORS_H

#include "qlmdb_global.h"

namespace QLMDB {
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
    extern QLMDBSHARED_EXPORT const int OutOfMemory;
    extern QLMDBSHARED_EXPORT const int OutOfDiskSpace;
    extern QLMDBSHARED_EXPORT const int IOError;

} // namespace Errors
} // namespace QLMDB

#endif // ERRORS_H
