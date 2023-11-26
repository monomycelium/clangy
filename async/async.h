
#ifndef ASYNC_H
#define ASYNC_H

#include <aio.h>
#include <stdbool.h>

struct aiocb async_init(int fildes, volatile void *buf, size_t nbytes,
                        off_t offset);
bool async_read(struct aiocb *aiocbp);
bool async_done(const struct aiocb *aiocbp);
ssize_t async_okay(struct aiocb *aiocbp);

#endif
