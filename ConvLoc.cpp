/*
 * build 
 * g++ -o convLoc ConvLoc.cpp -std=c++14
 * example
 * ./convLoc 45005 21DEFA7 L
 * ./convLoc 45005 138735:41 L 
 */
#include <stdio.h>

#include <regex>
#include <string>

enum {
	H_LTE = 1,
	H_5G     ,
	C_LTE    ,
	C_5G     ,
} ;

/**
 * \brief
 * \detail
 * \param[in]
 * \return
 *
 * */
bool HexStrToHex(
		const char *a_pszHexStr, unsigned char *a_pszHex,int a_nOutHexLen)
{
	bool bRtn = true;
	try
	{
		int nHexStrIdx = strlen((char*)a_pszHexStr);
		unsigned char szHexStr[16];
		snprintf((char*)szHexStr,sizeof szHexStr,"%s",a_pszHexStr);

		char szHex[2+1];
		int nHexIdx = 0;
		if((nHexStrIdx%2) == 0)
		{
			for(int i = 0 ; i< nHexStrIdx; )
			{
				snprintf(szHex, sizeof szHex,"%.*s",2,szHexStr+i);
				a_pszHex[nHexIdx] = strtoul(szHex,(char**)NULL,16);
				++nHexIdx;
				i = i+2;
			}
		}
		else
		{
			for(int i = 0 ; i< nHexStrIdx; )
			{
				if(i == 0)
				{
					snprintf(szHex, sizeof szHex,"%.*s",1,szHexStr+i);
					i = i+1;
				}
				else
				{
					snprintf(szHex, sizeof szHex,"%.*s",2,szHexStr+i);
					i = i+2;
				}
				a_pszHex[nHexIdx] = strtoul(szHex,(char**)NULL,16);
				++nHexIdx;
			}
		}
	}
	catch(std::exception &e)
	{
		bRtn = false;
	}

	return bRtn;
}
/**
 * \brief
 * \detail
 * \param[in]
 * \return
 *
 * */
int GetLocationKeyforLTE(unsigned char* input, char* output)
{
	unsigned int eNB, nw;

	//11111111 11111111 11111111 11111111
	//    1111 11111111 11111111 11111111
	eNB = ((input[0] & 0x0c)>>2);
	if(eNB < 2)
	{
		// eNodeBID = eNB ID(20)
		// Cell ID  = Cell(6)
		unsigned int uneNB,unCellID;
		uneNB = ((input[0]&0X0F)<<16) | (input[1]<<8) | input[2];
		unCellID = ((input[3]&0xFC)>>2);
		nw = (input[3] & 0X03) ;

		//sprintf(output,"%d:%d:%d",uneNB,unCellID,nw);
		sprintf(output,"%d:%d",uneNB,unCellID);
	}
	else // HeNB Type
	{
		unsigned int unHeNBID;
		nw = (input[3] & 0X03) ;
		unHeNBID = ((input[0] & 0X0F)<<24) +  (input[1]<<16) + (input[2]<<8) + ((input[3]&0XFF));
		sprintf(output, "%u", unHeNBID);
	}

	return nw;
}

/**
 * \brief
 * \detail
 * \param[in]
 * \return
 *
 * */
void GetLocationKeyfor4G(unsigned char* input, char* output)
{
	unsigned int uneNB,unCellID;

	//11111111 11111111 11111111 11111111
	//    1111 11111111 11111111 11111111
	// eNodeBID = eNB ID(20)
	// Cell ID  = Cell(8)
	uneNB = ((input[0]&0X0F)<<16) | (input[1]<<8) | input[2];
	unCellID = input[3];

	sprintf(output,"%d:%d",uneNB,unCellID);

	return ;
}

/**
 * \brief
 * \detail
 * \param[in]
 * \return
 *
 * */
