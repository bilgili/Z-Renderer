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
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

namespace zrenderer
{

typedef boost::adjacency_list< boost::listS,
                               boost::vecS,
                               boost::directedS,
                               boost::no_property,
                               NodePtr > Graph;

typedef Graph::vertex_descriptor NodeDescriptor;
typedef std::unordered_map< std::string, NodeDescriptor > NodeMap;


class DFSNodeVisitor : public boost::default_dfs_visitor
{
public:
    DFSNodeVisitor( Visitor& visitor )
    :  _visitor( visitor )
    {}

    template < typename NodePtr, typename Graph >
    void start_vertex( NodePtr node, const Graph& graph UNUSED ) const
    {
        _visitor->preVisit( node );
    }

    template < typename NodePtr, typename Graph >
    void examine_vertex( NodePtr node, const Graph& graph UNUSED ) const
    {
        _visitor->visit( node );
    }

    template < typename Vertex, typename Graph >
    void finish_vertex( NodePtr node, const Graph& graph UNUSED ) const
    {
        _visitor->postVisit( node );
    }
    Visitor _visitor;
};

struct SceneGraph::Impl
{
    Impl( SceneGraph& sceneGraph )
        : _rootNode( new Node( ROOT_NODE,
                               NodeDataPtr(),
                               sceneGraph ))
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
                        NodeDataPtr nodeData )
    {
        if( nodeExists( name ) )
            return NodePtr();

        WriteLock writeLock( _mutex );
        NodePtr node( new Node( name,
                                nodeData,
                                sceneGraph ));

        _nodeMap[ name ] =
                boost::add_vertex( node, _graph );
        return node;
    }

    NodePtr findNode( const std::string& name ) const
    {
        ReadLock readLock( _mutex );
        NodeMap::iterator it = _nodeMap.find( name );
        if( it != _nodeMap.end( ))
            return _graph[ it->second ];

        return NodePtr();
    }

    bool removeNode( const std::string& name )
    {
        if( nodeExists( name ) )
            return false;

        WriteLock writeLock( _mutex );
        _nodeMap[ name ] =
                boost::add_vertex( node, _graph );
        boost::remove_vertex( _nodeMap[ name ] );
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
        const NodeDescriptor& nd = _nodeMap[ child ];
        Graph::in_edge_iterator begin, end;
        boost::tie( begin, end ) = boost::in_edges( nd, _graph );
        if( begin == end )
            return NodePtr();

        return _graph[ boost::source( *begin, graph ) ];
    }

    NodePtrs&& getChildren( const std::string& parent ) const
    {
        NodePtrs children;
        if( nodeExists( parent ) )
            return std::move( children );

        ReadLock readLock( _mutex );
        const NodeDescriptor& nd = _nodeMap[ child ];

        Graph::out_edge_iterator begin, end;
        for( boost::tie( begin, end) = boost::out_edges( nd, _graph );
             begin != end; ++begin )
        {
            const NodeDescriptor& child =
                    boost::source( boost::target( *begin, _graph ));
            children.push_back( _graph[ child ] );
        }
        return std::move( children );
    }

    void SceneGraph::traverse( const Visitor& visitor,
                               const std::string& name ) const
    {
        if( nodeExists( name ) )
            return;

        ReadLock readLock( _mutex );
        DFSNodeVisitor dfs( visitor );
        boost::depth_first_search( _graph,
                                   boost::visitor( dfs ),
                                   _nodeMap[ name ] );
    }

    NodePtr _rootNode;
    NodeMap _nodeMap;
    mutable ReadWriteMutex _mutex;
    Graph _graph;
};

ConstNodePtr getRoot() const
{
    return _impl->rootNode;
}

NodePtr getRoot()
{
    return _impl->rootNode;
}

NodePtr SceneGraph::createNode( const std::string& name,
                                NodeDataPtr nodeData )
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

NodePtr SceneGraph::getParent(const std::string& child ) const
{
    return _impl->getParent( child );
}

NodePtrs&& SceneGraph::getChildren(const std::string& parent) const
{
    return _impl->getChildren( parent );
}

void SceneGraph::traverse( const Visitor& visitor,
                           const std::string& name ) const
{
    _impl->traverse( visitor, name );
}

}

