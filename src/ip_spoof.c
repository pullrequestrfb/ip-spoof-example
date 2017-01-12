#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <uv.h>
#include "eventloop/loop.h"
#include "srv/srv.h"
#include "spoof/socket.h"

uv_loop_t *loop;

void handle_signal(int sig)
{
    if(loop && (sig == SIGINT || sig == SIGTERM))
    {
        printf("got signal %d\n", sig);
        destroy_loop(loop);
        exit(0);
    }
}

void *start(void *ptr)
{
    loop = new_loop();
    int socket_fd = get_socket();
    server_t *server = new_server("0.0.0.0", 8080, socket_fd);
    listen_and_serve(server, loop);
    run_events(loop);
    return NULL;
}

int main()
{
    printf("starting server on 0.0.0.0:8080\n");
    signal(SIGINT, &handle_signal);
    signal(SIGTERM, &handle_signal);
    pthread_t test;
    int i;
    if(pthread_create(&test, NULL, start, &i))
    {
        fprintf(stderr, "Error creating runtime thread\n");
        return 1;
    }
    if(pthread_join(test, NULL))
    {
        fprintf(stderr, "Error joining runtime thread\n");
        return 1;
    }
    printf("exiting...\n");
    return 0;
}
