#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

    rt->running = false;
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

void readerthread_wake(ReaderThread *rt)
{
    ReaderThreadPrivate *p = rt->p;

    pthread_cond_signal(&p->wait);
}

/* static methods */
static void *readerthread_run(void *vrt)
{
    ReaderThread *rt = vrt;
    ReaderThreadPrivate *p = rt->p;
    rt->running = true;

    do {
        printf("running\n");
        size_t items_needed = 0;
        while ((items_needed = PaUtil_GetRingBufferWriteAvailable(rt->rb))) {
            printf("need %zd items\n", items_needed);
            if (!rt->r->n) {
                reader_read(rt->r);
                if (!rt->r->n) goto wavEOF;
            }
            size_t items_read = rt->r->n * rt->r->channels;
            printf("read %zd items\n", items_read);
            if (items_read < items_needed) {
                PaUtil_WriteRingBuffer(rt->rb, rt->r->buffer, items_read);
                rt->r->n -= items_read / rt->r->channels;
            } else { /* items_read >= items_needed */
                PaUtil_WriteRingBuffer(rt->rb, rt->r->buffer, items_needed);
                if (items_read > items_needed) {
                    memmove(rt->r->buffer, rt->r->buffer + items_needed,
                            items_read - items_needed);
                }
                rt->r->n -= items_read / rt->r->channels;
            }
        }
        printf("sleeping\n");
        pthread_mutex_lock(&p->lock);
        pthread_cond_wait(&p->wait, &p->lock);
        pthread_mutex_unlock(&p->lock);
        printf("waking\n");
    } while (true);
wavEOF:
    printf("quitting\n");
    rt->running = false;
    return NULL;
}
