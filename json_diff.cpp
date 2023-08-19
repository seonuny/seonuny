#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <iostream>

#include "rapidjson.h"
#include "pointer.h"
#include "document.h"
#include "error/en.h"     // for stringify JSON 

int main(int argc, char **argv)
{
    rapidjson::Document srcDoc;
    rapidjson::Document dstDoc;

    char szSrcJsonStr[1024] = {0,};
    char szDstJsonStr[1024] = {0,};

    FILE *pfSrc = NULL;
    FILE *pfDst = NULL;
    
    pfSrc = fopen(argv[1],"r");
    if(pfSrc == NULL)
    {
        printf("[%s] src\n",strerror(errno));
        return -1;
    }
    pfDst = fopen(argv[2],"r");
    if(pfDst == NULL)
    {
        printf("[%s] dst\n",strerror(errno));
        return -1;
    }

    size_t nReadSrc = fread(szSrcJsonStr,1,1024,pfSrc);

    size_t nReadDst = fread(szDstJsonStr,1,1024,pfDst);

    printf("[%u][%u]\n",nReadSrc,nReadDst);
    fclose(pfSrc);
    fclose(pfDst);

    if(srcDoc.Parse(szSrcJsonStr).HasParseError())
    {
        printf("[%s] src1",rapidjson::GetParseError_En(srcDoc.GetParseError()));
        return -1;
    }
    if(dstDoc.Parse(szDstJsonStr).HasParseError())
    {
        printf("[%s] dst",rapidjson::GetParseError_En(dstDoc.GetParseError()));
        return -2;
    }

    char szJsonPath[1024];
    snprintf(szJsonPath,1024,"%s",(char*)"/pccRules");
    rapidjson::Value* srcValue = rapidjson::Pointer(szJsonPath).Get(srcDoc);
    rapidjson::Value* dstValue = rapidjson::Pointer(szJsonPath).Get(dstDoc);

    if(srcValue == nullptr && dstValue == nullptr)
    {
        printf("same\n");
    }
    else if(srcValue == nullptr || dstValue == nullptr)
    {
        printf("difference\n");
    }
    else
    {
        if(*srcValue == *dstValue)
        {
            printf("same\n");
        }
        else
        {
            printf("difference\n");
        }
    }
    snprintf(szJsonPath,1024,"%s",(char*)"/zoneInfos");
    srcValue = rapidjson::Pointer(szJsonPath).Get(srcDoc);
    dstValue = rapidjson::Pointer(szJsonPath).Get(dstDoc);
    if(srcValue == nullptr && dstValue == nullptr)
    {
        printf("same\n");
    }
    else if(srcValue == nullptr || dstValue == nullptr)
    {
        printf("difference\n");
    }
    else
    {
        if(*srcValue == *dstValue)
        {
            printf("same\n");
        }
        else
        {
            printf("difference\n");
        }
    }
    snprintf(szJsonPath,1024,"%s",(char*)"/quotaNoti");
    srcValue = rapidjson::Pointer(szJsonPath).Get(srcDoc);
    dstValue = rapidjson::Pointer(szJsonPath).Get(dstDoc);
    if(srcValue == nullptr && dstValue == nullptr)
    {
        printf("same\n");
    }
    else if(srcValue == nullptr || dstValue == nullptr)
    {
        printf("difference\n");
    }
    else
    {
        if(*srcValue == *dstValue)
        {
            printf("same\n");
        }
        else
        {
            printf("difference\n");
        }
    }
    if(srcDoc == dstDoc)
    {
        printf("same\n");
    }
    else
    {
        printf("difference\n");
    }

    return 0;
}
