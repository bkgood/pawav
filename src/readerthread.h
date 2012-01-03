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

/**
 * Wakes a ReaderThread.
 */
void readerthread_wake(ReaderThread *rt);

/**
 * Wait for the started ReaderThread to prime itself. This can only be called
 * once (it will return immediately if called a second time).
 */
void readerthread_prime(ReaderThread *rt);

/**
 * Determines whether or not a ReaderThread is running. Its response is only
 * valid after a call to readerthread_prime has returned.
 */
bool readerthread_running(ReaderThread *rt);

#ifdef __cplusplus
}
#endif

#endif
