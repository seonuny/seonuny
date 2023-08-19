#include <stdio.h>
#include <set>


class CPDBConn
{
    int nUUID;
};

class CPDBConnAll
{
    public:
	    bool      bMain;          /**< DB active 플래그 */
	    unsigned int uWorkerID;   /**< Worker ID */
	    CPDBConn *active;         /**< main/sub 중 활성 conn을 저장 */
	    CPDBConn *main;           /**< main conn */
	    CPDBConn *sub;            /**< sub conn */
        CPDBConnAll()
        {
            printf("Construct\n");
        }
        ~CPDBConnAll()
        {
            printf("Disstruct\n");
        }
        bool operator<(CPDBConnAll &rhs) const 
        {
            printf(">>[%d<%d]\n",uWorkerID,rhs.uWorkerID);
            return uWorkerID<rhs.uWorkerID;
        }
        bool operator>(CPDBConnAll &rhs) const 
        {
            printf(">>[%d>%d]\n",uWorkerID,rhs.uWorkerID);
            return uWorkerID>rhs.uWorkerID;
        }
        bool operator()(CPDBConnAll &rhs) const 
        {
            printf(">>[%d<%d]\n",uWorkerID,rhs.uWorkerID);
            return uWorkerID<rhs.uWorkerID;
        }
} ;                /**< PDB Conn 구조체 */

// 직접 추가한 비교용 구조체
template <typename T>
struct CustomOrder
{
	bool operator() (T* const & lhs, T* const & rhs) const
	{
        return *lhs<*rhs;
	}
};

int main(int argc, char **argv)
{
    std::set<CPDBConnAll*,CustomOrder<CPDBConnAll>> setConn;
  //std::set<CPDBConnAll*> setConn;
    CPDBConnAll *pdbConn = new CPDBConnAll;
    pdbConn->uWorkerID = 1;
    setConn.emplace(pdbConn);
#if 1
    pdbConn = new CPDBConnAll;
    pdbConn->uWorkerID = 3;
    setConn.emplace(pdbConn);
    pdbConn = new CPDBConnAll;
    pdbConn->uWorkerID = 4;
    setConn.emplace(pdbConn);
    pdbConn = new CPDBConnAll;
    pdbConn->uWorkerID = 2;
    setConn.emplace(pdbConn);
    pdbConn = new CPDBConnAll;
    pdbConn->uWorkerID = 5;
    setConn.emplace(pdbConn);
#endif

    CPDBConnAll* pdb2 = new CPDBConnAll;
    pdb2->uWorkerID = 6;

    if(pdbConn < pdb2)
    {
        printf("[%d:%d]\n",pdbConn->uWorkerID,pdb2->uWorkerID);
    }

    for(CPDBConnAll *pdbC : setConn)
    {
        printf("[%u]\n",pdbC->uWorkerID);
    }

    for(CPDBConnAll *pdbC : setConn)
    {
        delete pdbC;
    }
	return 0;
}
