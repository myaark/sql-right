#pragma once
#include <stddef.h>
#include <stdint.h>
#include "status.h"

// le - little endian
// u32 - unsigned 32 bit integer

#define U32_SIZE 4 // 32 bit integer has 4 Bytes

status_t le_write_u32(uint8_t *buf, size_t buf_len, size_t offset, uint32_t v);
status_t le_read_32(const uint8_t *buf, size_t buf_len, size_t offset, uint32_t *out);