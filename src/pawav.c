#include <stdio.h>
#include <pulse/pulseaudio.h>

#include "reader.h"

int run_reader(const char *f)
{
    Reader *r;
    if (!reader_create(&r)) {
        printf("out of memory\n");
        return EXIT_FAILURE;
    }

    r->filename = f;
    reader_open(r);
    while (r->n) {
        reader_read(r);
    }
    reader_close(r);
    reader_destroy(&r);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("%s: filename.wav\n", argv[0]);
    }

    setenv("PULSE_PROP_application.name", "pawav", 1);
    setenv("PULSE_PROP_media.role", "music", 1);

    return run_reader(argv[1]);
}
