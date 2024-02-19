#ifndef __MAILBOX_H
#define __MAILBOX_H

#define PROPERTY_CHANNEL 8
#define FRAMEBUFFER_CHANNEL 1

#define MAILBOX_BASE (PERIPHERAL_BASE + MAILBOX_OFFSET)

#define MAIL0_READ      \
    (((mail_message_t *)(0x00 + MAILBOX_BASE)))

#define MAIL0_STATUS    \
    (((mail_status_t *)(0x18 + MAILBOX_BASE)))

#define MAIL0_WRITE     \
    (((mail_message_t *)(0x20 + MAILBOX_BASE)))

#define MAILBOX_REQUEST             0x00000000
#define MAILBOX_RESPONSE_SUCCESS    0x80000000
#define MAILBOX_RESPONSE_ERROR      0x80000001

typedef enum {
    REQUEST             = MAILBOX_REQUEST,
    RESPONSE_SUCCESS    = MAILBOX_RESPONSE_SUCCESS,
    RESPONSE_ERROR      = MAILBOX_RESPONSE_ERROR
} buffer_req_res_code_t;

typedef struct {
    uint8_t channel:    4;
    uint32_t data:      28;
} mail_message_t;

typedef struct {
    uint32_t reserved:  30;
    uint8_t empty:      1;
    uint8_t full:       1;
} mail_status_t;

typedef struct {
    uint32_t size;
    buffer_req_res_code_t req_res_code;
    uint32_t tags[1];
} propertyMessageBuff_t;

typedef struct {
    property_tag_t proptag;
    value_buffer_t value_buffer;
} propertyMessageTag_t;

int send_messages(propertyMessageTag_t* tags);

#endif
