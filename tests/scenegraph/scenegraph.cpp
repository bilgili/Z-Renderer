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

#include <zrenderer/scenegraph/scenegraph.h>
#include <zrenderer/scenegraph/node.h>
#include <zrenderer/scenegraph/visitor.h>

#define BOOST_TEST_MODULE scenegraph
#include <boost/test/unit_test.hpp>

const std::string parentName = "Albert";
const std::string childName1 = "Batman";
const std::string childName2 = "Robin";

class Visitor : public zrenderer::Visitor
{
public:

    Visitor()
        : current( 0 )
        , beginVisited( 0 )
        , rootVisited( 0 )
        , albertVisited( 0 )
        , batmanVisited( 0 )
        , robinVisited( 0 )
        , endVisited( 0 )
    {}

    uint32_t current;
    uint32_t beginVisited;
    uint32_t rootVisited;
    uint32_t albertVisited;
    uint32_t batmanVisited;
    uint32_t robinVisited;
    uint32_t endVisited;

    virtual ~Visitor() {}
    void onBegin( const zrenderer::SceneGraph& ) final
        { beginVisited = current++;  }

    void visit( const zrenderer::SceneGraph&,
                zrenderer::NodePtr node ) final
    {
        const std::string& name = node->getName();
        if( name == zrenderer::ROOT_NODE )
            rootVisited = current++;
        if( name == parentName )
            albertVisited = current++;
        if( name == childName1 )
            batmanVisited = current++;
        if( name == childName2 )
            robinVisited = current++;
    }

    void onEnd( const zrenderer::SceneGraph& ) final
        { endVisited = current++;  }
};


BOOST_AUTO_TEST_CASE( scene_graph_basic )
{
    zrenderer::SceneGraph scenegraph;
    zrenderer::NodePtr rootNode = scenegraph.getRoot();
    BOOST_CHECK( rootNode.get() != 0 );
    BOOST_CHECK( rootNode->getName() == zrenderer::ROOT_NODE );
    BOOST_CHECK( !scenegraph.hasChild( zrenderer::ROOT_NODE,
                                       childName1 ) );
    BOOST_CHECK( scenegraph.findNode( zrenderer::ROOT_NODE ) != 0 );
    BOOST_CHECK( scenegraph.findNode( parentName ) == 0 );
}

BOOST_AUTO_TEST_CASE( add_node_and_visit )
{
    zrenderer::SceneGraph scenegraph;
    zrenderer::NodePtr rootNode = scenegraph.getRoot();

    // Robin is a non existing node
    bool nodeAdded = scenegraph.addChild( zrenderer::ROOT_NODE,
                                          childName1 );
    BOOST_CHECK( !nodeAdded );

    zrenderer::NodePtr nodeParent = scenegraph.createNode( parentName );
    BOOST_CHECK( nodeParent.get() != 0 );

    zrenderer::NodePtr nodeChild1 = scenegraph.createNode( childName1 );
    BOOST_CHECK( nodeChild1.get() != 0 );

    zrenderer::NodePtr nodeChild2 = scenegraph.createNode( childName2 );
    BOOST_CHECK( nodeChild2.get() != 0 );

    // Add child node through scene graph
    nodeAdded = scenegraph.addChild( zrenderer::ROOT_NODE,
                                     nodeParent->getName( ));
    BOOST_CHECK( nodeAdded );

    nodeAdded = scenegraph.addChild( nodeParent->getName( ),
                                     nodeChild1->getName( ));
    BOOST_CHECK( nodeAdded );

     // Add child node through node
    nodeAdded = nodeParent->addChild( nodeChild2 );
    BOOST_CHECK( nodeAdded );

    BOOST_CHECK( scenegraph.hasChild( zrenderer::ROOT_NODE,
                                      nodeParent->getName( )));

    BOOST_CHECK( scenegraph.hasChild( nodeParent->getName(),
                                      nodeChild1->getName( )));

    BOOST_CHECK( nodeParent->hasChild( nodeChild2 ));

    BOOST_CHECK( !scenegraph.hasChild( zrenderer::ROOT_NODE,
                                       nodeChild1->getName( )));

    Visitor visitor;
    scenegraph.traverse( visitor );

    BOOST_CHECK( visitor.beginVisited == 0 );
    BOOST_CHECK( visitor.rootVisited == 1 );
    BOOST_CHECK( visitor.albertVisited == 2 );
    BOOST_CHECK( visitor.batmanVisited == 3 );
    BOOST_CHECK( visitor.robinVisited == 4 );
    BOOST_CHECK( visitor.endVisited == 5 );
}

