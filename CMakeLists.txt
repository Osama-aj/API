cmake_minimum_required(VERSION 3.14)
project(API)

set(CMAKE_CXX_STANDARD 17)

add_executable(API main.cpp cUrlAndJsoncpp.cpp cUrlAndJsoncpp.h)

target_link_libraries(API LINK_PUBLIC curl jsoncpp )

