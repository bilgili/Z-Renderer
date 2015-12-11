/* Copyright (c) 2015, Zombie Rendering
 *                     ahmetbilgili@gmail.com
 *
 * This file is part of Livre <https://github.com/BlueBrain/Livre>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _common_types_
#define _common_types_

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <deque>
#include <algorithm>
#include <cstdint>
#include <functional>

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <zrenderer/common/compiler.h>

namespace zrenderer
{

/**
 * Basic types
 */
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;

using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;

/**
 * Class definitions
 */

class CacheObject;
class Mesh;

/**
 * SmartPtr definition
 */
typedef std::shared_ptr< Mesh > MeshPtr;

/**
 * Locking object definitions
 */
typedef boost::shared_mutex ReadWriteMutex;
typedef boost::shared_lock< ReadWriteMutex > ReadLock;
typedef boost::unique_lock< ReadWriteMutex > WriteLock;
typedef boost::unique_lock< boost::mutex > ScopedLock;


}

#endif // _common_types_
