cmake_minimum_required(VERSION 3.10.0)
project(transport_catalogue VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(transport_catalogue input_reader.cpp main.cpp stat_reader.cpp transport_catalogue.cpp)

include(CTest)
enable_testing()

