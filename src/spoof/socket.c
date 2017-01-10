#include <sys/socket.h>
#include "socket.h"

int get_socket()
{
    return socket(PF_INET, SOCK_RAW, IPROTO_UDP);
}
