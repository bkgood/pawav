A test pulseaudio project, to familarize myself with its async api.

Everything here is LGPL3 unless said otherwise (eg. the portaudio ringbuffer
files).

Idea:
We're using the PulseAudio async API, which will sit off in our main thread.
First, we start a thread that reads a WAVE file into a lock-free ring buffer.
This thread fills the buffer and then sits on a pthread wait condition; every
time pulse gets audio from us we fire the condition.

Eventually, the WAVE will end, and we need a way to signal this to the pulse
callbacks from the reader. I haven't figured this out yet.
