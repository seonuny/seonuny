#include <cstdarg>
#include <iostream>
using namespace std;
#include <cstdio>
#include <cstring>

//void varfunction(int argc, ...) {
void varfunction(const char* format, ...) {
    cout<<format<<endl;
    int number=0;
    const char* pos=format;
    while(  (pos=strchr(pos, '%'))!=NULL ) {
        ++number;
        ++pos;

    }

    va_list va;
    va_start(va, format);

    int value;

    for(int index=0;index<number;++index) {
      value=va_arg(va, int);
      printf("The argument is %d\n", value);
   }
   va_end(va);

}

 

int main() {
  varfunction("The first is %s and the second is %d.  %d  %d", 2, 3, 4, 5);

  return 0;

}
