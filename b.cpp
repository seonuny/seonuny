#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{

    std::vector<std::string>  midnightNoti ;
    midnightNoti.clear();


    int nLoopCnt = atoi(argv[1]);

    for(int i = 0; i < nLoopCnt; i++)
    {
        midnightNoti.push_back("123");
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < nLoopCnt; i++)
    {
        for (auto &it : midnightNoti)
        {
            if(strcmp(it.c_str(),"xx") == 0)
            {
                ;
            }

        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout<<"Code 1 Duration:" << duration.count() << "seconds" << std::endl;

    return 0;
}
