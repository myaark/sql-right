#pragma once
typedef enum
{
    ST_OK = 0,
    ST_INVALID_ARG = 1,
    ST_SIZE_OVERFLOW = 2,
    /* A valid range does not fit in the provided buffer. */
    ST_BUFFER_TOO_SMALL,

    /* A file or operating-system operation failed. */
    ST_IO_ERROR,

    /* A database file contains invalid or unsupported data. */
    ST_INVALID_FORMAT,

    /* A required allocation failed. */
    ST_OUT_OF_MEMORY,
} status_t;

// TODO : MAKE THIS INTO DIFFERENT SECTIONS OF ERROR SERIES
// NOT JUST SEQUENTIALLY DEFINNING 1 2 3 ...
// E.G MEMORY ERRORS, CONNECTION ERROR ETC, GENERIC ERRORS
