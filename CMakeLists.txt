cmake_minimum_required(VERSION 3.5)

project(RT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenMP REQUIRED)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/toyota-supra.obj ${CMAKE_CURRENT_BINARY_DIR}/toyota-supra.obj COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/scene_param.txt ${CMAKE_CURRENT_BINARY_DIR}/scene_param.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/objects.txt ${CMAKE_CURRENT_BINARY_DIR}/objects.txt COPYONLY)
add_executable(RT main.cpp shape.h octotree.cpp octotree.h base.h base.cpp meshshape.h meshshape.cpp scene.cpp scene.h triangle.cpp triangle.h sphere.cpp sphere.h CImg.h)
target_link_libraries(RT PRIVATE OpenMP::OpenMP_CXX)
