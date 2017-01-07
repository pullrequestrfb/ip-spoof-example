#ifndef SRV_H_
#define SRV_H_
    typedef struct Server
    {
        char* addr_str;
        int socket;
    } Server;

    int listen_and_serve(struct Server *s);

    struct Server* new_server(char *proto, char *addr);
#endif
