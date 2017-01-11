#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <uv.h>
#include "eventloop/loop.h"
#include "srv/srv.h"
#include "spoof/socket.h"

uv_loop_t *loop;

void handle_signal(int sig)
{
    if(loop && (sig == SIGINT || sig == SIGTERM))
    {
        destroy_loop(loop);
    }
}

int main() {
    printf("starting server on 0.0.0.0:8080");
    signal(SIGINT, &handle_signal);
    signal(SIGTERM, &handle_signal);
    loop = new_loop();
    int socket_fd = get_socket();
    server_t *server = new_server("0.0.0.0", 8080, socket_fd);
    listen_and_serve(server, loop);
    run_events(loop);
    return 0;
}
