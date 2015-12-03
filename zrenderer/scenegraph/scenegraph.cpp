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

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

namespace zrenderer
{

typedef boost::adjacency_list< boost::vecS,
                               boost::vecS,
                               boost::bidirectionalS    ,
                               NodePtr > Graph;

typedef Graph::vertex_descriptor NodeDescriptor;
typedef std::unordered_map< std::string, NodeDescriptor > NodeMap;
class DFSNodeVisitor : public boost::default_dfs_visitor
{
public:
    DFSNodeVisitor( const SceneGraph& scenegraph,
                    Visitor& visitor )
    : _scenegraph( scenegraph )
    , _visitor( visitor )
    {}

    void discover_vertex( NodeDescriptor nd, const Graph& graph ) const
    {
        _visitor.visit( _scenegraph, graph[ nd ] );
    }

    const SceneGraph& _scenegraph;
    Visitor& _visitor;
};

struct SceneGraph::Impl
{
    Impl( SceneGraph& sceneGraph )
        : _rootNode( new Node( ROOT_NODE,
                               NodeDataPtr(),
                               sceneGraph ))
        , _sceneGraph( sceneGraph )
    {
        const NodeDescriptor& nd =
                boost::add_vertex( _rootNode, _graph );
        _nodeMap[ ROOT_NODE ] = nd;
    }

    ~Impl() {}

    bool nodeExists( const std::string& name ) const
    {
        ReadLock readLock( _mutex );
        return _nodeMap.count( name ) == 1;
    }

    NodePtr createNode( const std::string& name,
                        const NodeDataPtr& nodeData )
    {
        if( nodeExists( name ) )
            return NodePtr();

        WriteLock writeLock( _mutex );
        NodePtr node( new Node( name,
                                nodeData,
                                _sceneGraph ));

        _nodeMap[ name ] =
                boost::add_vertex( node, _graph );
        return node;
    }

    NodePtr findNode( const std::string& name ) const
    {
        ReadLock readLock( _mutex );
        NodeMap::const_iterator it = _nodeMap.find( name );
        if( it != _nodeMap.end( ))
            return _graph[ it->second ];

        return NodePtr();
    }

    bool removeNode( const std::string& name )
    {
        if( nodeExists( name ) )
            return false;

        WriteLock writeLock( _mutex );
        boost::remove_vertex( _nodeMap[ name ], _graph );
        _nodeMap.erase( name );
        return true;
    }

    bool addChild( const std::string& parent,
                   const std::string& child )
    {
        if( !nodeExists( parent ) ||
            !nodeExists( child ))
        {
            return false;
        }

        WriteLock writeLock( _mutex );
        boost::add_edge( _nodeMap[ parent ],
                         _nodeMap[ child ],
                         _graph );
        return true;
    }

    NodePtr getParent(const std::string& child ) const
    {
        if( nodeExists( child ) )
            return NodePtr();

        ReadLock readLock( _mutex );
        const NodeDescriptor& nd = _nodeMap.find( child )->second;
        Graph::in_edge_iterator begin, end;
        boost::tie( begin, end ) = boost::in_edges( nd, _graph );
        if( begin == end )
            return NodePtr();

        return _graph[ boost::source( *begin, _graph ) ];
    }

    NodePtrs&& getChildren( const std::string& parent ) const
    {
        NodePtrs children;
        if( nodeExists( parent ) )
            return std::move( children );

        ReadLock readLock( _mutex );
        const NodeDescriptor& nd = _nodeMap.find( parent )->second;

        Graph::out_edge_iterator begin, end;
        for( boost::tie( begin, end) = boost::out_edges( nd, _graph );
             begin != end; ++begin )
        {
            const NodeDescriptor& child =
                    boost::target( *begin, _graph );
            children.push_back( _graph[ child ] );
        }
        return std::move( children );
    }

    bool hasChild( const std::string& parent,
                   const std::string& child ) const
    {

        if( !nodeExists( parent ) ||
            !nodeExists( child ))
        {
            return false;
        }

        ReadLock readLock( _mutex );
        const NodeDescriptor& nd = _nodeMap.find( parent )->second;

        Graph::out_edge_iterator begin, end;
        for( boost::tie( begin, end) = boost::out_edges( nd, _graph );
             begin != end; ++begin )
        {
            const NodeDescriptor& cnd =
                    boost::target( *begin, _graph );
            const NodePtr& node = _graph[ cnd ];
            if( node->getName() == child )
                return true;
        }
        return false;
    }

    void traverse( Visitor& visitor,
                   const std::string& name )
    {
        if( !nodeExists( name ) )
            return;

        ReadLock readLock( _mutex );
        visitor.onBegin( _sceneGraph );
        DFSNodeVisitor dfs( _sceneGraph, visitor );
        boost::depth_first_search( _graph,
                                   boost::visitor( dfs )
                                   .root_vertex( _nodeMap[ name ] ));
        visitor.onEnd( _sceneGraph );
    }

    NodePtr _rootNode;
    NodeMap _nodeMap;
    mutable ReadWriteMutex _mutex;
    Graph _graph;
    SceneGraph& _sceneGraph;
};

SceneGraph::SceneGraph()
    : _impl( new SceneGraph::Impl( *this ))
{}

SceneGraph::~SceneGraph()
{}

ConstNodePtr SceneGraph::getRoot() const
{
    return _impl->_rootNode;
}

NodePtr SceneGraph::getRoot()
{
    return _impl->_rootNode;
}

NodePtr SceneGraph::createNode( const std::string& name,
                                const NodeDataPtr& nodeData )
{
    return _impl->createNode( name, nodeData );
}

NodePtr SceneGraph::findNode( const std::string& name ) const
{
    return _impl->findNode( name );
}

bool SceneGraph::removeNode( const std::string& name )
{
    return _impl->removeNode( name );
}

bool SceneGraph::addChild( const std::string& parent,
                           const std::string& child )
{
    return _impl->addChild( parent, child );
}

NodePtr SceneGraph::getParent( const std::string& child ) const
{
    return _impl->getParent( child );
}

NodePtrs&& SceneGraph::getChildren( const std::string& parent ) const
{
    return _impl->getChildren( parent );
}

bool SceneGraph::hasChild (const std::string& parent,
                           const std::string& child ) const
{
    return _impl->hasChild( parent, child );
}

void SceneGraph::traverse( Visitor& visitor,
                           const std::string& name )
{
    _impl->traverse( visitor, name );
}

}

