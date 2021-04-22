cmake_minimum_required(VERSION 3.5)

project(RT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RT main.cpp shape.h octotree.cpp octotree.h base.h base.cpp meshshape.h meshshape.cpp scene.cpp scene.h triangle.cpp triangle.h sphere.cpp sphere.h CImg.h)
