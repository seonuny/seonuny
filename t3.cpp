/******************************************
* FILE: t3.c
* Compiled using : g++ -g t3.c -o t3
*
* $ g++ -v
* Reading specs from /usr/lib/gcc/i686-pc-linux-gnu/3.4.6/specs
* Configured with: ./configure --prefix=/usr --infodir=/share/info --mandir=/share/man
*      --enable-languages=c,c++ --with-system-zlib --program-suffix=-3.4 --enable-threads=posix
* Thread model: posix
* gcc version 3.4.6
 ******************************************/


#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

void fin()
{
    printf("fin\n");
}

#define STR(x) #x

class Data
{
    private:
        std::string storage;
    public:
        Data() { storage.assign(STR(__func__)); }
        const char* get() { return storage.c_str(); }
};

/**************************************************************
 **************************************************************/
int main(int argc, char *argv[])
{
    Data *data1 = new Data();
    Data *data2 = new Data();
    Data *data3 = new Data();
    Data *data4 = new Data();
    Data *data5 = new Data();
    Data *data6 = new Data();
    Data  data7;

    delete data1;
    delete data2;
    delete data3;
    delete data4;
    delete data5;
    delete data6;
    string test1 ="abc";
    string test2 ="abc";
    string test3 ="abc";
    string test4 ="abc";
    string test5 ="abc";
    cout << "this is a test " << test1 << endl;
    cout << "data " << data7.get() << endl;
    alarm(5);
    sleep(4);
    alarm(0);
    printf("----\n");
    sleep(4);
    exit(0);
    //return 0;
}
