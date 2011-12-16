#include <pthread.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *filename;
    void *p;
} ReaderThread;

bool create_readerthread(ReaderThread **rt);
bool start_readerthread(ReaderThread *rt);
bool stop_readerthread(ReaderThread *rt);
void destroy_readerthread(ReaderThread **rt);

#ifdef __cplusplus
}
#endif
