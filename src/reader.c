#include <stdlib.h>
#include <ctype.h>
#include <sndfile.h>
#include <string.h>

#include "reader.h"

const size_t kDefaultBuffer = 4096;

typedef struct {
    SF_INFO info;
    SNDFILE *handle;
    size_t buffer_size;
} ReaderPrivate;

bool reader_init(Reader *r)
{
    ReaderPrivate *p = calloc(1, sizeof(ReaderPrivate));
    if (!p) return false;

    memset(r, 0, sizeof(*r));
    memset(&p->info, 0, sizeof(p->info));

    r->p = p;
    return true;
}

bool reader_open(Reader *r)
{
    ReaderPrivate *p = r->p;

    if (!r->filename) {
        printf("reader_open requires a file name\n");
        return false;
    }

    p->handle = sf_open(r->filename, SFM_READ, &p->info);
    if (!p->handle) {
        printf("sf_open error: %s\n", sf_strerror(NULL));
        return false;
    }

    if (!(p->info.format & SF_FORMAT_WAV)) {
        printf("file not a wav\n");
        return false;
    }
    r->channels = p->info.channels;
    r->samplerate = p->info.samplerate;

    if (!r->n) {
        r->n = kDefaultBuffer;
    }
    p->buffer_size = r->n;

    r->buffer = calloc(r->channels * p->buffer_size, sizeof(int16_t));
    if (!r->buffer) {
        printf("failed to allocate buffer\n");
        return false;
    }

    return true;
}

void reader_read(Reader *r)
{
    ReaderPrivate *p = r->p;
    r->n = sf_readf_short(p->handle, r->buffer, p->buffer_size);
}

bool reader_close(Reader *r)
{
    ReaderPrivate *p = r->p;

    int error = sf_close(p->handle);
    if (error) {
        printf("sndfile error: %s\n", sf_error_number(error));
        return false;
    }

    free(r->buffer);
    r->buffer = NULL;

    return true;
}

void reader_destroy(Reader *r)
{
    ReaderPrivate *p = r->p;
    free(p);
    r->p = NULL;
}
