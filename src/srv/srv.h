#ifndef SRV_H_
#define SRV_H_
    typedef struct Server
    {
        char* addr_str;
    }

    int listen_and_serve();
#endif
