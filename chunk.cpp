#include <stdio.h>
#include <string.h>

class CChunk
{
    public:
        int   m_nSize;
        char *m_pData;

    public:
        CChunk(): m_nSize(0), m_pData(nullptr)
        {
            printf("const\n");
        }
        CChunk(const CChunk& rhs)
        {
            printf("[%d:%s]\n",rhs.m_nSize,rhs.m_pData);
            this->m_nSize = rhs.m_nSize;
            if(rhs.m_nSize>0)
            {
                this->m_pData = new char [rhs.m_nSize]();
                memcpy(this->m_pData,rhs.m_pData,rhs.m_nSize);
            }
            else
            {
                m_pData = nullptr;
            }
        }
        ~CChunk()
        {
            printf("[%d:%s]\n",m_nSize,m_pData);
            if (m_pData != nullptr)
            {
                delete[] m_pData;
                m_pData = nullptr;
            }
        }
		// 복사 대입 연산자
	    CChunk &operator=(const CChunk& rhs)
	    {
            printf("[%d:%s]\n",rhs.m_nSize,rhs.m_pData);
            this->m_nSize = rhs.m_nSize;
            if(rhs.m_nSize>0)
            {
                this->m_pData = new char [rhs.m_nSize]();
                memcpy(this->m_pData,rhs.m_pData,rhs.m_nSize);
            }
            else
            {
                this->m_pData = nullptr;
            }
	    	return *this;
	    }
        void SetData(int a_nSize, char *a_pData)
        {
            if (m_pData == nullptr)
            {
                m_pData = new char [a_nSize]();
                memcpy(m_pData, a_pData, a_nSize);
                m_nSize = a_nSize;
            }
        };
        int GetSize()
        {
            return m_nSize;
        };
        char *GetDataPtr()
        {
            return m_pData;
        };
};


int main(int argc, char **argv)
{
    CChunk chunk ;
    chunk.SetData(strlen(argv[1]),argv[1]);
    CChunk chunk2;
    chunk2 = chunk;

    return 1;
}
