#include "pcx.h"

const char pcx_info_01[] = "pcx.manufacturer = %x\n"
                           "pcx.version      = %x\n"
                           "pcx.endcoding    = %x\n"
                           "pcx.bpp          = %x\n"
                           "pcx.window       = %u %u %u %u\n"
                           "pcx.hdpi         = %u\n"
                           "pcx.vdpi         = %u\n"
                           "pcx.colormap     = ";
const char pcx_info_02[] = "pcx.reserved     = %u\n"
                           "pcx.nplanes      = %u\n"
                           "pcx.bpl          = %u\n"
                           "pcx.pallete_info = %u\n"
                           "pcx.hscreen_size = %u\n"
                           "pcx.vscreen_size = %u\n"
                           "pcx.filler       = ";

void pcx_info(pcx_fmt_t *h) {
    uint8_t i;

    printf(pcx_info_01, h->manufacturer, h->version, h->encoding, h->bpp, h->xmin, h->ymin, h->xmax, h->ymax, h->hdpi,
           h->vdpi);
    for (i = 0; i < 48; i++) {
        printf("%x", h->colormap[i]);
    }
    putchar('\n');
    printf(pcx_info_02, h->reserved, h->nplanes, h->bpl, h->pallete_info, h->hscreen_size, h->vscreen_size);
    for (i = 0; i < 54; i++) {
        printf("%x", h->filler[i]);
    }
    putchar('\n');
}

int8_t pcx_is_correct(pcx_fmt_t *h) {
    if (h->manufacturer != 0x0A) {
        return STATUS_IMG_INCORRECT;
    }
    return STATUS_SUCCESS;
}

int8_t pcx_load(FILE *f, image_t *img) {
    uint32_t xsize, ysize, total_bytes;
    uint8_t *buffer, *data, count;
    size_t file_size, i, n = 0;
    uint8_t plane;
    pcx_fmt_t h;

    memset(&h, 0, sizeof(pcx_fmt_t));
    fread(&h, sizeof(pcx_fmt_t), 1, f);
    if (pcx_is_correct(&h) == STATUS_IMG_INCORRECT) {
        fseek(f, 0, SEEK_SET);
        return STATUS_IMG_INCORRECT;
    }
    if (__DEBUG_FLAG__) {
        pcx_info(&h);
    }
    xsize = h.xmax - h.xmin + 1;
    ysize = h.ymax - h.ymin + 1;
    total_bytes = h.nplanes * h.bpl;
    plane = h.nplanes;
    file_size = fsize(f) - sizeof(pcx_fmt_t);
    buffer = (uint8_t *)malloc(file_size * sizeof(uint8_t));
    fread(buffer, sizeof(uint8_t), file_size, f);
    data = (uint8_t *)malloc(total_bytes * ysize * sizeof(uint8_t));
    while (n < total_bytes * ysize) {
        // 2 high bytes == 11
        if ((*buffer & 0xC0) == 0xC0) {
            // get last 6 bytes
            count = *buffer & 0x3F;
            buffer++;
        } else {
            count = 1;
        }
        for (i = 0; i < count; i++) {
            data[i + n] = *buffer;
        }
        n += count;
        buffer++;
    }
    buffer = (uint8_t *)malloc(total_bytes * ysize * sizeof(uint8_t));
    img->data = buffer;
    img->bpp = plane * h.bpp / 8;
    img->width = xsize;
    img->height = ysize;
    /* support RGB & RGBA color */
    switch (plane) {
        case 0: // mono image
            /* not supported */
            img_module_error("pcx mono image not supported!");
            img->data = data;
            break;
        case 3: // rgb image
            for (n = 0; n < ysize; n++) {
                for (i = 0; i < xsize; i++) {
                    buffer[plane * (i + xsize * (ysize - n - 1)) + 0] = data[i + (0 + plane * n) * xsize];
                    buffer[plane * (i + xsize * (ysize - n - 1)) + 1] = data[i + (1 + plane * n) * xsize];
                    buffer[plane * (i + xsize * (ysize - n - 1)) + 2] = data[i + (2 + plane * n) * xsize];
                }
            }
            img->c_format = IMG_RGB;
            img->data = buffer;
            break;
        case 4: // rgba image
            for (n = 0; n < ysize; n++) {
                for (i = 0; i < xsize; i++) {
                    buffer[plane * (i + xsize * (ysize - n - 1)) + 0] = data[i + (0 + plane * n) * xsize];
                    buffer[plane * (i + xsize * (ysize - n - 1)) + 1] = data[i + (1 + plane * n) * xsize];
                    buffer[plane * (i + xsize * (ysize - n - 1)) + 2] = data[i + (2 + plane * n) * xsize];
                    buffer[plane * (i + xsize * (ysize - n - 1)) + 3] = data[i + (3 + plane * n) * xsize];
                }
            }
            img->c_format = IMG_RGBA;
            img->data = buffer;
            break;
        default:
            break;
    }
    return STATUS_SUCCESS;
}

int8_t pcx_save(FILE *f, image_t *img) {
    pcx_fmt_t h;

    memset(&h, 0, sizeof(pcx_fmt_t));
    h.manufacturer = 0x0A;
    h.version = 5;
    h.encoding = 1;
    h.bpp = img->bpp;
    h.xmax = img->width - 1;
    h.ymax = img->height - 1;
    h.bpl = img->width;
    h.pallete_info = 1;
    /* input code */
    return STATUS_SUCCESS;
}