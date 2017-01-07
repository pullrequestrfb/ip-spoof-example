#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include "srv.h"

int get_socket(char *proto)
{
    case "UDP":
    case "udp":
        return socket(AF_INET, SOCK_DGRAM, 17);
    default:
        printf("unsupported protocol requested");
        return -1;
}

int listen_and_serve(struct Server *s)
{
    // TODO implement listen_and_serve once libuv loop is implemented
}

struct Server* new_server(char* proto, char* addr)
{
    struct Server* s = (struct Server*)calloc(4, sizeof(struct Server));
    s->addr_str = addr;
    s->socket = get_socket(proto);
    return s;
}
