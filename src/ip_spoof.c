#include <stdlib.h>
#include <stdio.h>
#include <uv.h>
#include "eventloop/loop.h"
#include "srv/srv.h"
#include "spoof/socket.h"

int main() {
    uv_loop_t *loop = new_loop();
    int socket_fd = get_socket();
    server_t *server = new_server("0.0.0.0", 8080, socket_fd);
    listen_and_serve(server, loop);
    return 0;
}
