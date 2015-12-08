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
 * This class is the visitor interface for scenegraph
 * traversal.
 */
class Visitor
{
public:

    virtual ~Visitor() {}

    /**
     * Executed at the beginning of the traversal
     * @param scenegraph is the traversed scene graph
     */
    virtual void onBegin( const SceneGraph& scenegraph UNUSED ) {}

    /**
     * Executed while visiting a node
     * @param scenegraph is the traversed scene graph
     */
    virtual void visit( const SceneGraph& scenegraph,
                        NodePtr node ) = 0;

    /**
     * Executed at the end of the traversal
     * @param scenegraph is the traversed scene graph
     */
    virtual void onEnd( const SceneGraph& scenegraph UNUSED ) {}
};

}
