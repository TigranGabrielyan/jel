CMAKE_MINIMUM_REQUIRED(VERSION 3.18)

SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_CXX_COMPILER "g++")

PROJECT("json_encoder")

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(external/yajl-2.1.1/include)
INCLUDE_DIRECTORIES(external/yajl-2.1.1/include/yajl)
INCLUDE_DIRECTORIES(external/apr/include/apr-1)

LINK_DIRECTORIES(external/yajl-2.1.1/lib)
LINK_DIRECTORIES(external/apr/lib)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test/sample)
ADD_SUBDIRECTORY(test/gtest)