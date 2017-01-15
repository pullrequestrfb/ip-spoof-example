#include <stdlib.h>
#include <stdio.h>
#include <uv.h>
#include "loop.h"

uv_loop_t* new_loop()
{
    printf("creating event loop\n");
    uv_loop_t *loop = malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);
    return loop;
}

void run_events(uv_loop_t *loop)
{
    printf("started event loop\n");
    uv_run(loop, UV_RUN_DEFAULT);
}

void destroy_loop(uv_loop_t *loop)
{
    printf("tearing down event loop\n");
    uv_loop_close(loop);
    free(loop);
}
