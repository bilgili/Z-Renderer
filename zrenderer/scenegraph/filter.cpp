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

#include <zrenderer/scenegraph/filter.h>

namespace zrenderer
{

struct Filter::Impl
{
public:

    void compare( const FilterProperties& filter ) const
    {}

    void addFilterDefinition( FilterLogic filterLogic,
                              const FilterDefinition& filterDef )
    {}

    void addFilter( FilterLogic logic,
                    const Filter& filter )
    {}

    FilterProperties _requestedProperties;
    Filter _requestedFilters;
};

void Filter::compare( const FilterProperties& filter ) const
{}

void Filter::addFilterDefinition( FilterLogic filterLogic,
                                  const FilterDefinition& filterDef )
{}

void Filter::addFilter( FilterLogic logic,
                        const Filter& filter )
{}



}
