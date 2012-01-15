#include <stdbool.h>
#include <pulse/pulseaudio.h>
#include "readerthread.h"

#ifndef PULSEWRITER_H
#define PULSEWRITER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * An object which writes from a PaUtilRingBuffer to a pa_stream.
 */
typedef struct {
    ReaderThread *rt;
    bool (*stopped)(void);
    void *p;
} PulseWriter;

/**
 * Initializes a PulseWriter.
 *
 * \returns true on success.
 * \returns false on out of memory.
 */
bool pulsewriter_init(PulseWriter *pw);

/**
 * Starts playing the data in the ring buffer, until the ring buffer is empty
 * and the stopped method returns true (async).
 */
void pulsewriter_play(PulseWriter *pw);

/**
 * Blocks until pulsewriter_play finishes.
 */
void pulsewriter_join(PulseWriter *pw);

/**
 * Destroys a PulseWriter.
 */
void pulsewriter_destroy(PulseWriter *pw);

#ifdef __cplusplus
}
#endif
#endif
