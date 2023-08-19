#include <stdio.h>

int main(int argc , char **argv)
{
    int nMwday = 0;

    printf("0[%02x]\n",0x01<<(1%7));
    printf("1[%02x]\n",0x01<<(2%7));
    printf("2[%02x]\n",0x01<<(3%7));
    printf("3[%02x]\n",0x01<<(4%7));
    printf("4[%02x]\n",0x01<<(5%7));
    printf("5[%02x]\n",0x01<<(6%7));
    printf("6[%02x]\n",0x01<<(7%7));

    return 0;
}

