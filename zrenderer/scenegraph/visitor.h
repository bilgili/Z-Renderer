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

class Visitor
{
public:

    ~Visitor() {}
    virtual void onBegin( NodePtr& root UNUSED ) {}
    virtual void preVisit( NodePtr& node UNUSED ) {}
    virtual void visit( NodePtr& node ) = 0;
    virtual void postVisit( NodePtr& node UNUSED) {}
    virtual void onEnd( NodePtr& rootNode UNUSED) {}
}

}
