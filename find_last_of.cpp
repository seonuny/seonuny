#include <stdio.h>

#include <string>

int main(int argc, char **argv)
{
    std::string stringUrl = argv[1];

    size_t nPos = stringUrl.find_last_of("/");

    if(nPos != std::string::npos)
    {
        printf("[%s]\n",stringUrl.substr(nPos+1).data());
    }
    else
    {
        printf("now found!\n");
    }
    return 0;                
}
