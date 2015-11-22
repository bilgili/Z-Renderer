# Copyright (c) ZombieRendering 2011-2015 ahmetbilgili@gmail.com
#                                        serkan.ergun@gmail.com

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(Z-Renderer)
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
common_package(Boost REQUIRED COMPONENTS unit_test_framework program_options)
common_package_post()

add_subdirectory(tests)

include(CPackConfig)
include(DoxygenRule)
