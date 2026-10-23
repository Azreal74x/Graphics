cmake_minimum_required(VERSION 3.27)
project(MGE_CLion)

set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE SOURCES
        mge/*.h
        mge/*.hpp
        mge/*.cpp
)

add_executable(MGE_CLion ${SOURCE_FILES} ${SOURCES}
        glm.hpp main.cpp
        mge/MGE2.cpp
        mge/core/AbstractGame.cpp
        mge/core/Camera.cpp
        mge/core/GameObject.cpp
        mge/behaviours/AbstractBehaviour.cpp
        mge/behaviours/KeysBehaviour.cpp
        mge/behaviours/RotatingBehaviour.cpp
        mge/core/Light.cpp
        mge/core/Mesh.cpp
        mge/core/Renderer.cpp
        mge/core/ShaderProgram.cpp
        mge/core/Texture.cpp
        mge/core/World.cpp
        mge/behaviours/FollowCameraBehaviour.hpp
        mge/behaviours/FollowCameraBehaviour.cpp
        mge/materials/TerrainMaterial.hpp
        mge/materials/TerrainMaterial.cpp)


find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
target_link_libraries(MGE_CLion PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)
find_package(GLEW REQUIRED)
target_link_libraries(MGE_CLion PRIVATE GLEW::GLEW)