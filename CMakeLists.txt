cmake_minimum_required(VERSION 3.13.0)
project(shader-renderer VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/output)

find_package(OpenGL REQUIRED)

file (GLOB_RECURSE src
  "src/*.cpp"
  "src/*.c"
)

file(GLOB imgui_src
  "dependencies/imgui/*.cpp"
)

add_executable(
  ${PROJECT_NAME}
  ${src}
  ${imgui_src}
)

# 3rd party
set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage)
set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/subprojects)
add_subdirectory(thirdparty)

# Includes
target_include_directories(${PROJECT_NAME}
  PRIVATE dependencies
  ${STAGED_INSTALL_PREFIX}/include
)

# Links
target_link_directories(${PROJECT_NAME} PRIVATE ${STAGED_INSTALL_PREFIX}/lib)

if (WIN32)
  SET(OPENGL_LIBS glfw3 OpenGL::GL) # glfw3 is for static linking
elseif(UNIX)
  SET(OPENGL_LIBS glfw OpenGL::GL)
endif()

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBS})

# Copy resources
file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
