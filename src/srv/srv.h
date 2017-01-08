#include <uv.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#ifndef SRV_H_
#define SRV_H_
    typedef struct Server
    {
        char* addr_str;
        struct sockaddr_in *addr;
        int port;
        uv_udp_t *socket;
    } Server;

    typedef struct Server server_t;

    int listen_and_serve(server_t *s, uv_loop_t *loop);

    int send_msg(server_t *s, char* payload);

    server_t* new_server(char *addr, int port);
#endif
