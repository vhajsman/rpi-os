#include "framebuffer.h"

void gpu_putc(char c) {
    static const pixel_t WHITE = {0xff, 0xff, 0xff};
    static const pixel_t BLACK = {0x00, 0x00, 0x00};

    uint8_t w, h;
    uint8_t mask;

    const uint8_t * bitmap = font(c);

    uint32_t i;
    uint32_t num_rows = fbinfo.height / CHAR_HEIGHT;

    if (fbinfo.chars_y >= num_rows) {
        for (i = 0; i < num_rows - 1; i++)
            memcpy( fbinfo.buf + fbinfo.pitch * i * CHAR_HEIGHT, 
                    fbinfo.buf + fbinfo.pitch * (i+1) * CHAR_HEIGHT, 
                    fbinfo.pitch * CHAR_HEIGHT);

        bzero(fbinfo.buf + fbinfo.pitch * i * CHAR_HEIGHT,fbinfo.pitch * CHAR_HEIGHT);
        fbinfo.chars_y--;
    }

    if (c == '\n') {
        fbinfo.chars_x = 0;
        fbinfo.chars_y++;

        return;
    }

    for(w = 0; w < CHAR_WIDTH; w++) {
        for(h = 0; h < CHAR_HEIGHT; h++) {
            mask = 1 << (w);
            writePixel(fbinfo.chars_x * CHAR_WIDTH + w, fbinfo.chars_y * CHAR_HEIGHT + h, bitmap[h] & mask ? &WHITE : &BLACK);
        }
    }

    fbinfo.chars_x++;

    if (fbinfo.chars_x > fbinfo.chars_width) {
        fbinfo.chars_x = 0;
        fbinfo.chars_y++;
    }
}
