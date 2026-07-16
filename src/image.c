/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_FAILURE_STRINGS
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include <stb/stb_image.h>

#include <iguana/image.h>
#include <iguana/window.h>

struct igu_image *igu_image_load_from_file(const char *path)
{
    struct igu_image *image;
    int comp;

    image = malloc(sizeof(*image));
    if (!image)
        goto err;

    stbi_set_flip_vertically_on_load(true);

    image->data = stbi_load(path, &image->width, &image->height, &comp, 4);
    if (!image->data) {
        printf(ERROR "IMAGE: failed to open file: %s\n", path);
        goto err;
    }

    printf(INFO "IMAGE: loaded image: width %d, height %d\n", image->width,
           image->height);

    return image;

err:
    free(image);
    return NULL;
}

void igu_image_free(struct igu_image *image)
{
    if (!image)
        return;

    stbi_image_free(image->data);
    free(image);
}
