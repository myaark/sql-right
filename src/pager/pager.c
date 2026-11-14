#include <unistd.h>
#include <status.h>
#include <stdint.h>
#include <errno.h>
#include "constants.h"
#include "file_ops.h"
#include "pager.h"


status_t  pager_read_page(Pager *pager, uint64_t page_no,
                         uint8_t *buf){
    int file_descriptor = pager->fd;

    /*may need to change this later to skip a few bytes if 
    always store some sort of meta data*/
    off_t offset = (off_t)page_no * PAGE_SIZE;  
    //buf (buffer) is the actuall memory location of the pages
    ssize_t n = pread(file_descriptor, buf, PAGE_SIZE, offset);
    status_t err = file_close(file_descriptor);
    if(err != ST_OK)
    {
        return err;
    }

    if (n != PAGE_SIZE) {   // covers error (-1), EOF (0), and partial reads
        return ST_PAGE_READ_INVALID;
    }

    return ST_OK;
}

status_t pager_write_page(Pager *pager, uint64_t page_no,
                         uint8_t *buf){
    int file_descriptor = pager->fd;
    status_t err = ST_OK;

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
    err = pager_sync(pager);
    if(err != ST_OK)
    {
        return err;
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


status_t pager_sync(Pager *pager)
{
    if (pager == NULL) {
        return ST_INVALID_ARG;
    }
    int rc;
    do {
        rc = fsync(pager->fd);
    } while (rc != 0 && errno == EINTR); // only retry in case of EINTR i.e interrupted system call

    if (rc != 0) {
        return ST_IO_ERROR;
    }
    return ST_OK;
}
