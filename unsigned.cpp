#include <stdio.h>

int main (int argc, char **argv)
{
    unsigned int unReqNumber = 10;
    unsigned int unReqNum    = 7;
  //int unReqNumber = 10;
  //int unReqNum    = 7;

    if(unReqNum - unReqNumber > 1)
    {
        printf("[over][%u]\n",unReqNum - unReqNumber);
    }
    else
    {
        printf("[order][%u]\n",unReqNum - unReqNumber);
    }

    return 0;
}