int GetLocationKeyfor5G(unsigned char* input, char* output)
{
	unsigned int gNB ;

	//11111111 11111111 11111111 11111111 11111111
	//    1111 11111111 11111111 11111111 11111111
	//    1231 23123456 78123456 78123456 78123456
	gNB = ((input[0] & 0x0E)>>1);
	if(gNB == 1) // CU-DUH
	{
		// gNodeBID = gNB ID(22)
		// 1234 56789012 34567890 12
		// Cell ID  = Cell(14)
		unsigned int ungNB,unCellID;
		ungNB = ((input[0]&0X0F)<<18) | (input[1]<<10) | input[2]<<2 | (input[3]&0xC0)>>6;
		unCellID = ((input[3]&0x3F)<<8 | input[4]);

		sprintf(output,"%d:%d",ungNB,unCellID);
	}
	else if(gNB == 2)
	{
		// gNodeBID = gNB ID(22)
		// 1234 56789012 34567890 12
		// Cell ID  = Cell(11)
		unsigned int ungNB,unCellID;
		ungNB = ((input[0]&0X0F)<<18) | (input[1]<<10) | input[2]<<2 | (input[3]&0XC0)>>6;
		unCellID = ((input[3]&0x3F)<<8 | input[4]);
		//unCellID = ((input[3]&0x07)<<8 | input[4]);

		sprintf(output,"%d:%d",ungNB,unCellID);
	}
	else
	{
		// gNodeBID = gNB ID(22)
		// 1234 56789012 34567890 12
		// Cell ID  = Cell(14)
		unsigned int ungNB,unCellID;
		ungNB = ((input[0]&0X0F)<<18) | (input[1]<<10) | input[2]<<2 | (input[3]&0xC0)>>6;
		unCellID = ((input[3]&0x3F)<<8 | input[4]);

		sprintf(output,"%d:%d",ungNB,unCellID);
	}

	return 1;
}



