# Copyright (c) ZombieRendering 2015-2016 ahmetbilgili@gmail.com

set(ZSCENEGRAPH_PUBLIC_HEADERS types.h scenegraph.h node.h visitor.h nodedata.h)
set(ZSCENEGRAPH_SOURCES scenegraph.cpp node.cpp)
set(ZSCENEGRAPH_LINK_LIBRARIES PRIVATE ${Boost_GRAPH_LIBRARY})

common_library(zscenegraph)
