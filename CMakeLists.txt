# Copyright (c) ZombieRendering 2015-2016 ahmetbilgili@gmail.com
#                                         serkan.ergun@gmail.com

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(ZRenderer)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common)
include(GitExternal)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION_ABI 1)

set(ZRENDERER_DESCRIPTION "Out-of-core physically based GPU ray-tracing engine")
set(ZRENDERER_MAINTAINER "ZombieRendering <zombierendering@gmail.com>")
set(ZRENDERER_LICENSE LGPL )

include(Common)
common_package(Boost REQUIRED COMPONENTS unit_test_framework
                                         program_options
                                         thread
                                         graph)
common_package(FreeImage REQUIRED)
common_package(Eigen3 REQUIRED)
common_package_post()

# add_subdirectory(rbvh)
# add_subdirectory(apps)
add_subdirectory(zrenderer)
add_subdirectory(tests)

include(CPackConfig)
include(DoxygenRule)