int main(int argc, char **argv)
{
	if(argc != 4)
	{
		printf("%s [plmn] [loc] [ratType]\n",argv[0]);
		printf("\tplmn : 45005,45006,45008,45012..\n");
		printf("\tloc : 12345:12,1234123,1234,00123ac,1234acce9..\n");
		printf("\tratType : S,L\n");
		return 0;
	}
	/*
	 * 당사 기준으로는 ECGI 28 bits 중에서..
	 * 가장 큰 값이 1인 경우.. most significant bit .. Femto Cell 로 구분합니다.
	 * 즉, ECGI 값이 2^27 (134217728) 보다 같거나 크면 Femto Cell 인거죠.
	 * eNB ID는 ECGI 중에서 상위 20bits 이므로 (256으로 나눈 몫) ..
	 * eNB ID로 계산할때는 2^19 (524288) 보다 같거나 크면 Femto Cell 입니다.
	 * 물론 표준은 아니고, 당사 자체 규약입니다.
     * featuring : jhkoo@sk.com
	 */

	char *pszPLMN    = argv[1];
	char *pszRawLOC  = argv[2];
	char *pszRatType = argv[3];
	char   szLocId[32+1];
	std::regex eHexNr ("[a-fA-F0-9]{9}");
	std::regex eHexEutra ("[a-fA-F0-9]{7}");
	std::string stringRawLoc = pszRawLOC;

    int nType = 0;
    if(std::regex_match(stringRawLoc,eHexNr))
    {
        /*
         * Femtocell 최소 크기가 9자리
         */
        nType = pszRatType[0] == 'S' ? H_5G : C_LTE;
    }
    else if(std::regex_match(stringRawLoc,eHexEutra))
    {
        nType = H_LTE;
    }
    else if(pszRatType[0] == 'S')
    {
        nType = C_5G;
    }
    else
    {
        nType = C_LTE;
    }

	switch(nType)
	{
        case H_5G:
            {
                std::regex eNr ("[a-fA-F0-9]{9}");
                if(std::regex_match(stringRawLoc,eNr))
                {
                    unsigned char szNRCellIdRaw[8];
                    memset(szNRCellIdRaw, 0x00, 8);
                    HexStrToHex(pszRawLOC,
                            szNRCellIdRaw,8);
                    GetLocationKeyfor5G(szNRCellIdRaw, szLocId);
                    printf("[%s]\n",szLocId);
                }
                else
                {
                    printf("nrCellID length(9) error [9!=%d]\n",stringRawLoc.length());
                }
            }
			break;
		case H_LTE:
			{
				std::regex eNr ("[a-fA-F0-9]{7}");
				if(std::regex_match(stringRawLoc,eNr))
				{
					unsigned char szEutraCellIdRaw[4];
					memset(szEutraCellIdRaw, 0x00, 4);
					HexStrToHex(pszRawLOC,
							szEutraCellIdRaw,4);
					if(strstr((char*)"45005,45012",pszPLMN) != nullptr)
					{
						GetLocationKeyforLTE(szEutraCellIdRaw, szLocId);
					}
					else
					{
						GetLocationKeyfor4G(szEutraCellIdRaw, szLocId);
					}
					printf("[%s]\n",szLocId);
				}
				else
				{
                    std::regex e("[0-9]+");
                    if(std::regex_match(stringRawLoc,e))
                    {
                        unsigned long lEutraCellId;
                        lEutraCellId = strtoul(pszRawLOC,(char**)NULL,10);
                        printf("[%07lx]\n",lEutraCellId);
                    }
                    else
                    {
                        printf("eutraCellID length(7) error [7!=%d]\n",stringRawLoc.length());
                    }
				}
			}
			break;
		case C_5G:
			{
				int nOffset = stringRawLoc.find(":");
				char szgNBID[22+1]={0,};
				char szCellID[14+1]={0,};
				unsigned long lgNBID;
				unsigned int  nCellID;
				unsigned long lNrCellId;
				snprintf(szgNBID,sizeof szgNBID,"%s",stringRawLoc.substr(0,nOffset).data());
				snprintf(szCellID,sizeof szCellID,"%s",stringRawLoc.substr(nOffset+1).data());
				lgNBID  = strtoul(szgNBID,(char**)NULL,10);
				nCellID = strtoul(szCellID,(char**)NULL,10);
				if(lgNBID>4194303)
				{
					printf("gNBID overflow[4194303<%lu]\n",lgNBID);
					return 0;
				}
				else if(nCellID>16383)
				{
					printf("CellID overflow[16383<%u]\n",nCellID);
					return 0;
				}
				lgNBID = lgNBID<<14;
				lNrCellId = lgNBID + nCellID;
				printf("[%09lx]\n",lNrCellId);

			}
			break;
		case C_LTE:
			{
				int nOffset = stringRawLoc.find(":");
                if(nOffset == std::string::npos)
                {
                    unsigned long lEutraCellId;
                    lEutraCellId = strtoul(pszRawLOC,(char**)NULL,10);
                    printf("Femtocell[%07lx]\n",lEutraCellId);
                }
                else
                {
                    char szgNBID[22+1]={0,};
                    char szCellID[14+1]={0,};
                    unsigned long leNBID;
                    unsigned int  nCellID;
                    unsigned long lEutraCellId;
                    snprintf(szgNBID,sizeof szgNBID,"%s",stringRawLoc.substr(0,nOffset).data());
                    snprintf(szCellID,sizeof szCellID,"%s",stringRawLoc.substr(nOffset+1).data());
                    leNBID  = strtoul(szgNBID,(char**)NULL,10);
                    nCellID = strtoul(szCellID,(char**)NULL,10);
                    if(leNBID>1048575)
                    {
                        printf("eNBID overflow[1048575<%lu]\n",leNBID);
                        return 0;
                    }
                    if(strstr((char*)"45005,45012",pszPLMN) != nullptr)
                    {
                        if(nCellID>63)
                        {
                            printf("CellID overflow[63<%u]\n",nCellID);
                            return 0;
                        }
                        nCellID = nCellID*4+3;
                    }
                    else if(nCellID>255)
                    {
                        printf("CellID overflow[255<%u]\n",nCellID);
                        return 0;
                    }
                    leNBID = leNBID<<8;
                    lEutraCellId = leNBID + nCellID;
                    printf("[%07lx]\n",lEutraCellId);
                }
			}
			break;
	}

	return 0;
}
