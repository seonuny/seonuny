#include <stdio.h>

int main(int argc, char **argv)
{
    int n = 39;
    printf("%s\n",n);
    if(0x00 == 0x00)
    {
        printf("same\n");
    }
    else
    {
        printf("diff\n");
    }
    if(NULL == 0x00)
    {
        printf("same\n");
    }
    else
    {
        printf("diff\n");
    }
    return 0;
}
