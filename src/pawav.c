#include <stdio.h>

#include <pulse/pulseaudio.h>

int main(void)
{
    printf("pawav linked to libpulse %s\n", pa_get_library_version());
    return 0;
}
