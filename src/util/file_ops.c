#include <unistd.h>
#include <stdio.h>
#include <status.h>
#include <stdint.h>
#include <fcntl.h>
#include "constants.h"
#include <errno.h>

typedef enum { //todo move to a better place
    FILE_MODE_READ,        // O_RDONLY
    FILE_MODE_READWRITE,   // O_RDWR
    FILE_MODE_CREATE       // O_RDWR | O_CREAT
} file_mode_t;

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

status_t file_close(int fd){
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

status_t  file_read_page(const char *filename, uint32_t page_no, uint8_t *buf, ssize_t *bytes_read){
    int file_descriptor;
    status_t err = file_open(filename, FILE_MODE_READ, &file_descriptor);
    if(err != ST_OK)
    {
        return err;
    }

    /*may need to change this later to skip a few bytes if 
    always store some sort of meta data*/
    off_t offset = (off_t)page_no * PAGE_SIZE;  
    //buf (buffer) is the actuall memory location of the pages
    ssize_t n = pread(file_descriptor, buf, PAGE_SIZE, offset);
    err = file_close(file_descriptor);
    if(err != ST_OK)
    {
        return err;
    }


    if (bytes_read) {
        *bytes_read = n;
    }
    if (n != PAGE_SIZE) {   // covers error (-1), EOF (0), and partial reads
        return ST_PAGE_READ_INVALID;
    }

    return ST_OK;
}

status_t file_write_page(const char *filename, uint32_t page_no,
                         const uint8_t *buf){
    int file_descriptor;
    status_t err = file_open(filename, FILE_MODE_CREATE, &file_descriptor);
    if(err != ST_OK)
    {
        return err;
    }

    off_t offset = (off_t)page_no * PAGE_SIZE;
    ssize_t total = 0;
    while(total < PAGE_SIZE)
    {
        ssize_t n = pwrite(file_descriptor, buf + total, PAGE_SIZE - total, offset + total);
        if(n<0)
        {
            if(errno == EINTR) // handling system interrupts
            {
                continue; 
            }
            err = file_close(file_descriptor);
            if(err != ST_OK)
            {
                return err;
            }
        }
        if (n == 0) {                   // shouldn't happen for regular files
            break;
        }
        total += n;
    }
    // Make the write durable before reporting success
    if (fsync(file_descriptor) != 0) {
        err = file_close(file_descriptor);
        if(err != ST_OK)
        {
            return err;
        }
    }

    err = file_close(file_descriptor);
    if(err != ST_OK)
    {
        return err;
    }

    if (total != PAGE_SIZE) {
        return ST_PAGE_WRITE_FAILED;
    }
    return ST_OK;
}
