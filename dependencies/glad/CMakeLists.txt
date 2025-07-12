add_library(glad STATIC src/gl.c)

target_include_directories(glad PRIVATE include)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} glad)