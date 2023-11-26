#include "async.h"

#include <errno.h>

struct aiocb async_init(int fildes, volatile void *buf, size_t nbytes,
                        off_t offset) {
    return (struct aiocb){
        .aio_buf = buf,
        .aio_fildes = fildes,
        .aio_nbytes = nbytes,
        .aio_offset = offset,
    };
}

bool async_read(struct aiocb *aiocbp) { return aio_read(aiocbp) == 0; }

bool async_done(const struct aiocb *aiocbp) {
    return aio_error(aiocbp) != EINPROGRESS;
}

ssize_t async_okay(struct aiocb *aiocbp) { return aio_return(aiocbp); }
