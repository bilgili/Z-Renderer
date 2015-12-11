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

#ifndef _cache_h_
#define _cache_h_

#include <zrenderer/common/types.h>
#include <zrenderer/common/cache/lrucachepolicy.h>

#include <boost/mpl/list.hpp>

namespace zrenderer
{

/**
 * Caches an object and keeps the track of references. When policy
 * limits are reached deletes object according to the given precedence
 * given by the policy ( LRU, MRU ... )
 *
 * Also given the allocator, it transfers it to the cache object on the
 * construction time. So, cache object can allocate with the given memory
 * region/algorithm etc.
 */
template< typename CacheObject,
          typename Allocator,
          typename CachePolicy = LRUCachePolicy< CacheObject > >
class Cache
{
public:

    typedef typename CacheObject::key_type Key;

    typedef std::map< Key, std::unique_ptr<CacheObject> > DataMap;
    typedef std::pair< Key, std::unique_ptr<CacheObject> > DataPair;
    typedef std::vector< Key > Keys;

    /**
     * Construct a cache with a given allocator.
     * @param allocator C++ allocator.
     */
    Cache( const Allocator& allocator, size_t maxMemory )
        : _allocator( allocator )
        , _cachePolicy( maxMemory )
    {}

    /**
     * Construct a cache object. If an object with the same key
     * was created before, returns this object. If cache is
     * overcommiting an empty ptr is returned.
     *
     * Additionally the allocator is passed to the constructor
     * of the cache object where cache object can use to it
     * to allocate its own data structures.
     *
     * @param key is id of the cache object
     * @param args are the constructor parameters
     * @throw bad_alloc when allocator does not allow more memory
     * allocation. ( User may need to allocate more memory )
     */
    template< class... Args >
    std::shared_ptr< CacheObject > create( const Key& key,
                                           Args&&... args )
    {
        {
            ReadLock lock( _mutex );
            typename DataMap::const_iterator it = _dataMap.find( key );
            if( it != _dataMap.end( ))
            {
                it->second->increaseRef();
                _cachePolicy.insert( *it->second );
                return std::shared_ptr< CacheObject >( it->second.get( ),
                                                       std::bind( &Cache::_onDelete,
                                                                  this,
                                                                  std::placeholders::_1 ));
            }
        }

        WriteLock lock( _mutex );
        typename DataMap::const_iterator it = _dataMap.find( key );
        if( it != _dataMap.end( ))
            return std::shared_ptr< CacheObject >( it->second.get( ));

        CacheObject* cacheObject = _allocator.allocate( sizeof( CacheObject ));
        _allocator.construct( cacheObject, key, args..., _allocator );

        const size_t objectSize = cacheObject->getSize();
        const size_t maxMemory =  _cachePolicy.getMaxMemory();

        if( objectSize + _cachePolicy.getUsage() > maxMemory )
            _cleanCache();

        if( objectSize + _cachePolicy.getUsage() > maxMemory )
        {
            delete cacheObject;
            return std::shared_ptr< CacheObject >();
        }

        _cachePolicy.insert( *cacheObject );
        cacheObject->increaseRef();
        _dataMap.insert( std::move( DataPair( key,
                                              std::unique_ptr<CacheObject>( cacheObject ))));
        return std::shared_ptr< CacheObject >( cacheObject,
                                               std::bind( &Cache::_onDelete,
                                                          this,
                                                          std::placeholders::_1 ));
    }

    /**
     * @param key is the key of the cache object
     * @return a cache object. If there is no cache object with key,
     * empty ptr is returned.
     */
    std::shared_ptr< CacheObject > get( const Key& key ) const
    {
        ReadLock lock( _mutex );
        typename DataMap::const_iterator it = _dataMap.find( key );
        if( it == _dataMap.end( ))
            return std::shared_ptr< CacheObject >();

        _cachePolicy.insert( *it->second );
        it->second->increaseRef();
        return typename std::shared_ptr< CacheObject >( it->second.get(),
                                                        std::bind( &Cache::_onDelete,
                                                                   this,
                                                                   std::placeholders::_1 ));
    }

    const CachePolicy& getPolicy() const { return _cachePolicy; }

private:

    Cache( const Cache& ) = delete;
    Cache& operator=( const Cache& ) = delete;

    void _onDelete( CacheObject* cacheObject ) const
    {
        WriteLock lock( _mutex );
        cacheObject->decreaseRef();
    }

    void _cleanCache()
    {
        for( const Key& deleteKey: _cachePolicy.getKeys())
        {
            const CacheObject& cacheObject = *_dataMap[ deleteKey ];
            if( cacheObject.getRefCount() == 0 )
            {
                _cachePolicy.remove( cacheObject );
                _dataMap.erase( deleteKey );
                if( !_cachePolicy.cleanCache( ))
                    return;
            }
        }
    }

    Allocator _allocator;
    mutable CachePolicy _cachePolicy;
    DataMap _dataMap;
    mutable ReadWriteMutex _mutex;
};

}

#endif
