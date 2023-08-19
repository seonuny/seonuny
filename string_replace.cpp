#include <stdio.h>

#include <string>

int main(int argc, char **argv)
{
    std::string str1 = "abcdefghijklmnopqrstuvwxyz";

    str1.replace(2,1,"12345678");

    printf("[%s]\n",str1.data());

    return 0;
}
