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

#ifndef _cachable_h_
#define _cachable_h_

#include <zrenderer/common/types.h>

namespace zrenderer
{

/**
 * Base object for cachable objects.
 */
template< typename Key >
class Cachable
{
public:

    Cachable( const Key& key )
        : _key( key )
        , _refCount( 0 )
    {}

    virtual ~Cachable() {}

    typedef Key key_type;

    /**
     * @return the size of allocation
     */
    virtual size_t getSize() const = 0;

    /**
     * @return the key of the cachable object
     */
    const Key& getKey() const { return _key; }

    /**
     * Increases the ref count
     */
    void increaseRef() { ++_refCount; }

    /**
     * Decreases the ref count
     */
    void decreaseRef() { --_refCount; }

    /**
     * @return the ref count
     */
    uint64_t getRefCount() const { return _refCount; }

private:

    Key _key;
    std::atomic< uint64_t > _refCount;
};

}

#endif
