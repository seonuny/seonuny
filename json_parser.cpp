#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "rapidjson.h"
#include "document.h"     // rapidjson's DOM-style API
#include "prettywriter.h" // for stringify JSON
#include "error/en.h"     // for stringify JSON 

bool Parser(rapidjson::Value &obj,int nDepth=0);
void PrintJsonArray(rapidjson::Value &array,int a_nDepth=1)
{
    for(int i=0; i < (int)array.Size();i++)
    {
        switch(array[i].GetType())
        {
            case rapidjson::kNullType:
                break;
            case rapidjson::kFalseType:
                printf("%d,",array[i].GetBool());
                break;
            case rapidjson::kTrueType:
                printf("%d,",array[i].GetBool());
                break;
            case rapidjson::kObjectType:
                Parser(array[i],a_nDepth+1);
                break;
            case rapidjson::kArrayType:
                PrintJsonArray(array[i]);
                break;
            case rapidjson::kStringType:
                printf("%s,",array[i].GetString());
                break;
            case rapidjson::kNumberType:
                printf("%d,",array[i].GetInt());
                break;
        }
    }
  //printf("\n");

}
bool Parser(rapidjson::Value &obj,int nDepth)
{
    bool bRtn = true;
    for (rapidjson::Value::MemberIterator it = obj.MemberBegin(); it != obj.MemberEnd(); ++it)
    {
        switch(it->value.GetType())
        {
            case rapidjson::kNullType:
                break;
            case rapidjson::kFalseType:
                break;
            case rapidjson::kTrueType:
                break;
            case rapidjson::kNumberType:
                printf("%lf,",it->value.GetFloat());
                break;
            case rapidjson::kStringType:
                printf("%s,",it->value.GetString());
                break;
            case rapidjson::kArrayType:
                PrintJsonArray(it->value,nDepth+1);
                break;
            case rapidjson::kObjectType:
                printf("[%s]",it->name.GetString());
                Parser(it->value,nDepth+1);
                break;
        }
    }
    printf("\n");
    return bRtn;
}

int main(int argc, char **argv)
{
    if(argv[1] == nullptr)
    {
        printf("Usage: %s [json file]\n",argv[0]);
        return -1;
    }
    FILE *pfSrc = fopen(argv[1],"r");

    struct stat stFStat;
    int nFd = fileno(pfSrc);
    fstat(nFd,&stFStat);

    char *pszJsonStr = new char[stFStat.st_size+1];

    fread(pszJsonStr,stFStat.st_size,1,pfSrc);
    fclose(pfSrc);

    rapidjson::Document doc;

    if(doc.Parse(pszJsonStr).HasParseError())
    {
        printf("%s",rapidjson::GetParseError_En(doc.GetParseError()));
        return -2;
    }

    rapidjson::Value::MemberIterator member = doc.MemberBegin();

    Parser(doc,0);


    return 1;

}

