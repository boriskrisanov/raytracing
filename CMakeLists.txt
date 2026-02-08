cmake_minimum_required(VERSION 3.29)
project(ray_tracing)

set(CMAKE_CXX_STANDARD 23)

add_executable(ray_tracing src/main.cpp
        src/math.hpp
        src/Ray.hpp
        src/Sphere.cpp
        src/Sphere.hpp
        src/SceneObject.cpp
        src/SceneObject.hpp
        src/random.hpp
        src/random.cpp
        src/Material.hpp
        src/Diffuse.hpp
        src/Diffuse.cpp
        src/Reflective.cpp
        src/Reflective.hpp
        src/Emissive.cpp
        src/Emissive.hpp
        src/UI.cpp
        src/UI.hpp
        src/Renderer.cpp
        src/Renderer.hpp
        src/Scene.cpp
        src/Scene.hpp
        src/Camera.cpp
        src/Camera.hpp
        src/PixelBuffer.cpp
        src/PixelBuffer.hpp
        src/Triangle.cpp
        src/Triangle.hpp
        src/Mesh.cpp
        src/Mesh.hpp
)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(ray_tracing
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(ray_tracing PRIVATE imgui::imgui)
