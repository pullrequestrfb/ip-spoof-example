#include <uv.h>

#ifndef LOOP_H_
#define LOOP_H_
    uv_loop_t* new_loop();
    void run_events(uv_loop_t *loop);
    void destroy_loop(uv_loop_t *loop);
#endif
