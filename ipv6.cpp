#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool Ipv6PrefixFull(
        const char *a_pszIpv6PrefixS,
        char *a_pszIpv6PrefixD)
{

    char szIpv6[128];
    int nPrefix;

    int nScanfCnt = sscanf(a_pszIpv6PrefixS,
            "%128[^/]/%d",
             szIpv6,&nPrefix);

    if(nScanfCnt==2)
    {
        const int nMAX_IPV6_LEN = sizeof(struct in6_addr);
        const int nMAX_IPV6_PREFIX_LEN=64;
        unsigned char buf[nMAX_IPV6_LEN];
        int nRtn = inet_pton(AF_INET6,szIpv6,buf);
        if(nRtn >0)
        {
            int j=0;
            int nOffset = 0;
            for(int i = 0 ; i < nMAX_IPV6_LEN;i++)
            {
                if(i!=0&&(i%2)==0)
                {
                    nOffset += snprintf(a_pszIpv6PrefixD+nOffset,
                            nMAX_IPV6_PREFIX_LEN-nOffset,"%s",(char*)":");
                }
                j=j+8;
                nOffset += snprintf(a_pszIpv6PrefixD+nOffset,
                        nMAX_IPV6_PREFIX_LEN-nOffset,"%02x",buf[i]);
                if(j >= nPrefix)
                {
                    break;
                }
            }
            snprintf(a_pszIpv6PrefixD+nOffset, 
                    nMAX_IPV6_PREFIX_LEN-nOffset,
                    "%s%d",j==128?"/":"::/",j);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

int main(int argc, char**argv)
{
    char szIpv6Prefix[128];
    Ipv6PrefixFull(argv[1],szIpv6Prefix);

    printf("[%s]\n[%s]\n",argv[1],szIpv6Prefix);
    return 0;
}
