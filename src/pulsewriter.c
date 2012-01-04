#include <string.h>
#include <stdlib.h>

#include "pulsewriter.h"

typedef struct {
    pa_threaded_mainloop *ml;
    pa_stream *s;
} PulseWriterPrivate;


bool pulsewriter_init(PulseWriter *pw)
{
    memset(pw, 0, sizeof(PulseWriter));
    pw->p = calloc(1, sizeof(PulseWriterPrivate));
    if (!pw->p) return false;

    return true;
}

void pulsewriter_play(PulseWriter *pw)
{
    PulseWriterPrivate *p = pw->p;

    p->ml = pa_threaded_mainloop_new();


    pa_threaded_mainloop_free(p->ml);
}

void pulsewriter_destroy(PulseWriter *pw)
{
    PulseWriterPrivate *p = pw->p;

    free(p);
    pw->p = NULL;
}
