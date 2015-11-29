#include <zrenderer/scenegraph/node.h>
#include <zrenderer/scenegraph/scenegraph.h>
#include <zrenderer/scenegraph/nodedata.h>

namespace zrenderer
{

struct Node::Impl
{
    Impl( const std::string& name,
          NodeDataPtr nodeData,
          SceneGraph& sceneGraph )
        : _name( name )
        , _nodeData( nodeData )
        , sceneGraph( sceneGraph )
            {}
    ~Impl()
    {
        _sceneGraph.removeChild( _name );
    }

    NodePtr getParent() const
    {
        return _sceneGraph.getParent( _name );
    }

    void addChild( const NodePtr& node )
    {
        _sceneGraph.addChild( _name, node->_impl->_name );
    }

    void removeChild( NodePtr node )
    {
        _sceneGraph.removeChild( _name );
    }

    NodePtrs&& getChildren( const Filter& filter ) const
    {
        NodePtrs&& nodes = _sceneGraph.getChildren( _name );
        NodePtrs ret;
        for( const NodePtr& node: nodes )
        {
            if( node->getNodeData()->satisfiesFilter( filter ))
                ret.push_back( node );
        }
        return std::move( nodes );
    }

    SceneGraph& _sceneGraph;
    const std::string _name;
    NodeDataPtr _nodeData;
};

Node::Node( const std::string& name,
            NodeDataPtr nodeData,
            SceneGraph& sceneGraph )
    : _impl( new Node::Impl( name,
                             nodeData,
                             sceneGraph ))
{

}

std::string& Node::getName() const
{
    return _impl->_name;
}

bool Node::addChild( const NodePtr& node )
{
    return _impl->addChild( node );
}

bool Node::removeChild( const NodePtr& node )
{
    return _impl->removeChild( node );
}

NodePtr Node::getParent() const
{
    return _impl->getParent();
}

NodePtrs&& Node::getChildren( const Filter& filter ) const
{
    return _impl->getChildren( filter );
}

Node::~Node() {}

NodeDataPtr Node::getNodeData()
{
    return _impl->_nodeData;
}

ConstNodeDataPtr Node::getNodeData() const
{
    return _impl->_nodeData;
}


}
