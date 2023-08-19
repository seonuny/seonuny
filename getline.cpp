#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>

#define HTTP_H_3GPP_SBI_BINDING           (char*)"3gpp-sbi-binding" 
#define SBI_BINDING_IN_BL                 (char*)"bl"
#define SBI_BINDING_IN_NFINST             (char*)"nfinst"
#define SBI_BINDING_IN_NFSET              (char*)"nfset"

int main(int argc, char **argv)
{
    if(argv[1] == nullptr)
    {
        printf("%s sbi-binding\n",argv[0]);
        return 0;
    }
    char *pszHeader = argv[1];
    std::istringstream ss (pszHeader);
    std::string token;
    std::string stringNfInstanceId;

    while(std::getline(ss,token,';'))
    {
        printf("[%s]\n" ,token.data());
        if(token.find(SBI_BINDING_IN_NFINST) != std::string::npos)
        {
            size_t uPos = token.find("=");
            if(uPos != std::string::npos)
            {
                stringNfInstanceId = token.substr(uPos+1);
                printf(">>>>>[%s]<<<<<\n" ,stringNfInstanceId.data());
              //break;
            }
        }
    }

    return 0;
}
