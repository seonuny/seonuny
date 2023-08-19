#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char szHostnbame[128+1];
    if(gethostname(szHostnbame,128) != 0)
    {
        perror("gethostname");
    }
    printf("[%s]\n",szHostnbame);
    return 0;
}
