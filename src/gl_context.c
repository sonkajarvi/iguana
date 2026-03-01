/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <dlfcn.h>
#include <stdio.h>

#include <glad/gl.h>

#include <gui/window.h>

#include "internal.h"

static void *__GL_handle = NULL;

static const char *__GL_lib_names[] = {
    "libGL.so",
    "libGL.so.1",
    NULL
};

int GL_open_handle(void)
{
    int i;

    if (!__GL_handle) {
        for (i = 0; __GL_lib_names[i]; i++) {
            __GL_handle = dlopen(__GL_lib_names[i], RTLD_LAZY | RTLD_LOCAL);
            if (__GL_handle)
                break;
        }

        if (!__GL_handle) {
            printf(ERROR "GL: failed to open handle\n");
            return -1;
        }

        printf(INFO "GL: opened handle for %s\n", __GL_lib_names[i]);
    }

    return 0;
}

void GL_close_handle(void)
{
    if (__GL_handle)
        dlclose(__GL_handle);
}

GLADapiproc GL_get_proc_address(const char *name)
{
    return dlsym(__GL_handle, name);
}

int GL_load_functions(struct window *wnd)
{
    (void)wnd;
    int version;

    version = gladLoadGL(GL_get_proc_address);
    if (!version) {
        printf(ERROR "GL: failed to load functions\n");
        return -1;
    }

    printf(INFO "GL: loaded functions for version %d.%d\n",
        GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    printf(INFO "OpenGL device information:\n");
    printf(INFO "... vendor:         %s\n", glGetString(GL_VENDOR));
    printf(INFO "... renderer:       %s\n", glGetString(GL_RENDERER));
    printf(INFO "... GL version:     %s\n", glGetString(GL_VERSION));
    printf(INFO "... GLSL version:   %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    return 0;
}
