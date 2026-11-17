#pragma once
#include <stdint.h>

#define DB_MAGIC            "MYSQLDB\0"   /* 8 bytes */
#define DB_MAGIC_LEN        8
#define DB_FORMAT_VERSION   1u

#define HDR_OFF_MAGIC        0
#define HDR_OFF_VERSION      8
#define HDR_OFF_PAGE_SIZE   12
#define HDR_OFF_PAGE_COUNT  16
#define HDR_OFF_FREE_HEAD   20
#define HDR_OFF_FREE_COUNT  24
#define HDR_OFF_CATALOG     28
#define HDR_OFF_CHANGE_CTR  32
#define HDR_OFF_CHECKSUM    36
#define HDR_SIZE            40

typedef struct {
    uint32_t version;
    uint32_t page_size;
    uint32_t page_count;
    uint32_t freelist_head;
    uint32_t freelist_count;
    uint32_t catalog_root;
    uint32_t change_counter;
    uint32_t checksum;
} PagerHeader;

static uint32_t header_checksum(const uint8_t *buf);
status_t header_parse(const uint8_t *buf, size_t expected_page_size,
                      size_t file_size, PagerHeader *h);