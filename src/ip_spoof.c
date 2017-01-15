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
pthread_mutex_t loop_lock;

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
    pthread_mutex_lock(&loop_lock);
    loop = new_loop();
    pthread_mutex_unlock(&loop_lock);
    int socket_fd = get_socket();
    server_t *server = new_server("0.0.0.0", 8080, socket_fd);
    listen_and_serve(server, loop);
    run_events(loop);
    return NULL;
}

void *send_init(void *addr)
{
    int socket_fd = get_socket();
    char** a = (char**) addr;
    server_t *remote = new_server(*a, 8080, socket_fd);
    char* payload = gen_datagram(remote->addr);
    int has_loop = 0;
    for(;;)
    {
        if(!has_loop)
        {
            pthread_mutex_lock(&loop_lock);
            if(loop)
                has_loop = 1;
            pthread_mutex_unlock(&loop_lock);
        }
        if(has_loop)
        {
            send_msg(remote, payload);
            sleep(1);
        }
    }
}

int main(int argc, char** argv)
{
    printf("starting server on 0.0.0.0:8080\n");
    signal(SIGINT, &handle_signal);
    signal(SIGTERM, &handle_signal);
    pthread_t srv;
    int i;
    if(pthread_create(&srv, NULL, start, &i))
    {
        fprintf(stderr, "Error creating runtime thread\n");
        return 1;
    }
    pthread_t send_loop;
    char *remote_addr = argv[sizeof(argv) - 2];
    if(pthread_create(&send_loop, NULL, send_init, &remote_addr)){
        fprintf(stderr, "Error creating send thread");
        return 1;
    }
    if(pthread_join(srv, NULL))
    {
        fprintf(stderr, "Error joining runtime thread\n");
        return 1;
    }
    printf("exiting...\n");
    return 0;
}
