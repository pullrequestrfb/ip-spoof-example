#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "srv.h"

static void on_recv(uv_udp_t *handler, ssize_t nread, const uv_buf_t *rcvbuf, const struct sockaddr *addr, unsigned flags)
{
    if(nread > 0)
    {
        printf("received msg\n");
        printf("%s\n", rcvbuf->base);
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
    struct sockaddr_in *srv_addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    uv_ip4_addr(s->addr_str, s->port, srv_addr);
    int status = uv_udp_bind(s->socket, (const struct sockaddr*)srv_addr, UV_UDP_REUSEADDR);
    if(status != 0)
    {
        fprintf(stderr, "could not bind to addr %s\n", s->addr_str);
        return status;
    }
    s->addr = srv_addr;
    status = uv_udp_recv_start(s->socket, on_alloc, on_recv);
    if(status != 0)
    {
        fprintf(stderr, "could not open %s:%d for receiving packets\n", s->addr_str, s->port);
        return status;
    }
    return 0;
}

static void on_send(uv_udp_send_t *req, int status)
{
    printf("sent\n");
}

int send_msg(server_t *s, char *payload)
{
    printf("sending %s to %s:%d\n", payload, s->addr_str, s->port);
    uv_udp_send_t req;
    uv_buf_t buf = uv_buf_init(payload, sizeof(payload));
    return uv_udp_send(&req, s->socket, &buf, 1, (const struct sockaddr*)s->addr, on_send);
}

server_t* new_server(char *addr, int port, int socket_fd)
{
    printf("creating a server on %s:%d\n", addr, port);
    uv_udp_t *socket = (uv_udp_t*)malloc(sizeof(uv_udp_t));
    if(socket_fd != -1)
        uv_udp_open(socket, socket_fd);
    else
    {
        fprintf(stderr, "socket denied\n");
        return NULL;
    }
    server_t *s = (server_t*)malloc(sizeof(server_t));
    s->addr_str = addr;
    s->port = port;
    s->socket = socket;
    return s;
}

server_t* new_remote(char *remote, uv_loop_t *loop, int socket_fd)
{
    printf("creating remote for %s", remote);
    uv_udp_t *socket = (uv_udp_t*)malloc(sizeof(uv_udp_t));
    if(socket_fd != 1)
        uv_udp_open(socket, socket_fd);
    else
    {
        fprintf(stderr, "socket denied\n");
        return NULL;
    }
    server_t *r = (server_t*)malloc(sizeof(server_t));
    r->addr_str = remote;
    r->port = 8080;
    r->socket = socket;
    uv_udp_init(loop, r->socket);
    struct sockaddr_in *rem_addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    uv_ip4_addr(r->addr_str, r->port, rem_addr);
    uv_udp_bind(r->socket, (const struct sockaddr*)rem_addr, UV_UDP_REUSEADDR);
    r->addr = rem_addr;
    return r;
}
