
/* Copyright (c) 2015 <zombierendering@gmail.com>
 *
 */

#include <string>

#define BOOST_TEST_MODULE example
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( text_test )
{
    std::string text1 = "text1";
    std::string text2 = "text2";
    BOOST_CHECK_NE( text1, text2 );
}

