#include <stdlib.h>
#include <ctype.h>
#include <sndfile.h>
#include <string.h>

#include "reader.h"

const size_t kDefaultBuffer = 4096;

typedef struct {
    SF_INFO *info;
    SNDFILE *handle;
    size_t buffer_size;
} ReaderPrivate;

bool reader_create(Reader **r)
{
    *r = calloc(1, sizeof(Reader));
    if (!*r) return false;
    memset(*r, 0, sizeof(Reader));

    ReaderPrivate *p = calloc(1, sizeof(ReaderPrivate));
    if (!p) return false;
    memset(p, 0, sizeof(ReaderPrivate));
    (*r)->p = p;

    p->info = calloc(1, sizeof(SF_INFO));
    if (!p->info) return false;
    memset(p->info, 0, sizeof(SF_INFO));

    return true;
}

bool reader_open(Reader *r)
{
    ReaderPrivate *p = r->p;

    if (!r->n) {
        r->n = kDefaultBuffer;
    }
    p->buffer_size = r->n;

    if (!r->filename) {
        printf("reader_open requires a file name\n");
        return false;
    } else {
        char ext[4];
        /* get last 3+nul values of extension */
        memcpy(&ext[0], r->filename + strlen(r->filename) - 3, 4);
        for (int i = 0; i < 3; i++) {
            ext[i] = tolower(ext[i]);
        }
        if (strcmp(&ext[0], "wav")) {
            printf("only wav files are supported\n");
            return false;
        }
    }

    p->handle = sf_open(r->filename, SFM_READ, p->info);
    if (!p->handle) {
        printf("sf_open error: %s\n", sf_strerror(NULL));
        return false;
    }

    if (!(p->info->format & SF_FORMAT_WAV) /*||
            !(p->info->format & SF_FORMAT_PCM_16)*/) {
        printf("file not a wav\n");
        return false;
    }
    r->channels = p->info->channels;
    r->samplerate = p->info->samplerate;

    r->buffer = calloc(r->channels * p->buffer_size, sizeof(int16_t));
    if (!r->buffer) {
        printf("failed to allocate buffer\n");
        return false;
    }

    return true;
}

bool reader_read(Reader *r)
{
    ReaderPrivate *p = r->p;
    r->n = sf_read_short(p->handle, r->buffer, p->buffer_size * r->channels);
    return true;
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

void reader_destroy(Reader **r)
{
    ReaderPrivate *p = (*r)->p;
    free(p->info);
    free(p);
    free(*r);
    *r = NULL;
}
