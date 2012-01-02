#include <stdlib.h>

#include "readerthread.h"
#include "reader.h"
#include "pa_ringbuffer.h"

/** Private data for ReaderThread. */
typedef struct {
    Reader r;
    PaUtilRingBuffer rb;
    pthread_t thread;
    pthread_mutex_t lock;
    pthread_cond_t wait;
} ReaderThreadPrivate;

static void *readerthread_run(void *vrt);

bool readerthread_init(ReaderThread *rt)
{
    ReaderThreadPrivate *p = calloc(1, sizeof(ReaderThreadPrivate));
    if (!p) return false;

    rt->p = p;
    return true;
}

bool readerthread_start(ReaderThread *rt)
{
    ReaderThreadPrivate *p = rt->p;
    int err;
    if ((err = pthread_mutex_init(&p->lock, NULL))) {
        return false;
    }
    if ((err = pthread_cond_init(&p->wait, NULL))) {
        return false;
    }

    if ((err = pthread_create(&p->thread, NULL, readerthread_run, rt))) {
        return false;
    }
    return true;
}

void readerthread_join(ReaderThread *rt)
{
    ReaderThreadPrivate *p = rt->p;

    pthread_join(p->thread, NULL);
}

void readerthread_destroy(ReaderThread *rt)
{
    ReaderThreadPrivate *p = rt->p;
    pthread_mutex_destroy(&p->lock);
    pthread_cond_destroy(&p->wait);
}

/* static methods */
static void *readerthread_run(void *vrt)
{
    ReaderThread *rt = vrt;
    ReaderThreadPrivate *p = rt->p;
    (void) p;

    return NULL;
}
