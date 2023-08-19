#include <stdio.h>
#include <string.h>
#include <string>
#include <algorithm>

int main(int argc, char **argv)
{
    if(argv[1] == nullptr || argv[2] == nullptr)
    {
        printf("%s [nfipendpoints] [notify uri]\n",argv[0]);
        return 0;
    }
    char szSMFAuthority[128];
    int nIdx = 0;
    std::size_t start=0,end=0;
    std::string stringNFIPEndPoints = argv[1];
    std::string stringIPEndPoint;
    int m_nIPEndPointIdx = std::count(stringNFIPEndPoints.begin(),stringNFIPEndPoints.end(),',');
    bool bFnd = false;
	while((end=stringNFIPEndPoints.find_first_of(",",start)) != std::string::npos)
	{
		stringIPEndPoint = stringNFIPEndPoints.substr(start,end-1);
		if(nIdx == 0)
		{
			snprintf(szSMFAuthority,128,"%s",stringIPEndPoint.data());
		}
		printf("[%d:%d][%s]\n"
				,m_nIPEndPointIdx
				,nIdx
				,stringIPEndPoint.data());
		if(nIdx >=m_nIPEndPointIdx )
		{
			/*
			 * 아이피가 같은지 확인.
			 */
			if(strstr(argv[2],stringIPEndPoint.data()) == nullptr)
			{
                bFnd = true;
                printf("[%s:%s]\n",argv[2],stringIPEndPoint.data());
				snprintf(szSMFAuthority,128,"%s",stringIPEndPoint.data());
				break;
			}
			printf("same ip retry[%s:%s]\n"
					,stringIPEndPoint.data(),argv[2]);
			continue;
		}
		start = end+1;
		++nIdx;
	}
    if(bFnd == false)
    {
        printf("last\n");
        stringIPEndPoint = stringNFIPEndPoints.substr(start);
        if(strstr(argv[2],stringIPEndPoint.data()) == nullptr)
        {
            snprintf(szSMFAuthority,128,"%s",stringIPEndPoint.data());
        }
    }
    printf("[%s]\n",szSMFAuthority);

    return 0;
}
