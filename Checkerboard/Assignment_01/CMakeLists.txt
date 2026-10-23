cmake_minimum_required(VERSION 3.27)
project(Assignment_01)

set(CMAKE_CXX_STANDARD 17)

add_executable(Assignment_01 main.cpp ShaderUtil.h ShaderUtil.cpp)

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
target_link_libraries(Assignment_01 PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)
find_package(GLEW REQUIRED)
target_link_libraries(Assignment_01 PRIVATE GLEW::glew)

