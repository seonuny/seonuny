#include <stdio.h>
#include <string.h>

#define SBI_BINDING_IN_NFINST             (char*)"nfinst"

int main(int argc, char **argv)
{
    if(argv[1] == nullptr)
    {
        printf("%s header\n",argv[0]);
        return 0;
    }
    char *pszHeader = argv[1];
	int nLen = strlen(pszHeader);
	char szNFInstanceId[128+1];
	int nStart = 0;
	for(int i = 0 ; i < nLen;++i)
	{
		if(i== 0 || pszHeader[i] == ';')
		{
            if(i>0)
            {
                ++i;
            }
            printf("find[%d:%d]%s\n",nStart,i,pszHeader+i);
            char szDummy[128];
            snprintf(szDummy,sizeof szDummy,"%.*s",strlen(SBI_BINDING_IN_NFINST),pszHeader+nStart);
            printf(">>>>[%s]\n",szDummy);
			if(memcmp(pszHeader+nStart,SBI_BINDING_IN_NFINST,strlen(SBI_BINDING_IN_NFINST)) == 0)
			{
                printf("find[%d]%s\n",i, pszHeader+nStart+strlen(SBI_BINDING_IN_NFINST));
				snprintf(szNFInstanceId,sizeof szNFInstanceId,"%s",pszHeader+nStart+strlen(SBI_BINDING_IN_NFINST));
				break;
			}
			nStart = i;
		}
	}

    printf("[%s]\n",szNFInstanceId);
	return 0;
}
