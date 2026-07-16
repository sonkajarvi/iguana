/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_IMAGE_H
#define _IGUANA_IMAGE_H

/**
 * struct igu_image - Image structure
 */
struct igu_image {
    int width, height;
    unsigned char *data;
};

struct igu_image *igu_image_load_from_file(const char *path);
void igu_image_free(struct igu_image *image);

#endif /* !_IGUANA_IMAGE_H */
