#include <math.h>
#include <stdio.h>
#include <pulse/pulseaudio.h>
#include <pthread.h>
#include <unistd.h>

#include "reader.h"
#include "readerthread.h"
#include "pa_ringbuffer.h"

int run_reader(const char *f)
{
    Reader r;
    if (!reader_init(&r)) {
        printf("out of memory\n");
        return EXIT_FAILURE;
    }

    r.filename = f;
    reader_open(&r);
    while (r.n) {
        reader_read(&r);
    }
    reader_close(&r);
    reader_destroy(&r);
    return 0;
}

int run_readerthread(const char *f)
{
    Reader r;
    ReaderThread rt;
    PaUtilRingBuffer rb;

    size_t buffer_size = 8192;
    reader_init(&r);
    r.filename = f;
    r.n = buffer_size;
    reader_open(&r);
    assert(log2(r.channels) == (int) log2(r.channels));
    int16_t *rb_data = calloc(buffer_size * r.channels, sizeof(int16_t));
    PaUtil_InitializeRingBuffer(&rb, sizeof(int16_t), buffer_size * r.channels,
        rb_data);
    readerthread_init(&rt);
    rt.r = &r;
    rt.rb = &rb;
    readerthread_start(&rt);

    sleep(1);
    while (rt.running) {
//        printf("trashing %d items\n", PaUtil_GetRingBufferReadAvailable(&rb));
        PaUtil_AdvanceRingBufferReadIndex(&rb,
                PaUtil_GetRingBufferReadAvailable(&rb));
        printf("command wake\n");
        readerthread_wake(&rt);
    }
    printf("joining\n");
    readerthread_join(&rt);
    /* readerthread is done */
    readerthread_destroy(&rt);
    reader_close(&r);
    reader_destroy(&r);
    free(rb_data);
    return 0;
}



int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("%s: filename.wav\n", argv[0]);
    }

    setenv("PULSE_PROP_application.name", "pawav", 1);
    setenv("PULSE_PROP_media.role", "music", 1);

    return run_readerthread(argv[1]);
}
