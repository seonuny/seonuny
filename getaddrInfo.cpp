#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


// Convert a struct sockaddr address to a string, IPv4 and IPv6:

char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen)
{
    switch(sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                    s, maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                    s, maxlen);
            break;

        default:
            strncpy(s, "Unknown AF", maxlen);
            return NULL;
    }

    return s;
}

int main(int argc, char **argv)
{
    struct addrinfo   stAddrHints;
    struct addrinfo *pstAddrRes,*pstAddrRP;
    memset(&stAddrHints, 0, sizeof(struct addrinfo));
    stAddrHints.ai_family     = AF_UNSPEC;      /* Allow IPv4 or IPv6 */
    stAddrHints.ai_socktype   = SOCK_STREAM;    /* steam socket */
    stAddrHints.ai_flags      = 0;
    stAddrHints.ai_flags      = AI_PASSIVE;     /* For wildcard IP address */
    stAddrHints.ai_flags     |= AI_NUMERICHOST; /* Allow IPv4 or IPv6 */
    stAddrHints.ai_protocol   = 0;   /* Any protocol */

    printf("[%d]\n",AF_INET);

    int nRet = getaddrinfo(NULL,"7418",&stAddrHints,&pstAddrRes);
    if(nRet != 0)
    {
        printf("getaddrinfo:%s",gai_strerror(nRet));
        return -1;
    }

    char szAddr[64+1];
    unsigned char ucAddr[5];
    for (pstAddrRP = pstAddrRes; pstAddrRP != NULL; pstAddrRP = pstAddrRP->ai_next)
    {
        printf("[%d][%s][%p]\n",pstAddrRP->ai_family,get_ip_str(pstAddrRP->ai_addr,szAddr,64),pstAddrRP);
    }

    return 0;
}
