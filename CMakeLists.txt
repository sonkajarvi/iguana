cmake_minimum_required(VERSION 3.20)

project(gui LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(gui
    "src/color.c"
    "src/div.c"
    "src/image.c"
    "src/renderer.c"
    "src/util.c"
    "src/window.c"

    # gl
    "src/gl/gl_context.c"
    "src/gl/gl_renderer.c"
    "src/gl/gl_renderer.h"
    "src/gl/gl_texture.c"
    "src/gl/glx_context.c"

    # x11
    "src/x11/x11_window.c"

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
    "PROJECT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\""
)
