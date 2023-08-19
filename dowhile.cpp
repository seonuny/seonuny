#include <stdio.h>

int main(int argc, char **argv)
{
    do
    {
        if(argv[1] == nullptr)
        {
            printf("argv[1] is null\n");
            continue;
        }
        if(argv[2] == nullptr)
        {
            printf("argv[2] is null\n");
            continue;
        }
        if(argv[3] == nullptr)
        {
            printf("argv[3] is null\n");
            break;
        }
    }while(false);
    printf("end\n");
    return 0;
}
