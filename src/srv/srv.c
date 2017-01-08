#include <stdlib.h>
#include <stdio.h>
#include "srv.h"

static void on_recv(uv_udp_t *handler, ssize_t nread, const uv_buf_t *rcvbuf, const struct sockaddr *addr, unsigned flags)
{
    if(nread > 0)
    {
        printf("received msg");
    }
    free(rcvbuf->base);
}

static void on_alloc(uv_handle_t *client, size_t suggested_size, uv_buf_t *buf)
{
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

int listen_and_serve(server_t *s, uv_loop_t *loop)
{
    uv_udp_init(loop, s->socket);
    struct sockaddr_in srv_addr;
    uv_ip4_addr(s->addr_str, s->port, &srv_addr);
    int status = uv_udp_bind(s->socket, (const struct sockaddr*)&srv_addr, UV_UDP_REUSEADDR);
    if(status != 0)
    {
        printf("could not bind to addr");
        return status;
    }
    s->addr = &srv_addr;
    status = uv_udp_recv_start(s->socket, on_alloc, on_recv);
    if(status != 0)
    {
        printf("could not open for receiving packets");
        return status;
    }
    return 0;
}

static void on_send(uv_udp_send_t *req, int status)
{
    printf("sent");
}

int send_msg(server_t *s, char* payload)
{
    uv_udp_send_t req;
    uv_buf_t buf = uv_buf_init(payload, sizeof(payload));
    return uv_udp_send(&req, s->socket, &buf, 1, (const struct sockaddr*)s->addr, on_send);
}

server_t* new_server(char* addr, int port)
{
    uv_udp_t *socket = (uv_udp_t*)malloc(sizeof(uv_udp_t));
    server_t* s = (server_t*)malloc(sizeof(server_t));
    s->addr_str = addr;
    s->port = port;
    s->socket = socket;
    return s;
}
