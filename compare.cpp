#include <stdio.h>
#include <string>

int main(int argc, char ** argv)
{
    std::string stringTrace = std::string(argv[1]);

    if(stringTrace.compare(0,7,"9999999") == 0)
    {
        printf("[%s:%d][%d]\n",__FILE__,__LINE__,stringTrace.compare(7,4,"01020201211",0,4));
    }
    else if(stringTrace.compare(0,4,"9999") == 0)
    {
        printf("[%s:%d][%d]\n",__FILE__,__LINE__,stringTrace.compare(4,7,"01020201211",0,7));
    }
    else
    {
        printf("[%s:%d][%d]\n",__FILE__,__LINE__,stringTrace.compare("01020201211"));
    }
}
