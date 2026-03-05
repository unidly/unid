cmake_minimum_required(VERSION 4.2.3)

project(unid VERSION 1.0 LANGUAGES CXX)

# This project requires C++20
# Update CPP and CXX in the .bashrc to gcc14-gcc, gcc14-g++ for C++220 support
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(server)
