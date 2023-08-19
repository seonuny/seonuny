#include <stdio.h>
#include <string.h>

#include <string>

int main(int argc, char **argv)
{
    char *pszHFC = (char*)"45008-4194303:16383,AB123C|45008-4194303:16384,AB123C|45008-4194303:16385,AB123C|45008-4194303:16386,AB123C|45008-4194303:16387,AB123C|45008-4194303:16388,AB123C|45008-4194303:16389,AB123C|45008-4194303:16390,AB123C|45008-4194303:16391,AB123C|45008-4194303:16393,AB123C|45008-4194303:16394,AB123C|45008-4194303:16413,AB123C|45008-4194303:16395,AB123C|45008-4194303:16396,AB123C|45008-4194303:16397,AB123C|45008-4194303:16398,AB123C|45008-4194303:16399,AB123C|45008-4194303:16400,AB123C|45008-4194303:16401,AB123C|45008-4194303:16402,AB123C|45008-4194303:16403,AB123C|45008-4194303:16404,AB123C|45008-4194303:16405,AB123C|138735:41,3710|45006-오후 4: :15,AB123C|45008-4194303:16406,AB123C|45008-4194303:16407,AB123C|45008-4194303:16409,AB123C|45008-4194303:16410,AB123C|4194303:16394,ABCD|45008-4194303:16411,AB123C";
    
    if(argc!=2)
    {
        printf("%s [loc_id]\n",argv[0]);
        return 0;
    }
    char *pszLocId  = argv[1];
    printf("[%s]\n",pszHFC);
    
    std::string stringHFCList = std::string(pszHFC);
    int nLenLocId = strlen(pszLocId);
    int nPosition = stringHFCList.find(pszLocId);

    printf("[%d:%d]\n",nLenLocId,nPosition);
    if(nPosition == std::string::npos)
    {
        printf("1nothing[%s]",pszLocId);
    }
    else
    {
        printf("[%s]\n",pszHFC+nPosition+nLenLocId);
        if(stringHFCList[nPosition-1]=='|' &&
                stringHFCList[nPosition+nLenLocId]==',')
        {
            printf("1find[%s]",pszLocId);
        }
        else
        {
            nPosition = stringHFCList.find(pszLocId,nPosition+1);
    printf("[%d:%d]\n",nLenLocId,nPosition);
            if(nPosition == std::string::npos)
            {
                printf("2nothing[%s]",pszLocId);
            }
            else
            {
                printf("[%s]\n",pszHFC+nPosition+nLenLocId);
              //printf("[%s]\n",stringHFCList[nPosition+nLenLocId]);
                if(stringHFCList[nPosition+nLenLocId]==',')
                {
                printf("2find[%s]",pszLocId);
                }
            }
        }
    }


    return 0;

}
