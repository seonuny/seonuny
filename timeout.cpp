#include <stdio.h>
#include <chrono>
#include <unistd.h>
#include <map>

typedef std::multimap<long,std::string> MAP_TIME;
typedef std::pair<long,std::string>      PA_TIME;

using Clock          = std::chrono::system_clock;
using Milli          = std::chrono::milliseconds;
using TimePoint      = std::chrono::system_clock::time_point;
#define  Now         std::chrono::system_clock::now()

template<class Scale>
using TP = std::chrono::time_point<Clock,Scale>;
int main(int argc,char **argv)
{
    TimePoint now;
    TP<Milli> now_ms;
    std::string strVal;


    MAP_TIME mapTime ;
    strVal = "A";
    now_ms = std::chrono::time_point_cast<Milli>(Now);
    printf("[%ld]\n",now_ms.time_since_epoch().count());
    mapTime.emplace(PA_TIME(now_ms.time_since_epoch().count(),strVal));
    usleep(1000);
    strVal = "B";
    now_ms = std::chrono::time_point_cast<Milli>(Now);    
    printf("[%ld]\n",now_ms.time_since_epoch().count());
    mapTime.emplace(PA_TIME(now_ms.time_since_epoch().count(),strVal));
    usleep(1000);
    strVal = "C";
    now_ms = std::chrono::time_point_cast<Milli>(Now);    
    printf("[%ld]\n",now_ms.time_since_epoch().count());
    mapTime.emplace(PA_TIME(now_ms.time_since_epoch().count(),strVal));
    usleep(1000);
    strVal = "D";
    now = Now;
    now_ms = std::chrono::time_point_cast<Milli>(now);    
    printf("[%ld]\n",now_ms.time_since_epoch().count());
    mapTime.emplace(PA_TIME(now_ms.time_since_epoch().count(),strVal));
    usleep(1000);
    strVal = "E";
    now = Now;
    now_ms = std::chrono::time_point_cast<Milli>(now);    
    printf("[%ld]\n",now_ms.time_since_epoch().count());
    mapTime.emplace(PA_TIME(now_ms.time_since_epoch().count(),strVal));
    usleep(1000);

    now = std::chrono::system_clock::now();
    now_ms = std::chrono::time_point_cast<Milli>(now);    
    printf("[%ld]\n",now_ms.time_since_epoch().count());
    printf("[%ld]\n",now_ms.time_since_epoch().count()-5);
    MAP_TIME::iterator itEnd = mapTime.lower_bound(now_ms.time_since_epoch().count()-3);
    for(MAP_TIME::iterator it = mapTime.begin();it != itEnd; ++it)
    {
        printf("[%s]\n",it->second.data());
        sleep(1);
    }
    printf("[%ld:%ld]\n",sizeof(int),sizeof(long));

    return 0;
}
