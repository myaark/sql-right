#include "endian_util.h"
#include "checked.h"

status_t le_write_u32(uint8_t *buf, size_t buf_len, size_t offset, uint32_t v)
{
    if (buf == NULL)
        return ST_INVALID_ARG;
    size_t end;
    status_t st = checked_add(offset, U32_SIZE, &end);
    if (st != ST_OK)
        return st;
    if (end > buf_len)
        return ST_SIZE_OVERFLOW;

    // Converting a 32-bit integer v into a byte buffer in little-endian order
    //  pick the nth byte and into index of the buffer
    buf[offset + 0] = (uint8_t)(v);
    buf[offset + 1] = (uint8_t)(v >> 8);
    buf[offset + 2] = (uint8_t)(v >> 16);
    buf[offset + 3] = (uint8_t)(v >> 24);
    return ST_OK;
}

status_t le_read_32(const uint8_t *buf, size_t buf_len, size_t offset, uint32_t *out)
{
    if (buf == NULL || out == NULL)
    {
        return ST_INVALID_ARG;
    }

    size_t end;
    status_t st = checked_add(offset, U32_SIZE, &end);
    if (st != ST_OK)
        return st;

    if (end > buf_len)
        return ST_SIZE_OVERFLOW;

    // combine seperate groups of 8 bit integers in a single group of one(32 bit)
    *out = (uint32_t)buf[offset + 0] | ((uint32_t)buf[offset + 1] << 8) | ((uint32_t)buf[offset + 2] << 16) | ((uint32_t)buf[offset + 3] << 24);

    return ST_OK;
}