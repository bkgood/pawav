#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "pulsewriter.h"

typedef struct {
    pa_threaded_mainloop *ml;
    pa_context *c;
    pa_stream *s;
} PulseWriterPrivate;

static void setup(PulseWriter *pw);
static void start_cb(pa_context *c, void *pw);

bool pulsewriter_init(PulseWriter *pw)
{
    memset(pw, 0, sizeof(PulseWriter));
    pw->p = calloc(1, sizeof(PulseWriterPrivate));
    PulseWriterPrivate *p = pw->p;
    if (!pw->p) return false;

    p->ml = pa_threaded_mainloop_new();
    pa_threaded_mainloop_start(p->ml);

    setup(pw);
    create_stream(pw);
    return true;
}

void pulsewriter_play(PulseWriter *pw)
{
    PulseWriterPrivate *p = pw->p;
    (void) p;
}

void pulsewriter_join(PulseWriter *pw)
{
    PulseWriterPrivate *p = pw->p;

    (void) p;
}

void pulsewriter_destroy(PulseWriter *pw)
{
    PulseWriterPrivate *p = pw->p;

    pa_threaded_mainloop_stop(p->ml);
    pa_threaded_mainloop_free(p->ml);

    free(p);
}

static void setup(PulseWriter *pw)
{
    PulseWriterPrivate *p = pw->p;

    pa_threaded_mainloop_lock(p->ml);
    p->c = pa_context_new(pa_threaded_mainloop_get_api(p->ml), "pawav");
    pa_context_set_state_callback(p->c, start_cb, pw);
    pa_context_connect(p->c, NULL, PA_CONTEXT_NOFLAGS, NULL);

    while (pa_context_get_state(p->c) != PA_CONTEXT_READY) {
        pa_threaded_mainloop_wait(p->ml);
    }
    pa_context_set_state_callback(p->c, NULL, NULL);

    pa_threaded_mainloop_unlock(p->ml);
}

static void create_stream(PulseWriter *pw)
{
    PulseWriterPrivate *p = pw->p;

    pa_threaded_mainloop_lock(p->ml);
    pa_sample_spec ss;
    pa_channel_map map;
    pa_proplist pl;
    p->s = pa_stream_new(p->c, "pawav", &ss, &map, &pl);
}

static void start_cb(pa_context *c, void *vpw)
{
    PulseWriter *pw = vpw;
    PulseWriterPrivate *p = pw->p;
    (void) c;
    pa_threaded_mainloop_signal(p->ml, false);
}
