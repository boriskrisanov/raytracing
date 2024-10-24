cmake_minimum_required(VERSION 3.29)
project(ray_tracing)

set(CMAKE_CXX_STANDARD 23)

add_executable(ray_tracing src/main.cpp
        src/math.hpp
        src/Ray.hpp
        Sphere.cpp
        Sphere.hpp)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(ray_tracing
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(ray_tracing PRIVATE imgui::imgui)
