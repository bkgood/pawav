#include <stdbool.h>
#include <pulse/pulseaudio.h>

#include "pa_ringbuffer.h"

#ifndef PULSEWRITER_H
#define PULSEWRITER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * An object which writes from a PaUtilRingBuffer to a pa_stream.
 */
typedef struct {
    PaUtilRingBuffer *rb;
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
 * Plays the data in the PaUtilRingBuffer, until the ring buffer is empty and
 * the stopped function returns true.
 */
void pulsewriter_play(PulseWriter *pw);

/**
 * Destroys a PulseWriter.
 */
void pulsewriter_destroy(PulseWriter *pw);

#ifdef __cplusplus
}
#endif
#endif
