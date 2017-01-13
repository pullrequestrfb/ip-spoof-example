#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#ifndef SOCKET_H_
#define SOCKET_H_
    typedef struct pseudo_header
    {
        u_int32_t source_address;
        u_int32_t dest_address;
        u_int8_t placeholder;
        u_int8_t protocol;
        u_int16_t udp_length;
    } pseudo_header;

    unsigned short checksum(unsigned short *ptr,int nbytes);

    char* gen_datagram(struct sockaddr_in *sin);

    void set_ipheader(struct iphdr *iph, struct sockaddr_in *sin, char datagram[4096], char source_ip[32], char *data);

    void set_udpheader(struct udphdr *udph, char *data);

    void set_pseudo_header(struct pseudo_header *psh, struct sockaddr_in *sin, struct udphdr *udph, char source_ip[32], char *data);

    void set_sockaddr_in(struct sockaddr_in *sin);

    int get_socket();
#endif
