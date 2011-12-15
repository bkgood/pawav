#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *filename;
    void *buffer;
    size_t n;
    void *p;
} Reader;

bool reader_create(Reader **r);
bool reader_open(Reader *r);
bool reader_read(Reader *r);
bool reader_close(Reader *r);
bool reader_destroy(Reader **r);

#ifdef __cplusplus
} // extern "C"
#endif
