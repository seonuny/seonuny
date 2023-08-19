
// Example program
#include <stdio.h>
#include <iostream>
#include <string>

int main()
{
    char szData[1024] = {0,};
    sscanf("abcpcf01-mp01-app01","%*[^g^-]%127[^$]",szData);
    printf("%s\n",szData);
}
