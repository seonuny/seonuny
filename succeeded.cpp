#include <stdio.h>
#include <stdlib.h>

#define SQL_SUCCEEDED(rc) (((rc)&(~1))==0)  

int main(int argc, char **argv)
{
    if(argv[1] == nullptr)
    {
        printf("%s code\n",argv[0]);
        return 0;
    }
    int sret_ = strtoul(argv[1],NULL,10);

    printf("[%ld]\n",~1);

    printf("ret[%ld:%s]\n",sret_&~1,SQL_SUCCEEDED(sret_)?"true":"false");

    return 1;
}
