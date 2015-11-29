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

#ifndef _filter_h_
#define _filter_h_

#include <zrenderer/scenegraph/types.h>

#include <boost/any.hpp>

namespace zrenderer
{

/**
 * This class holds a name, variable value
 * pair. The value should implement the
 * properties needed for the boost::any
 */
class FilterProperty
{
public:

    template<class T>
    FilterProperty( const std::string& name_,
                    const T& value_ )
        : name( name_ )
        , value( value_ )
    {}

    std::string name;
    boost::any value;
};

typedef std::vector<FilterProperty> FilterProperties;

enum FilterLogic
{
    FL_AND,
    FL_OR,
    FL_NOT
};

typedef std::pair<FilterLogic, FilterProperty> FilterDefinition;

/**
 * This class holds a filter. This filter is able to compare
 * itself against the given properties.
 */
class Filter
{
public:

    /**
     * Compares the filter against the given list of properties.
     * @param properties is the list of properties to compare.
     * @return true if filter satisfies the conditions. If compare
     * result is not satisfied or properties covered by filter
     * is not covered by given list of properties it will be a false.
     */
    bool compare( const FilterProperties& properties ) const;

    /**
     * Adds a property condition to the filter
     * @param filterLogic is applied to the property
     * while comparing.
     * @param filterDef is the property definition to compare with a
     * value.
     */
    void addFilterDefinition( FilterLogic filterLogic,
                              const FilterDefinition& filterDef );

    /**
     * Adds a sub filter as a group of properties.
     * @param filterLogic is applied to the filter
     * while comparing.
     * @param filter is the sub filter for comaprision
     */
    void addFilter( FilterLogic logic,
                    const Filter& filter );
private:

    struct Impl;
    std::unique_ptr<Impl> _impl;
};

}

#endif // _filter_h_
