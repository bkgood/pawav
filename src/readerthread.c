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
    pthread_mutex_t prime_lock;
    pthread_cond_t prime_wait;
    pthread_mutex_t run_lock;
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

    if ((err = pthread_mutex_init(&p->lock, NULL))) return false;
    if ((err = pthread_cond_init(&p->wait, NULL))) return false;
    if ((err = pthread_mutex_init(&p->prime_lock, NULL))) return false;
    if ((err = pthread_cond_init(&p->prime_wait, NULL))) return false;
    if ((err = pthread_mutex_init(&p->run_lock, NULL))) return false;

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
    pthread_mutex_destroy(&p->prime_lock);
    pthread_cond_destroy(&p->prime_wait);
}

void readerthread_wake(ReaderThread *rt)
{
    ReaderThreadPrivate *p = rt->p;

    pthread_cond_signal(&p->wait);
}

void readerthread_prime(ReaderThread *rt)
{
    ReaderThreadPrivate *p = rt->p;

    if (!pthread_mutex_trylock(&p->prime_lock)) {
        pthread_cond_wait(&p->prime_wait, &p->prime_lock);
    }
}

bool readerthread_running(ReaderThread *rt)
{
    ReaderThreadPrivate *p = rt->p;

    if (!pthread_mutex_trylock(&p->run_lock)) {
        pthread_mutex_unlock(&p->run_lock);
        return false;
    }
    return true;
}

static void *readerthread_run(void *vrt)
{
    ReaderThread *rt = vrt;
    ReaderThreadPrivate *p = rt->p;
    pthread_mutex_lock(&p->run_lock);

    do {
        size_t items_needed = 0;
        while ((items_needed = PaUtil_GetRingBufferWriteAvailable(rt->rb))) {
            if (!rt->r->n) {
                reader_read(rt->r);
                if (!rt->r->n) goto wavEOF;
            }
            size_t items_read = rt->r->n * rt->r->channels;
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
        pthread_cond_signal(&p->prime_wait);
        pthread_mutex_lock(&p->lock);
        pthread_cond_wait(&p->wait, &p->lock);
        pthread_mutex_unlock(&p->lock);
    } while (true);
wavEOF:
    pthread_mutex_unlock(&p->run_lock);
    return NULL;
}
