cmake_minimum_required(VERSION 3.13.0)
project(shader-renderer VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/output)

add_compile_options(-fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=)

find_package(OpenGL REQUIRED)

file (GLOB_RECURSE src "src/*.cpp")

set(inc
  "src/app"
  "src/core"
  "src/node"
  "src/nodes"
  "src/renderer"
  "src/ui"
  "src"
)

add_executable(
  ${PROJECT_NAME}
  ${src}
)

# Common dependencies
add_subdirectory(dependencies)

# 3rd party
set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage)
set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/subprojects)
add_subdirectory(thirdparty)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE ${inc})

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

# Copy "project" folder - includes default shader example 
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
