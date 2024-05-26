cmake_minimum_required(VERSION 3.0)

# set the project name
project(jsearch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

# add the executable
add_executable(jsearch utils.cpp jsearch_app.cpp)
