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
#ifndef QLMDB_GLOBAL_H
#define QLMDB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QLMDB_LIBRARY_STATIC)
#    define QLMDBSHARED_EXPORT
#else
#    if defined(QLMDB_LIBRARY)
#        define QLMDBSHARED_EXPORT Q_DECL_EXPORT
#    else
#        define QLMDBSHARED_EXPORT Q_DECL_IMPORT
#    endif
#endif

#endif // QLMDB_GLOBAL_H
