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

status_t le_read_u32(const uint8_t *buf, size_t buf_len, size_t offset, uint32_t *out)
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

status_t le_write_u16(uint8_t *buf, size_t buf_len, size_t offset, uint16_t v)
{
    if (buf == NULL)
    {
        return ST_INVALID_ARG;
    }
    size_t end;
    status_t st = checked_add(offset, U16_SIZE, &end);
    if (st != ST_OK)
        return st;
    if (end > buf_len)
        return ST_SIZE_OVERFLOW;
    // Converting a 16-bit integer v into a byte buffer in little-endian order
    buf[offset + 0] = (uint8_t)(v);
    buf[offset + 1] = (uint8_t)(v >> 8);
    return ST_OK;
}

status_t le_read_u16(const uint8_t *buf, size_t buf_len, size_t offset, uint16_t *out)
{
    if (buf == NULL || out == NULL)
    {
        return ST_INVALID_ARG;
    }

    size_t end;
    status_t st = checked_add(offset, U16_SIZE, &end);

    if (st != ST_OK)
    {
        return st;
    }

    if (end > buf_len)
    {
        return ST_SIZE_OVERFLOW;
    }
    *out = (uint16_t)buf[offset + 0] | ((uint16_t)buf[offset + 8] << 8) | ((uint16_t)buf[offset + 16] << 16) | ((uint16_t)buf[offset + 24] << 24);

    return ST_OK;
}

status_t le_write_u64(uint8_t *buf, size_t buf_len, size_t offset, uint64_t v)
{
    if (buf == NULL)
        return ST_INVALID_ARG;
    size_t end;
    status_t st = checked_add(offset, U64_SIZE, &end);
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
    buf[offset + 4] = (uint8_t)(v >> 32);
    buf[offset + 5] = (uint8_t)(v >> 40);
    buf[offset + 6] = (uint8_t)(v >> 48);
    buf[offset + 7] = (uint8_t)(v >> 56);
    return ST_OK;
}

status_t le_read_u64(const uint8_t *buf, size_t buf_len, size_t offset, uint64_t *out)
{
    if (buf == NULL || out == NULL)
    {
        return ST_INVALID_ARG;
    }

    size_t end;
    status_t st = checked_add(offset, U64_SIZE, &end);
    if (st != ST_OK)
        return st;

    if (end > buf_len)
        return ST_SIZE_OVERFLOW;

    // combine seperate groups of 8 bit integers in a single group of one(64 bit)
    *out = (uint64_t)buf[offset + 0] | ((uint64_t)buf[offset + 1] << 8) | ((uint64_t)buf[offset + 2] << 16) | ((uint64_t)buf[offset + 3] << 24) |
           ((uint64_t)buf[offset + 4] << 32) | ((uint64_t)buf[offset + 5] << 40) | ((uint64_t)buf[offset + 6] << 48) | ((uint64_t)buf[offset + 7] << 56);

    return ST_OK;
}