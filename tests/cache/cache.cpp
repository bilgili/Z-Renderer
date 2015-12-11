/* Copyright (c) 2015, Zombie Rendering
 *                     ahmetbilgili@gmail.com
 *
 * This file is part of Z-Renderer <https://github.com/ZombieRendering/Z-Renderer>
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


#include <zrenderer/common/cache/cachable.h>
#include <zrenderer/common/cache/cache.h>
#include <zrenderer/common/cache/lrucachepolicy.h>

#include <memory>

#define BOOST_TEST_MODULE cache
#include <boost/test/unit_test.hpp>

class TestObject : public zrenderer::Cachable< std::string >
{
public:

    TestObject( const std::string& key,
                size_t size,
                std::allocator<TestObject>& )
        : Cachable( key )
        , _size( size )
    {}

    size_t getSize() const { return _size; }

private:
    size_t _size;
};

typedef std::shared_ptr< TestObject > TestObjectPtr;

typedef zrenderer::Cache< TestObject,
                          std::allocator< TestObject > > Cache;


BOOST_AUTO_TEST_CASE( construct_cache_object )
{
    std::allocator<TestObject> allocator;
    Cache cache( allocator, 1024 );

    TestObjectPtr cacheObject1 = cache.create( "woody", 5 );
    TestObjectPtr cacheObject2 = cache.create( "woody", 5 );
    BOOST_CHECK( cacheObject1->getRefCount() == 2 );

    TestObjectPtr cacheObject3 = cacheObject1;
    BOOST_CHECK( cacheObject1->getRefCount() == 2 );

    BOOST_CHECK( cacheObject1.get() == cacheObject2.get() );
    BOOST_CHECK( cacheObject1->getRefCount() == 2 );

    TestObject* rawPtr = cacheObject1.get();
    cacheObject1.reset();
    cacheObject3.reset();
    BOOST_CHECK( rawPtr->getRefCount() == 1 );
    cacheObject2.reset();
    BOOST_CHECK( rawPtr->getRefCount() == 0 );
    cacheObject1 = cache.get( "woody" );
    BOOST_CHECK( rawPtr->getRefCount() == 1 );
    BOOST_CHECK( cacheObject1.get() == rawPtr );
    cacheObject1.reset(); // No reference to woody left
    BOOST_CHECK( rawPtr->getRefCount() == 0 );

    TestObjectPtr cacheObject4 = cache.create( "allen", 10 );
    BOOST_CHECK( cacheObject4.get() );
    BOOST_CHECK( cacheObject4->getKey() == "allen" );
    BOOST_CHECK( cacheObject4->getSize() == 10 );

    // Now alien will overcommit the cache, so wont be generated
    TestObjectPtr cacheObject5 = cache.create( "alien", 10000 );
    BOOST_CHECK( !cacheObject5.get() );

    // Now alien will not overcommit the cache, so will be generated
    cacheObject5 = cache.create( "alien", 1010 );
    BOOST_CHECK( cacheObject5.get() );

    cacheObject1 = cache.get( "woody" );
    // So woody was deleted in order to make space
    BOOST_CHECK( !cacheObject1 );

}
