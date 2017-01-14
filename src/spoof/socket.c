#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "socket.h"

unsigned short checksum(unsigned short *ptr,int nbytes)
{
    register long sum;
    unsigned short oddbyte;
    register short answer;
    sum = 0;
    while(nbytes > 1)
    {
        sum += *ptr++;
        nbytes -= 2;
    }
    if(nbytes==1)
    {
        oddbyte = 0;
        *((u_char*)&oddbyte) = *(u_char*)ptr;
        sum += oddbyte;
    }
    sum = (sum>>16) + (sum & 0xffff);
    sum = sum + (sum>>16);
    answer = (short)~sum;
}

char* gen_datagram(struct sockaddr_in *sin)
{
    char datagram[4096];
    char source_ip[32];
    memset(datagram, 0, 4096);
    struct iphdr *iph = (struct iphdr *)datagram;
    struct udphdr *udph = (struct udphdr *)(datagram + sizeof(struct iphdr));
    struct pseudo_header psh;
    char *data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(data, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    strcpy(source_ip, "192.168.1.2");
    set_sockaddr_in(sin);
    set_ipheader(iph, sin, datagram, source_ip, data);
    set_udpheader(udph, data);
    set_pseudo_header(psh, sin, udph, source_ip, data);
}

void set_ipheader(struct iphdr *iph, struct sockaddr_in *sin, char datagram[4096], char source_ip[32], char *data)
{
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
    iph->id = htonl(54321);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr(source_ip);
    iph->daddr = sin->sin_addr.s_addr;
    iph->check = checksum((unsigned short *)datagram, iph->tot_len);
}

void set_udpheader(struct udphdr *udph, char *data)
{
    udph->source = htons(6666);
    udph->dest = htons(8622);
    udph->len = htons(8 + strlen(data));
    udph->check = 0;
}

void set_pseudo_header(struct pseudo_header *psh, struct sockaddr_in *sin, struct udphdr *udph, char source_ip[32], char *data)
{
    psh->source_address = inet_addr(source_ip);
    psh->dest_address = sin->sin_addr.s_addr;
    psh->placeholder = 0;
    psh->protocol = IPPROTO_UDP;
    psh->udp_length = htons(sizeof(struct udphdr) + strlen(data));
    int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
    char *pseudogram = malloc(psize);
    memcpy(pseudogram, (char*) &psh, sizeof(struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header), udph, sizeof(struct udphdr) + strlen(data));
    udph->check = checksum((unsigned short*)pseudogram, psize);
}

void set_sockaddr_in(struct sockaddr_in *sin)
{
    sin->sin_family = AF_INET;
    sin->sin_port = htons(10001);
    sin->sin_addr.s_addr = inet_addr("192.168.1.1");
}

int get_socket()
{
    return socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
}
