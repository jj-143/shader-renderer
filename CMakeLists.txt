cmake_minimum_required(VERSION 3.6.0)
project(shader-renderer VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(OpenGL REQUIRED)

file (GLOB src
  "src/*.h"
  "src/*.cpp"
  "src/*.c"
)

file(GLOB imgui_src
  "dependencies/imgui/*.h"
  "dependencies/imgui/*.cpp"
)

add_executable(
  ${PROJECT_NAME}
  ${src}
  ${imgui_src}
)

target_include_directories(${PROJECT_NAME} PRIVATE dependencies)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)