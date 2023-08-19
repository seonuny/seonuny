#include <python2.7/Python.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include<stdio.h>

void print_memory(void)
{

static char buf[128] = {'\0'};
long vmsize, vmrss;
int rc;
FILE *fp;
fp = fopen("/proc/self/statm", "r");
if (!fp)
return;
  
/* Only fetch the first two entries. */
rc = fscanf(fp, "%ld%ld", &vmsize, &vmrss);
fclose(fp);

if (rc != 2)
return;
  
/* Convert to KB. */
vmsize *= 4;
vmrss *= 4;
snprintf(buf, sizeof(buf), "VmSize=%ldkB, VmRSS=%ldkB", vmsize, vmrss);
printf("VmSize=%ldkB, VmRSS=%ldkB\n", vmsize, vmrss);
return;

}

void start_interpreter(){
printf("before starting python interpreter\t");
print_memory();
Py_Initialize();
printf("after starting python interpreter\t");
print_memory();
return;
}

void close_interpreter(){
printf("before stopping python interpreter\t");
print_memory();
Py_Finalize();
printf("after stopping python interpreter\t");
print_memory();
return;
}





int main(int argc, char **argv) 
{
    printf("The memory being used before this program starts is : \n");
    print_memory();

    printf("\n");
    printf("\n");
    printf("\n");
    int i;
    int j;

    int time_passed_till_now=0;

    for(i=0;i<30;i++)
    {

        start_interpreter();
        PyRun_SimpleString("from time import time,ctime\n"
                "print ('Today is ctime(time())')\n");
        PyRun_SimpleString("from time import time,ctime\n"
                "print ('Today is ctime(time())')\n");
        PyRun_SimpleString("from time import time,ctime\n"
                "print ('Today is ctime(time())')\n");


        PyRun_SimpleString("import json as js\n");
        PyRun_SimpleString("a=90\n");
        PyRun_SimpleString("b=70\n");
        PyRun_SimpleString("c=32\n");
        PyRun_SimpleString("c=a+b/5+c*10+a\n");
        PyRun_SimpleString("d=a/2+a*5+b*a/5+c*10+a%4\n");
        PyRun_SimpleString("a=b+a/2+b%2*b+c-19+a/5+b*43\n");
        PyRun_SimpleString("b=70\n");		     


        //PyRun_SimpleString("import matplotlib.pyplot as plt\n");   

        PyRun_SimpleString("import cmath\n");              
        printf("\n");

        printf("\n");
        PyRun_SimpleString("import hashlib\n");   



        PyRun_SimpleString("import readline\n");
        PyRun_SimpleString("import subprocess\n");
        PyRun_SimpleString("import string\n");      

        close_interpreter();

        printf("\n");

        //Start the python interpreter every 120 seconds.
        sleep(120);

    }
    printf("\n");
    printf("\n");
    printf("\n");
    printf("The memory being used now at the end of the process is: \n");
    print_memory();

    return 0;

}
