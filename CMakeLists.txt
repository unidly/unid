cmake_minimum_required(VERSION 3.22)

project(unid VERSION 1.0 LANGUAGES CXX)

# This project uses C++20
# Update CPP and CXX in the .bashrc to gcc14-gcc, gcc14-g++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(spdlog REQUIRED)

add_executable(unid
  ./src/main.cpp
)

target_include_directories(unid PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(unid PRIVATE spdlog::spdlog)
