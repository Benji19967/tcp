cmake_minimum_required(VERSION 3.15)
project(tcpserver)

set(CMAKE_CXX_STANDARD 17)

link_libraries(ws2_32 wsock32)

add_executable(tcpserver main.cpp)