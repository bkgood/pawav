#include <stdlib.h>
#include <sndfile.h>

#include "reader.h"

typedef struct {
    SF_INFO *info;
    SNDFILE *handle;
} ReaderPrivate;

bool reader_create(Reader **r)
{
    *r = calloc(1, sizeof(Reader));
    if (!*r) return false;

    return true;
}


bool reader_open(Reader *r)
{
    ReaderPrivate *p = r->p;
    (void) p;

    return true;
}

bool reader_read(Reader *r)
{
    ReaderPrivate *p = r->p;
    (void) p;

    return true;
}

bool reader_close(Reader *r)
{
    ReaderPrivate *p = r->p;
    (void) p;

    return true;
}

bool reader_destroy(Reader **r)
{
    ReaderPrivate *p = (*r)->p;
    (void) p;

    return true;
}
