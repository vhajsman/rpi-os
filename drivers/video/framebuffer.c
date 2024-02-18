#include "framebuffer.h"

static uint32_t get_value_buffer_len(property_message_tag_t * tag) {
    switch(tag->proptag) {
        case FB_ALLOCATE_BUFFER: 
        case FB_GET_PHYSICAL_DIMENSIONS:
        case FB_SET_PHYSICAL_DIMENSIONS:
        case FB_GET_VIRTUAL_DIMENSIONS:
        case FB_SET_VIRTUAL_DIMENSIONS:
            return 8;

        case FB_GET_BITS_PER_PIXEL:
        case FB_SET_BITS_PER_PIXEL:
        case FB_GET_BYTES_PER_ROW:
            return 4;

        case FB_RELESE_BUFFER:
        default:
            return 0;
    }
}

int fb_sendMessages(property_message_tag_t* tags) {
    property_message_buffer_t* msg;
    mbox mail;

    uint32_t bufsize = 0, i, len, bufpos;
   
    for (i = 0; tags[i].proptag != NULL_TAG; i++)
        bufsize += get_value_buffer_len(&tags[i]) + 3 * sizeof(uint32_t);

    bufsize += 3 * sizeof(uint32_t);
    bufsize += (bufsize % 16) ? 16 - (bufsize % 16) : 0;

    msg = kmalloc(bufsize);
    if (!msg)
        return -1;

    msg->size = bufsize;
    msg->req_res_code = REQUEST;

    for (i = 0, bufpos = 0; tags[i].proptag != NULL_TAG; i++) {
        len = get_value_buffer_len(&tags[i]);

        msg->tags[bufpos++] = tags[i].proptag;
        msg->tags[bufpos++] = len;
        msg->tags[bufpos++] = 0;

        memcpy(msg->tags+bufpos, &tags[i].value_buffer, len);

        bufpos += len/4;
    }

    msg->tags[bufpos] = 0;

    mail.data = ((uint32_t)msg) >>4;
    
    mailbox_send(mail, PROPERTY_CHANNEL);
    mail = mailbox_read(PROPERTY_CHANNEL);

    if (msg->req_res_code == REQUEST) {
        kfree(msg);
        return 1;
    }

    if (msg->req_res_code == RESPONSE_ERROR) {
        kfree(msg);
        return 2;
    }

    for (i = 0, bufpos = 0; tags[i].proptag != NULL_TAG; i++) {
        len = get_value_buffer_len(&tags[i]);
        bufpos += 3;

        memcpy(&tags[i].value_buffer, msg->tags+bufpos,len);

        bufpos += len/4;
    }

    kfree(msg);
    return 0;
}

int fb_init(void) {
    property_message_tag_t tags[5];

    tags[0].proptag = FB_SET_PHYSICAL_DIMENSIONS;
    tags[0].value_buffer.fb_screen_size.width = 640;
    tags[0].value_buffer.fb_screen_size.height = 480;
    tags[1].proptag = FB_SET_VIRTUAL_DIMENSIONS;
    tags[1].value_buffer.fb_screen_size.width = 640;
    tags[1].value_buffer.fb_screen_size.height = 480;
    tags[2].proptag = FB_SET_BITS_PER_PIXEL;
    tags[2].value_buffer.fb_bits_per_pixel = COLORDEPTH;
    tags[3].proptag = NULL_TAG;

    if (fb_sendMessages(tags) != 0)
        return -1;

    fbinfo.width =          tags[0].value_buffer.fb_screen_size.width;
    fbinfo.height =         tags[0].value_buffer.fb_screen_size.height;
    fbinfo.chars_width =    fbinfo.width / CHAR_WIDTH;
    fbinfo.chars_height =   fbinfo.height / CHAR_HEIGHT;
    fbinfo.chars_x =        0;
    fbinfo.chars_y =        0;
    fbinfo.pitch =          fbinfo.width*BYTES_PER_PIXEL;

    tags[0].proptag =                               FB_ALLOCATE_BUFFER;
    tags[0].value_buffer.fb_screen_size.width =     0;
    tags[0].value_buffer.fb_screen_size.height =    0;
    tags[0].value_buffer.fb_allocate_align =        16;
    tags[1].proptag =                               NULL_TAG;

    if (fb_sendMessages(tags) != 0)
        return -1;

    fbinfo.buf =        tags[0].value_buffer.fb_allocate_res.fb_addr;
    fbinfo.buf_size =   tags[0].value_buffer.fb_allocate_res.fb_size;

    return 0;
}

void writePixel(uint32_t x, uint32_t y, const pixel_t* pix) {
    uint8_t* location = fbinfo.buf + y * fbinfo.pitch + x * BYTES_PER_PIXEL;
    memcpy(location, pix, BYTES_PER_PIXEL);
}

