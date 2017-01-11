#include <sys/socket.h>
#include <netinet/ip.h>
#include "socket.h"

int get_socket()
{
    return socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
}
