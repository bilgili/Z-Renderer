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

#ifndef _lrucachepolicy_h_
#define _lrucachepolicy_h_

#include <zrenderer/common/types.h>
#include <zrenderer/common/cache/cachable.h>

namespace zrenderer
{

template< class CacheObject >
class LRUCachePolicy
{
public:

    typedef typename CacheObject::key_type Key;
    typedef std::deque< Key > LRUQueue;
    typedef std::vector< Key > Keys;

    /**
     * @param maxMemory Max memory for the cache
     */
    LRUCachePolicy( size_t maxMemory )
        : _maxMemory( maxMemory )
        , _currentMemory( 0 )
    {}

    /**
     * Inserts a cachable object into queue. If object is
     * already in the queue, it is pushed to back of the queue.
     * @param cachable the cachable object
     */
    void insert( const CacheObject& cachable )
    {
        if( !_remove( cachable.getKey() ) )
            _currentMemory += cachable.getSize();
        _lruQueue.push_back( cachable.getKey( ));
    }

    /**
     * Removes a cachable object from the queue.
     * @param cachable
     */
    void remove( const CacheObject& cachable )
    {
        if( _remove( cachable.getKey() ))
            _currentMemory -= cachable.getSize();
    }

    /**
     * @return the list of cachable keys
     */
    Keys getKeys() const
    {
        Keys keys;
        keys.reserve( _lruQueue.size() );
        for( const Key& key: _lruQueue )
            keys.push_back( key );
        return keys;
    }

    /**
     * @return true if cache has tobe cleaned
     */
    bool cleanCache() const
    {
        return _currentMemory >= _maxMemory;
    }

    /**
     * @return true if cache is empty
     */
    bool isEmpty() const
    {
        return _lruQueue.empty();
    }

    /**
     * @return Return the current memory usage
     */
    size_t getUsage() const { return _currentMemory; }

    /**
     * @return Return the max memory
     */
    size_t getMaxMemory() const { return _maxMemory; }

private:

    bool _remove( const Key& key )
    {
        typename LRUQueue::const_iterator it = _lruQueue.begin();
        while( it != _lruQueue.end( ))
        {
            if( *it == key )
            {
                _lruQueue.erase( it );
                return true;
            }
            else ++it;
        }
        return false;
    }

    LRUQueue _lruQueue;
    size_t _maxMemory;
    size_t _currentMemory;
};

}

#endif
