#pragma once
#include "pager.h"
#include "status.h"
#include <stddef.h>
#include <stdint.h>

struct Pager
{
    int fd;           // file descriptor for the database file
    size_t page_size; // size of each page in bytes
};

typedef struct Pager Pager;

status_t pager_open(const char *path, size_t page_size, Pager **out);
status_t pager_close(Pager *pager);
status_t pager_read_page(Pager *pager, uint64_t page_number,
                         uint8_t *buffer);
status_t pager_write_page(Pager *pager, uint64_t page_number,
                          const uint8_t *buffer);
status_t pager_sync(Pager *pager);
status_t pager_truncate(Pager *pager, uint64_t page_count);
status_t pager_delete_page(Pager *pager, uint64_t page_number);