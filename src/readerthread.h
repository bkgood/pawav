#include <pthread.h>
#include <stdbool.h>

#include "pa_ringbuffer.h"
#include "reader.h"

#ifndef READERTHREAD_H
#define READERTHREAD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A thread which reads data from a Reader into a PaUtilRingBuffer.
 */
typedef struct {
    /** Reader to read from. */
    Reader *r;
    /** PaUtilRingBuffer to read data into. */
    PaUtilRingBuffer *rb;
    /** Private data. */
    void *p;
} ReaderThread;

/**
 * Initializes a ReaderThread.
 *
 * \returns true on success
 * \returns false on out of memory
 */
bool readerthread_init(ReaderThread *rt);

/**
 * Starts a ReaderThread.
 *
 * \returns true on success
 * \returns false on failure (pthread error)
 */
bool readerthread_start(ReaderThread *rt);

/**
 * Joins the calling process to the ReaderThread.
 *
 * May return immediately if the thread has already ended.
 */
void readerthread_join(ReaderThread *rt);

/**
 * Destroys a ReaderThread.
 */
void readerthread_destroy(ReaderThread *rt);

#ifdef __cplusplus
}
#endif

#endif
