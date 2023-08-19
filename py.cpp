#include <stdio.h>
#include <stdlib.h>
//#include <python2.7/Python.h> 
#include <Python.h> 

int main(int argc, char **argv)
{
    if(argc != 2 ) return 0;
    int nLoop = atoi(argv[1]);
    char * ptr = new char[10000];
    for(int i = 0 ; i < nLoop;++i)
    {
		Py_Initialize();

        Py_Finalize();
    }
    delete [] ptr;
	return 0;
}
