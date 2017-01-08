#include <uv.h>

#ifndef SRV_H_
#define SRV_H_
    typedef struct Server
    {
        char* addr_str;
        int port;
        uv_udp_t *socket;
    } server_t;

    int listen_and_serve(server_t *s, uv_loop_t *loop);

    server_t new_server(char *addr, int port);
#endif
