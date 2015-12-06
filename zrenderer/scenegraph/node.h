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

#ifndef _node_h_
#define _node_h_

#include <zrenderer/scenegraph/types.h>

namespace zrenderer
{

/**
 * Node class represents the scene graph nodes. It
 * carries the corresponding data.( Material, Mesh,
 * Camera, etc ). Nodes can be created through the
 * scene graph, supplied the data.
 */
class Node
{

public:

    virtual ~Node();

    /**
     * Get the node data with the given type
     * @return the type casted data. If the casting fails an empty
     * ptr is returned
     */
    template<class T>
    std::shared_ptr<T> getNodeData()
    {
        return boost::dynamic_pointer_cast<T>( _getNodeData() );
    }

    /**
     * Get the node data with the given type
     * @return the type casted data. If the casting fails an empty
     * ptr is returned
     */
    template<class T>
    std::shared_ptr<const T> getNodeData() const
    {
        return boost::dynamic_pointer_cast<T>( _getNodeData() );
    }

    /**
     * @return the node name
     */
    const std::string& getName() const;

    /**
     * @return the parent node. If there is no parent, an empty
     * ptr is returned.
     */
    NodePtr getParent() const;

    /**
     * Add a node as a child. The node has to be created by the
     * scenegraph before attached.
     * @param node for the child
     * @return true if child can be added.
     */
    bool addChild( const NodePtr& child );

    /**
     * Remove the child. The node has to be part of the same scene
     * graph.
     * @param node for the child
     * @return true if child can be added.
     */
    bool removeChild( const NodePtr& node );

    /**
     * Get the children of a node.
     * @return the array of children
     */
    NodePtrs getChildren() const;

    /**
     * @param node for the child
     * @return true if node has the child
     */
    bool hasChild( const NodePtr& node ) const;

private:

    friend class SceneGraph;

    Node( const std::string& name,
          const NodeDataPtr& nodeData,
          SceneGraph& sceneGraph );

    NodeDataPtr _getNodeData();
    ConstNodeDataPtr _getNodeData() const;

    struct Impl;
    std::unique_ptr<Impl> _impl;
};

}

#endif // _node_h_
