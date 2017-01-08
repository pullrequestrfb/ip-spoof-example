#include <stdlib.h>
#include <uv.h>
#include "loop.h"

uv_loop_t* new_loop()
{
    uv_loop_t *loop = malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);
    return loop;
}

void run_events(uv_loop_t *loop)
{
    uv_run(loop, UV_RUN_DEFAULT);
}

void destroy_loop(uv_loop_t *loop)
{
    uv_loop_close(loop);
    free(loop);
}

