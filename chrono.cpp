#include <iostream>
#include <chrono>

int main ()
{
    using namespace std::chrono;
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  //auto now_ms = time_point_cast<std::chrono::milliseconds>(now);
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> now_ms = time_point_cast<std::chrono::milliseconds>(now);    

    printf("%ld\n",now_ms.time_since_epoch().count());
    auto value = now_ms.time_since_epoch();
    long duration = value.count();

    milliseconds dur(duration);

    time_point<system_clock> dt(dur);

    if (dt != now_ms)
        std::cout << "Failure." << std::endl;
    else
        std::cout << "Success."<<std::endl<<value.count()<<std::endl<<duration<<std::endl<<dur.count() << std::endl;
}
