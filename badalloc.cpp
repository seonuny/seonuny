#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <functional>

// function to call if operator new can't allocate enough memory or error arises
void outOfMemHandler()
{
    std::cerr << "Unable to satisfy request for memory\n";

    std::abort();
}

class CNew
{
    public:
        void Handler()
        {
            printf("handler\n");
        }
};

int main(int argc, char **argv)
{
    try
    {
        CNew cNew;
        std::function<void(void)> handler;
        handler = std::bind(&CNew::Handler,&cNew);

        handler();
        //set the new_handler
        //std::set_new_handler(outOfMemHandler);
      //std::set_new_handler(handler);

        //Request huge memory size, that will cause ::operator new to fail
        int *pBigDataArray = new int[1000000000000L];

        if(pBigDataArray==nullptr)
        {
            printf("nullptr\n");
        }
    }
    catch(std::exception e)
    {
        printf("%s\n",e.what());
    }

    return 0;
}
