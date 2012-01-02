#include <stdbool.h>
#include <stdint.h>

#ifndef READER_H
#define READER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A Reader, which describes a WAV file being decoded.
 */
typedef struct {
    /** The filename */
    const char *filename;
    /** The decoded audio buffer */
    int16_t *buffer;
    /** The number of frames this buffer contains. If buffer is null, allocates
     * a new buffer able to hold this many frames on open (or a default if 0) */
    size_t n;
    /** The number of channels with available data */
    int channels;
    /** The sample rate in Hz */
    int samplerate;
    /** Private data */
    void *p;
} Reader;

/**
 * Initialized a Reader structure.
 *
 * \returns true if a reader was sucessfully initialized.
 * \returns false on out of memory.
 */
bool reader_init(Reader *r);

/**
 * Opens a reader.
 *
 * \returns true on success
 * \returns false on error, with error printed to stdout
 */
bool reader_open(Reader *r);

/**
 * Reads data into the reader buffer.
 */
void reader_read(Reader *r);

/**
 * Closes a reader.
 *
 * \returns true on success
 * \returns false on sndfile error, printed to stdout
 */
bool reader_close(Reader *r);

/**
 * Destroys a reader.
 */
void reader_destroy(Reader *r);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
