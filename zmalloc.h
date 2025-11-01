#include <unistd.h>

typedef enum {
    FIRST_FIT,
    BEST_FIT
} strategy_t;

void *zmalloc(size_t, strategy_t);
void zfree(void *);