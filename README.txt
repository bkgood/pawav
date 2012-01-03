A test pulseaudio project, to familarize myself with its async api.

Everything here is LGPL3 unless said otherwise (eg. the portaudio ringbuffer
files).

Idea:
We're using the PulseAudio async API, which will sit off in our main thread.
First, we start a thread that reads a WAVE file into a lock-free ring buffer.
This thread fills the buffer and then sits on a pthread wait condition; every
time pulse gets audio from us we fire the condition.

Reading functionality is implemented in two files (see Doxygen for more):
Reader is a simple layer over sndfile, which reads a file into a buffer.
ReaderThread uses pthreads to write data read with a Reader into a
PaUtilRingBuffer.

