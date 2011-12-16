#include <stdlib.h>

#include "readerthread.h"
#include "pa_ringbuffer.h"

typedef struct {
    PaUtilRingBuffer *rb;
    pthread_t *thread;
    pthread_cond_t *wait;
} ReaderThreadPrivate;

bool create_readerthread(ReaderThread **rt)
{
    *rt = calloc(1, sizeof(ReaderThread));
    ReaderThreadPrivate *p = calloc(1, sizeof(ReaderThreadPrivate));
    (*rt)->p = p;
    return true;
}
