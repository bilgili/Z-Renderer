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

#ifndef _scenegraph_types_
#define _scenegraph_types_

#include <zrenderer/common/types.h>

namespace zrenderer
{

class GeometryData;
class Node;
class NodeData;
class SceneGraph;
class Visitor;

typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<const Node> ConstNodePtr;
typedef std::shared_ptr<NodeData> NodeDataPtr;
typedef std::shared_ptr<const NodeData> ConstNodeDataPtr;
typedef std::shared_ptr<GeometryData> GeometryDataPtr;

typedef std::vector<NodePtr> NodePtrs;
typedef std::vector<ConstNodePtr> ConstNodePtrs;

const std::string ROOT_NODE = "RootNode";

}

#endif // _scenegraph_types_
