cmake_minimum_required(VERSION 3.6.0)
project(shader-renderer VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


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

target_include_directories(${PROJECT_NAME}
  PRIVATE dependencies
  ${STAGED_INSTALL_PREFIX}/include
)

add_dependencies(${PROJECT_NAME} nfd_external FileWatch)

# Link
set(NFD_LIBRARIES nfd gtk-3 glib-2.0)

target_link_directories(${PROJECT_NAME} PRIVATE ${STAGED_INSTALL_PREFIX}/lib)

target_link_libraries(${PROJECT_NAME}
  glfw OpenGL::GL 
  ${NFD_LIBRARIES}
)

# Copy resources
file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR})
