#include "file_ops.h"
#include <unistd.h>
#include <stdio.h>
#include "status.h"
#include <fcntl.h>

status_t file_open(const char *filename, file_mode_t mode, int *fd_out)
{
    int flags;
    switch (mode) {
        case FILE_MODE_READ:      flags = O_RDONLY;           break;
        case FILE_MODE_READWRITE: flags = O_RDWR;             break;
        case FILE_MODE_CREATE:    flags = O_RDWR | O_CREAT;   break;
        default: 
            return ST_INVALID_ARG;
    }

    int fd = open(filename, flags, 0644);
    if (fd < 0) return ST_FILE_OPEN_FAILED;

    *fd_out = fd;
    return ST_OK;
}

status_t file_close(int fd)
{
    if(fd<0)
    {
        return ST_INVALID_ARG;
    }
    int err = close(fd);
    if (err != 0) {
        perror("Error closing file");
        return ST_IO_ERROR;
    }

    return ST_OK;
}

