file(GLOB imgui_src *.cpp)

add_library(imgui STATIC ${imgui_src})

target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

target_compile_options(imgui PUBLIC -Wno-nontrivial-memcall)

target_link_libraries(${PROJECT_NAME} imgui)
