cmake_minimum_required(VERSION 3.25.0)

project(tinypt)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")


file(GLOB TINYPT_SRC
     "src/*.hpp"
     "src/*.cpp"
)

add_executable(tinypt ${TINYPT_SRC})
