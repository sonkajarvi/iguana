cmake_minimum_required(VERSION 3.20)

project(gui LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(gui
    "src/div.c"
    "src/gl_context.c"
    "src/gl_renderer.c"
    "src/glx_context.c"
    "src/renderer.c"
    "src/util.c"
    "src/window.c"
    "src/x11_window.c"

    # glad sources
    "third_party/glad/src/gl.c"
    "third_party/glad/src/glx.c"
)

target_include_directories(gui PRIVATE
    "."
    "third_party"
    "third_party/cglm/include"
    "third_party/glad/include"
)

target_link_libraries(gui PRIVATE
    "X11"
)

target_compile_options(gui PRIVATE
    "-Wall"
    "-Werror"
    "-Wextra"
    "-Wstrict-prototypes"
)

target_compile_definitions(gui PRIVATE
    "RESOURCES_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/resources\""
)
