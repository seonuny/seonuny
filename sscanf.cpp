#include <stdio.h>


int main(int argc, char **argv)
{
    char  szScheme[64+1];
    char  szHost[64+1];
    int    nPort;
    int nRtn = sscanf(argv[1],"%20[^:]://%64[^:]:%d",szScheme,szHost,&nPort);
    printf("[%s://%s:%d][%d]\n",szScheme,szHost,nPort,nRtn);

    char szENBID[10];
    int   nCellID;

    nRtn = sscanf(argv[2],"%10[^:]:%d",szENBID,&nCellID);

    printf("[%s:%d][%d]\n",szENBID,nCellID,nRtn);

    return 0;
}
