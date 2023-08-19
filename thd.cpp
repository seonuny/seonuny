#include <stdio.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <functional>
#include <thread>
#include <map>

std::map<std::string*,std::string*> g_mapData;
thread_local  std::string stringKey1,stringVal1;
std::string *stringKey2 ;
std::string *stringVal2 ;
void Run1(void *arg)
{
    stringKey1 = "abc";
    stringVal1 = "def";
    stringKey2 = new std::string();
    stringVal2 = new std::string();
   *stringKey2 = stringKey1;
   *stringVal2 = stringVal1;
    g_mapData.emplace(stringKey2,stringVal2);
}

void Run2(void *arg)
{
    sleep(1);
    std::cout<<"size:"<<g_mapData.size() << std::endl;
    for(const std::pair<std::string*,std::string*> &kv: g_mapData)
    {
        std::cout<<kv.first->data()<<" " << kv.second->data() << std::endl;
    }
  //printf("[%s:%s]\n",stringKey2->data(),stringVal2->data());
}

int main(int argc, char **argv)
{
    std::thread t1(Run1,nullptr);
    std::thread t2(Run2,nullptr);

    t1.join();
    t2.join();

    return 0;
}
