#include <stdio.h>
#include <iostream>

int main(int argc, char **argv)
{
    std::string name = "SessionDetail_3";
    std::string active = "SessionDetail_3_P";

    std::size_t pos = name.find("_");
    printf("[%u][%s:%s]\n",pos,name.substr(0,pos).data(),name.substr(pos+1,1).data());
                pos = active.rfind("_");
    printf("[%u][%s:%s]\n",pos,active.substr(0,pos).data(),active.substr(pos+1,1).data());
    return 0;
}
