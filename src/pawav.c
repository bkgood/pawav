#include <math.h>
#include <stdio.h>
#include <pulse/pulseaudio.h>
#include <pthread.h>
#include <unistd.h>

#include "reader.h"
#include "readerthread.h"
#include "pa_ringbuffer.h"

int run_reader(int argc, char *argv[])
{
    if (argc < 2) {
        printf("%s: filename.wav\n", argv[0]);
        return 1;
    }
    const char *f = argv[1];

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

int run_readerthread(int argc, char *argv[])
{
    if (argc < 2) {
        printf("%s: filename.wav [output.raw]\n", argv[0]);
        return 1;
    }
    const char *f = argv[1];
    const char *of = NULL;
    if (argc > 2) of = argv[2];

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
    int16_t *file_data = calloc(buffer_size * r.channels, sizeof(int16_t));
    readerthread_init(&rt);
    rt.r = &r;
    rt.rb = &rb;
    readerthread_start(&rt);

    sleep(1);
    FILE *fh = NULL;
    if (of) {
        fh = fopen(of, "wb");
    } else {
        printf("reading in a thread without output\n");
    }

    while (rt.running) {
        size_t items_avail = PaUtil_GetRingBufferReadAvailable(&rb);
        if (of) {
            size_t items_read = PaUtil_ReadRingBuffer(&rb, file_data,
                    items_avail);
            fwrite(file_data, sizeof(int16_t), items_read, fh);
        } else {
            PaUtil_AdvanceRingBufferReadIndex(&rb, items_avail);
        }

        readerthread_wake(&rt);
    }
    if (fh) fclose(fh);
    readerthread_join(&rt);
    readerthread_destroy(&rt);
    reader_close(&r);
    reader_destroy(&r);
    free(rb_data);
    return 0;
}



int main(int argc, char *argv[])
{
    setenv("PULSE_PROP_application.name", "pawav", 1);
    setenv("PULSE_PROP_media.role", "music", 1);

    return run_readerthread(argc, argv);
}
