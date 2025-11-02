cmake_minimum_required(VERSION 3.11)
set(project_name cpu)

project(${project_name})

#library
file(GLOB srcs_lib "utilities/*.cpp")
file(GLOB hdrs_lib "utilities/*.hpp")
add_library(utilities_lib STATIC ${srcs_lib} ${hdrs_lib})

#main part
file(GLOB srcs "cpu/src/*.cpp")
file(GLOB hdrs "cpu/include/*.cpp")
add_executable(${project_name} ${srcs} ${hdrs})

target_link_libraries(${project_name} PUBLIC utilities_lib)