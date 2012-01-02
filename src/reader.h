#include <stdbool.h>
#include <stdint.h>

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
     * a new buffer able to hold this many frames on open. */
    size_t n;
    /** The number of channels with available data */
    int channels;
    /** The sample rate in Hz */
    int samplerate;
    /** Private data */
    void *p;
} Reader;

/**
 * Creates a Reader structure, ready to be filled in and sent to
 * reader_open.
 *
 * \returns true if a reader was sucessfully created
 * \returns false if out of memory.
 */
bool reader_create(Reader **r);

/**
 * Opens a reader.
 *
 * \returns true on success
 * \returns false on error
 */
bool reader_open(Reader *r);

/**
 * Reads data into the reader buffer.
 *
 * \returns true on success
 * \false on error
 */
bool reader_read(Reader *r);

/**
 * Closes a reader.
 *
 * \returns true on success
 * \returns false on error
 */
bool reader_close(Reader *r);

/**
 * Destroys a reader.
 */
void reader_destroy(Reader **r);

#ifdef __cplusplus
} // extern "C"
#endif
