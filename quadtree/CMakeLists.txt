cmake_minimum_required(VERSION 3.28.1)
project(learn_alg VERSION 0.1.0)

set(CXX_STANDARD 23)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

include(../3rd/3rd.cmake)
include(../utils/utils.cmake)

set(SOURCE 
    main.cpp
)

add_executable(learn_alg ${SOURCE})
target_link_libraries(learn_alg 3rd::raylib 3rd::box2d Utils::Box2dDebug)

