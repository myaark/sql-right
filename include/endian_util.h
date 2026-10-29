#pragma once
#include <stddef.h>
#include <stdint.h>
#include "status.h"

// le - little endian
// u32 - unsigned 32 bit integer

#define U32_SIZE 4 // 32 bit integer has 4 Bytes
#define U16_SIZE 2 // 16 bit integer has 2 Bytes
#define U64_SIZE 8 // 64 bit integer has 8 Bytes

status_t le_write_u32(uint8_t *buf, size_t buf_len, size_t offset, uint32_t v);
status_t le_read_u32(const uint8_t *buf, size_t buf_len, size_t offset, uint32_t *out);

status_t le_write_u16(uint8_t *buf, size_t buf_len, size_t offset, uint16_t v);
status_t le_read_u16(const uint8_t *buf, size_t buf_len, size_t offset, uint16_t *out);

status_t le_write_u64(uint8_t *buf, size_t buf_len, size_t offset, uint64_t v);
status_t le_read_u64(const uint8_t *buf, size_t buf_len, size_t offset, uint64_t *out);
