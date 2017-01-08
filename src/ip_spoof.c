#include <stdlib.h>
#include <stdio.h>
#include <uv.h>
#include "src/eventloop/loop.h"
#include "src/srv/srv.h"

int main() {
    uv_loop_t *loop = new_loop();
    server_t *server = new_server("0.0.0.0", 8080);
    listen_and_serve(server, loop);
    return 0;
}
