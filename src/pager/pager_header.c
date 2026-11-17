#include <unistd.h>
#include "pager_header.h"
#include "status.h"
#include "endian_util.h"

#define READ_FIELD(field, off)                                   \
    do {                                                         \
        status_t rc = le_read_u32(buf, HDR_SIZE, (off), &tmp.field); \
        if (rc != ST_OK) return rc;                              \
    } while (0)

static uint32_t header_checksum(const uint8_t *buf)
{
    uint32_t sum = 0;
    for (size_t i = 0; i < HDR_OFF_CHECKSUM; i++) {
        sum = sum * 31u + buf[i];
    }
    return sum;
}

status_t header_parse(const uint8_t *buf, size_t expected_page_size,
                      size_t file_size, PagerHeader *h)
{
    PagerHeader tmp;
    if (memcmp(buf + HDR_OFF_MAGIC, DB_MAGIC, DB_MAGIC_LEN) != 0)
        return ST_INVALID_FORMAT;

    READ_FIELD(version,        HDR_OFF_VERSION);
    READ_FIELD(page_size,      HDR_OFF_PAGE_SIZE);
    READ_FIELD(page_count,     HDR_OFF_PAGE_COUNT);
    READ_FIELD(freelist_head,  HDR_OFF_FREE_HEAD);
    READ_FIELD(freelist_count, HDR_OFF_FREE_COUNT);
    READ_FIELD(catalog_root,   HDR_OFF_CATALOG);
    READ_FIELD(change_counter, HDR_OFF_CHANGE_CTR);
    READ_FIELD(checksum,       HDR_OFF_CHECKSUM);

    if (h->version != DB_FORMAT_VERSION)
        return ST_INVALID_FORMAT;
    if (h->page_size != expected_page_size)
        return ST_BAD_PAGE_SIZE;
    if (h->checksum != header_checksum(buf))
        return ST_CORRUPT_PAGE_HEADER;

    // Sanity checks
    if (h->page_count == 0)                          return ST_CORRUPT_PAGE_HEADER;
    if ((uint64_t)h->page_count * h->page_size > file_size) return ST_CORRUPT_PAGE_HEADER;
    if (h->catalog_root >= h->page_count)            return ST_CORRUPT_PAGE_HEADER;
    if (h->freelist_head >= h->page_count)           return ST_CORRUPT_PAGE_HEADER; // if 0 means "none", allow 0 explicitly
    if (h->freelist_count >= h->page_count)          return ST_CORRUPT_PAGE_HEADER;

    return ST_OK;
}