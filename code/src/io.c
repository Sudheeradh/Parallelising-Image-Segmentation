#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "./stb_image.h"
#include "./stb_image_write.h"

#include "io.h"

byte_t *img_load(char *img_file, int *width, int *height, int *n_channels)
{
    byte_t *data;

    data = stbi_load(img_file, width, height, n_channels, 0);
    if (data == NULL) {
        fprintf(stderr, "ERROR : Invalid file name or format \n");
        exit(EXIT_FAILURE);
    }

    return data;
}

void img_save(char *img_file, byte_t *data, int width, int height, int n_channels)
{
    char *ext;

    ext = strrchr(img_file, '.');

    if (!ext) {
        fprintf(stderr, "ERROR: Unspecified format \n");
        return;
    }

    if ((strcmp(ext, ".jpeg") == 0) || (strcmp(ext, ".jpg") == 0)) {
        stbi_write_jpg(img_file, width, height, n_channels, data, 100);
    } else {
        fprintf(stderr, "ERROR: Unsupported format \n");
    }
}
