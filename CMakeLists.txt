cmake_minimum_required(VERSION 3.13)
project(cpp_kask2)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(cpp_kask2 main.cpp crow.h storage.cpp storage.h)

target_link_libraries( cpp_kask2 ${Boost_LIBRARIES} )
