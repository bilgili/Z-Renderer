#include <zrenderer/scenegraph/node.h>
#include <zrenderer/scenegraph/scenegraph.h>
#include <zrenderer/scenegraph/nodedata.h>

namespace zrenderer
{

struct Node::Impl
{
    Impl( const std::string& name,
          const NodeDataPtr& nodeData,
          SceneGraph& sceneGraph )
        : _name( name )
        , _nodeData( nodeData )
        , _sceneGraph( sceneGraph )
            {}
    ~Impl()
    {
        _sceneGraph.removeNode( _name );
    }

    NodePtr getParent() const
    {
        return _sceneGraph.getParent( _name );
    }

    bool addChild( const NodePtr& node )
    {
        return _sceneGraph.addChild( _name, node->getName() );
    }

    bool removeChild( const NodePtr& node )
    {
        if( !_sceneGraph.hasChild( _name,node->getName() ))
            return false;

        return _sceneGraph.removeNode( node->getName() );
    }

    NodePtrs&& getChildren() const
    {
        return _sceneGraph.getChildren( _name );
    }

    const std::string _name;
    NodeDataPtr _nodeData;
    SceneGraph& _sceneGraph;
};

Node::Node( const std::string& name,
            const NodeDataPtr& nodeData,
            SceneGraph& sceneGraph )
    : _impl( new Node::Impl( name,
                             nodeData,
                             sceneGraph ))
{

}

const std::string& Node::getName() const
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

NodePtrs&& Node::getChildren() const
{
    return _impl->getChildren();
}

Node::~Node() {}

NodeDataPtr Node::_getNodeData()
{
    return _impl->_nodeData;
}

ConstNodeDataPtr Node::_getNodeData() const
{
    return _impl->_nodeData;
}


}
