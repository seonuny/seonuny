#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char **argv)
{
    struct in_addr  addr;
    char temp[256] ;

    if ( inet_aton(argv[1],&addr) == 0 ) return false ;
    int nStart = ntohl(addr.s_addr) ;

    addr.s_addr = htonl(nStart+atoi(argv[2])) ;
    printf("[%s]\n[%s]\n",argv[1],inet_ntoa(addr)) ;
    return 0;
}
