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

#include "cursorprivate.h"
#include "errors.h"

namespace QLMDB {

CursorPrivate::CursorPrivate() :
    cursor(nullptr),
    lastError(Errors::NoError),
    lastErrorString(),
    valid(false)
{

}

} // namespace QLMDB
