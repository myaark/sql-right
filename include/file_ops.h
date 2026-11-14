#pragma once
#include "status.h"

typedef enum { //todo move to a better place
    FILE_MODE_READ,        // O_RDONLY
    FILE_MODE_READWRITE,   // O_RDWR
    FILE_MODE_CREATE       // O_RDWR | O_CREAT
} file_mode_t;

status_t file_open(const char *filename, file_mode_t mode, int *fd_out);
status_t file_close(int fd);
