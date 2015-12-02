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

#include <zrenderer/scenegraph/types.h>

namespace zrenderer
{

/**
 * This class holds the tree structure of the scene and
 * provides thread-safe function for querying and generating
 * nodes. Every node in the tree has a unique name
 * identifier
 */
class SceneGraph
{
public:

    SceneGraph();
    ~SceneGraph();

    /**
     * @return the root node
     */
    ConstNodePtr getRoot() const;

    /**
     * @return the root node
     */
    NodePtr getRoot();

    /**
     * Creates a new node in the tree with given name
     * @return the generated node. If a node with the
     * same name is there return an empty NodePtr
     */
    NodePtr createNode( const std::string& name,
                        const NodeDataPtr& nodeData );

    /**
     * Finds a node in the scene graph
     * @param name of the node
     * @return the node. If there is no node with the name,
     * return an empty NodePtr
     */
    NodePtr findNode( const std::string& name ) const;

    /**
     * Removes the node from the scene graph
     * @param name of the node
     * @return true if node can be removed.
     */
    bool removeNode( const std::string& nodeName );

    /**
     * Add a child to the parent node. Nodes has to
     * be created before.
     * @param parent node name
     * @param child node name
     * @return true if child can be added to parent
     */
    bool addChild( const std::string& parent,
                   const std::string& child );

    /**
     * Get the parent node of a child.
     * @param child node name
     * @return the parent node. If there is not, return an
     * empty NodePtr
     */
    NodePtr getParent( const std::string& child ) const;

    /**
     * Get the children of a node
     * @param parent node name
     * @return the array of children
     */
    NodePtrs&& getChildren( const std::string& parent ) const;

    /**
     * Queries if a parent node has a child
     * @param parent node name
     * @param child node name
     * @return true if parent has the child
     */
    bool hasChild( const std::string& parent,
                   const std::string& child ) const;

    /**
     * Traverse the scene graph with depth first search algorithm
     * using the visitor and starting point
     * @param visitor the visitor class that is executed per vertex
     * @param name name of the root node to start traversing.
     */
    void traverse( Visitor& visitor,
                   const std::string& name = ROOT_NODE );

private:

    struct Impl;
    std::unique_ptr<Impl> const _impl;
};

}

