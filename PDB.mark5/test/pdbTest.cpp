#include <stdio.h>
#include <chrono>

#include "BaseDefine.hpp"
#include "define.hpp"

#include "CPDBWorker.hpp"
#include "CPDUSession.hpp"

#include <map>


class CThread
{
    private:
        int               m_nLoopCnt;
        int               m_nIdx;
        PDB::CPDBConnMgr *m_plsConnMgr ;
        PDB::CPDBWorker   m_clsWorker;

        CPDUSessionSelect Select;
        CPDUSessionInsert Insert;
        CPDUSessionUpdate Update;
        CPDUSessionDelete Delete;

    public:
        CThread(int a_nLoopCnt);
        ~CThread();
    void Run(int a_nIdx);
    void Log(ST_SMF_SESSION_INFO &stSess,int a_nLogLevel=NDF_LOG_T_DEBUG);
    void SetData(ST_SMF_SESSION_INFO &stSess,time_t tCurr,int a_nI);
};

CThread::CThread(int a_nLoopCnt)
{
    m_plsConnMgr = PDBConnMgrSTPtr();
    m_nLoopCnt = a_nLoopCnt;
    m_clsWorker.Assign(m_plsConnMgr);
    m_clsWorker.TurnOn();
    _THD_LOG(NDF_LOG_T_DEBUG,"[%d/%d]"
            ,m_plsConnMgr->GetCurConnCnt()
            ,m_plsConnMgr->GetMaxConnCnt()
            );
}

CThread::~CThread()
{
    m_clsWorker.TurnOff();
    _THD_LOG(NDF_LOG_T_DEBUG,"%d [%d/%d]"
            ,m_nIdx
            ,m_plsConnMgr->GetCurConnCnt()
            ,m_plsConnMgr->GetMaxConnCnt()
		  );
}

void CThread::SetData(ST_SMF_SESSION_INFO &stSess,time_t tCurr,int a_nI)
{
    SNPRINTF(stSess.szSMPolicyID      ,"%s%d%lu%06d","12010103000-02-16297678700939083809390e3d",m_nIdx,tCurr,a_nI);
    SNPRINTF(stSess.szSUPI            ,"%s%d%06d","imsi-450050211073",m_nIdx,a_nI);
    stSess. nPDUSessionID    = 1;
    SNPRINTF(stSess.szGPSI            ,"%s%d%06d","msisdn-82100",m_nIdx,a_nI); 
    SNPRINTF(stSess.szMDN             ,"%s%d%06d","82100",m_nIdx,a_nI); 
    SNPRINTF(stSess.szIpAddr          ,"%d.%d.%d.%d",a_nI%1111,a_nI%222,a_nI%123,a_nI%321);
    SNPRINTF(stSess.szDNN             ,"%s","5g.sktelecom.com");
    stSess. nSNssaiSst       = 100; 
    SNPRINTF(stSess.szSNssaiSd        ,"%s%03d","103",a_nI%1000);
    SNPRINTF(stSess.szLocId           ,"%s%04d:%d","10",a_nI%10000,a_nI%100);
    SNPRINTF(stSess.szMCCMNC          ,"%s","45005");
    SNPRINTF(stSess.szRatType         ,"%s","SA");
    SNPRINTF(stSess.szOcsSubsStatus   ,"%s","1");
    stSess.szInHttpHeader[0] = 0x00;
    SNPRINTF(stSess.szInHttpPayload   ,"%s","{\"smfId\":\"unknown\"}");
    SNPRINTF(stSess.szOutHttpPayload  ,"%s","{\"zoneInfos\":\"0000000000\",\"pccRules\":{\"QoS_0K\":{\"refQosData\":[\"0K\"],\"pccRuleId\":\"QoS_0K\"}}}");
    SNPRINTF(stSess.szResUri          ,"%s%d%06d","http://192.168.15.57:16101/npcf-smpolicycontrol/v1/sm-policies/12010103000-02-16297678700939083809390e3d0e3",m_nIdx,a_nI); 
    SNPRINTF(stSess.szNotifyUri       ,"%s","http://192.168.10.246:30011/npcf-smpolicycontrol/v1/sm-policies");
    SNPRINTF(stSess.szUDRNotiUri      ,"%s%d%06d","http://192.168.15.57:21301/npcf-event-exposure/v1/nudr-smf-notify/12010103000-02-16297678700939083809390e3d0e3",m_nIdx,a_nI); 
    stSess.szTMNotiUri[0]    = 0x00;
    SNPRINTF(stSess.szStatus          ,"%s","3");
    stSess.szAFSubscribe[0]  = 0x00;
    SNPRINTF(stSess.szNodeID          ,"%s","pcf76-mp01-app01");
    SNPRINTF(stSess.szProcID          ,"%s","SMF.MGR.76");
    SNPRINTF(stSess.szSMFId           ,"%s","550e8400-e29b-41d4-a716-446655440002");
    stSess. nConnectionID    = 0; 
    stSess. nStreamID        = 1;
    stSess. tCreateTime      = time(NULL);
    stSess. tUpdateTime      = time(NULL);
    SNPRINTF(stSess.szDesc            ,"%s","manual insert");
    return;
}

void CThread::Log(ST_SMF_SESSION_INFO &stSess,int a_nLogLevel)
{
#if 0
    if((NDF_GET_LOG_MASK() & a_nLogLevel) == a_nLogLevel)
    {
        _THD_LOG(a_nLogLevel,"    SM_POLICY_ID[%s]" ,stSess.szSMPolicyID     ); 
        _THD_LOG(a_nLogLevel,"            SUPI[%s]" ,stSess.szSUPI           ); 
        _THD_LOG(a_nLogLevel,"  PDU_SESSION_ID[%d]" ,stSess. nPDUSessionID   ); 
        _THD_LOG(a_nLogLevel,"            GPSI[%s]" ,stSess.szGPSI           ); 
        _THD_LOG(a_nLogLevel,"             MDN[%s]" ,stSess.szMDN            ); 
        _THD_LOG(a_nLogLevel,"         IP_ADDR[%s]" ,stSess.szIpAddr         ); 
        _THD_LOG(a_nLogLevel,"             DNN[%s]" ,stSess.szDNN            ); 
        _THD_LOG(a_nLogLevel,"     S_NSSAI_SST[%d]" ,stSess. nSNssaiSst      ); 
        _THD_LOG(a_nLogLevel,"      S_NSSAI_SD[%s]" ,stSess.szSNssaiSd       ); 
        _THD_LOG(a_nLogLevel,"          LOC_ID[%s]" ,stSess.szLocId          ); 
        _THD_LOG(a_nLogLevel,"         MCC_MNC[%s]" ,stSess.szMCCMNC         ); 
        _THD_LOG(a_nLogLevel,"        RAT_TYPE[%s]" ,stSess.szRatType        ); 
        _THD_LOG(a_nLogLevel," OCS_SUBS_STATUS[%s]" ,stSess.szOcsSubsStatus  ); 
        _THD_LOG(a_nLogLevel,"  IN_HTTP_HEADER[%s]" ,stSess.szInHttpHeader   ); 
        _THD_LOG(a_nLogLevel," IN_HTTP_PAYLOAD[%s]" ,stSess.szInHttpPayload  ); 
        _THD_LOG(a_nLogLevel,"OUT_HTTP_PAYLOAD[%s]" ,stSess.szOutHttpPayload ); 
        _THD_LOG(a_nLogLevel,"         RES_URI[%s]" ,stSess.szResUri         ); 
        _THD_LOG(a_nLogLevel,"        NOTI_URI[%s]" ,stSess.szNotifyUri      ); 
        _THD_LOG(a_nLogLevel,"    UDR_NOTI_URI[%s]" ,stSess.szUDRNotiUri     ); 
        _THD_LOG(a_nLogLevel,"     TM_NOTI_URI[%s]" ,stSess.szTMNotiUri      ); 
        _THD_LOG(a_nLogLevel,"          STATUS[%s]" ,stSess.szStatus         ); 
        _THD_LOG(a_nLogLevel,"    AF_SUBSCRIBE[%s]" ,stSess.szAFSubscribe    ); 
        _THD_LOG(a_nLogLevel,"         NODE_ID[%s]" ,stSess.szNodeID         ); 
        _THD_LOG(a_nLogLevel,"         PROC_ID[%s]" ,stSess.szProcID         ); 
        _THD_LOG(a_nLogLevel,"          SMF_ID[%s]" ,stSess.szSMFId          ); 
        _THD_LOG(a_nLogLevel,"         CONN_ID[%d]" ,stSess. nConnectionID   ); 
        _THD_LOG(a_nLogLevel,"       STREAM_ID[%d]" ,stSess. nStreamID       ); 
        _THD_LOG(a_nLogLevel,"     CREATE_TIME[%ld]",stSess. tCreateTime     ); 
        _THD_LOG(a_nLogLevel,"     UPDATE_TIME[%ld]",stSess. tUpdateTime     ); 
        _THD_LOG(a_nLogLevel,"     DESCRIPTION[%s]" ,stSess.szDesc           ); 
    }
#endif
}

void CThread::Run(int a_nIdx)
{
    ST_SMF_SESSION_INFO *pstSess = nullptr;
    ST_SMF_SESSION_INFO stSess;

    m_nIdx = a_nIdx;

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now() ;
    _THD_LOG(NDF_LOG_T_DEBUG,"start");
    time_t tCurr = time(NULL);
    unsigned long ulCnt = 0;
    for(int i = 0 ; i < m_nLoopCnt; ++i)
    {
        if(i % 1000 == 0)
        {
            _THD_LOG(NDF_LOG_T_DEBUG,"cycle[%d]",i);
        }
        memset(&stSess,0x00,sizeof(ST_SMF_SESSION_INFO));
        SetData(stSess,tCurr,i+1);
        Insert.SetValue(&stSess);
        if(m_clsWorker.Execute(Insert) == true)
        {
            ++ulCnt;
            Log(stSess);
          //_THD_LOG(NDF_LOG_T_DEBUG,"[%s]",stSess.szSUPI);
        }
        else
        {
          //_THD_LOG(NDF_LOG_T_INFO,"[%d][%s]",i,Insert.GetODBCErrMsg());
            Delete.SetValue(stSess.szSMPolicyID);
            if(m_clsWorker.Execute(Delete) == true)
            {
                ++ulCnt;
                //_THD_LOG(NDF_LOG_T_DEBUG,"[%s]",pstSess->szSUPI);
            }
            else
            {
                _THD_LOG(NDF_LOG_T_INFO,"[%d][%s]",i,Delete.GetODBCErrMsg());
                return;
            }
            Insert.SetValue(&stSess);
            if(m_clsWorker.Execute(Insert) == true)
            {
                ++ulCnt;
                Log(stSess);
            }
            else
            {
                _THD_LOG(NDF_LOG_T_INFO,"[%d][%s]",i,Insert.GetODBCErrMsg());
                return;
            }
        }
        Select.SetValue(stSess.szSMPolicyID);
        if(m_clsWorker.Execute(Select) == true)
        {
            ++ulCnt;
            pstSess = Select.GetValue();
            Log(*pstSess);
          //H_LOG((char*)"SESS",(unsigned char*)pstSess,sizeof(ST_SMF_SESSION_INFO));
          //_THD_LOG(NDF_LOG_T_DEBUG,"[%s]",pstSess->szSUPI);
        }
        else
        {
            _THD_LOG(NDF_LOG_T_INFO,"[%d][%s]",i,Select.GetODBCErrMsg());
            return;
        }

      //_THD_LOG(NDF_LOG_T_DEBUG,"%s",pstSess->szIpAddr);
        snprintf(pstSess->szIpAddr,sizeof pstSess->szIpAddr,"%s","0.0.0.0");
        Update.SetValue(pstSess);
        if(m_clsWorker.Execute(Update) == true)
        {
            ++ulCnt;
          //_THD_LOG(NDF_LOG_T_DEBUG,"[%s]",pstSess->szSUPI);
        }
        else
        {
            _THD_LOG(NDF_LOG_T_INFO,"[%d][%s]",i,Update.GetODBCErrMsg());
            return;
        }

        Select.SetValue(pstSess->szSMPolicyID);
        if(m_clsWorker.Execute(Select) == true)
        {
            ++ulCnt;
            pstSess = Select.GetValue();
            Log(*pstSess);
          //_THD_LOG(NDF_LOG_T_DEBUG,"[%s]",pstSess->szSUPI);
        }
        else
        {
            _THD_LOG(NDF_LOG_T_INFO,"[%d][%d:%s]",i,Select.GetSQLReturn(),Select.GetODBCErrMsg());
            return;
        }

        Delete.SetValue(pstSess->szSMPolicyID);
      //Delete.SetValue(stSess.szSMPolicyID);
        if(m_clsWorker.Execute(Delete) == true)
        {
            ++ulCnt;
          //_THD_LOG(NDF_LOG_T_DEBUG,"[%s]",pstSess->szSUPI);
        }
        else
        {
            _THD_LOG(NDF_LOG_T_INFO,"[%d][%s]",i,Delete.GetODBCErrMsg());
            return;
        }

      //_THD_LOG(NDF_LOG_T_DEBUG,"idx[%d]",i);
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now() ;
    std::chrono::duration<double>         duration ;
    duration = end - start;
    _THD_LOG(NDF_LOG_T_WARNING,"[%d] ThreadEnd elaped[%lu][%lf/%lf]",m_nIdx,ulCnt,duration.count(),ulCnt/duration.count());
    return ;
}

int main(int argc, char **argv)
{

    if (argv[1] == nullptr || argv[2] == nullptr)
    {
        printf("Usage: %s [thread count] [loop count]\n",argv[0]);
        return -1;
    }


    int nLogLevel = 15;
    if(argv[3] != nullptr)
    {
        nLogLevel = strtoul(argv[3],NULL,10);
    }
    printf("log level [%d]\n",nLogLevel);


  //NDF_OPEN_SERVICE_LOG(nLogLevel);
    NDF_OPEN_SERVICE_LOG((char*)"./",(char*)"log.log",2,nLogLevel,true,(100*1024*1024)) ;  
    PDB::CPDBConnMgr *plsConnMgr =nullptr;

    plsConnMgr = PDBConnMgrSTPtr();

    plsConnMgr->SetConfig((char*)"./pdb.json");

    plsConnMgr->ThreadStart();

    int nThdCnt = atoi(argv[1]);
    int nLoopCnt = atoi(argv[2]);

    _THD_LOG(NDF_LOG_T_DEBUG,"[%d:%d]",nThdCnt,nLoopCnt);
    CThread **thd = new CThread*[nThdCnt];
    std::thread **t = new std::thread*[nThdCnt];
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now() ;
    for (int i =0 ; i < nThdCnt ; ++i)
    {
        thd[i] = new CThread(nLoopCnt);
        t[i] = new std::thread(&CThread::Run,thd[i],i);
    }



    for(int i = 0 ; i < nThdCnt; ++i)
    {
        t[i]->join();
    }
    _THD_LOG(NDF_LOG_T_INFO,"[Thread End]");

    for(int i = 0; i < nThdCnt; ++i)
    {
        delete thd[i];
        delete t[i];
    }
    delete [] thd;
    delete [] t;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now() ;
    std::chrono::duration<double>         duration ;
    duration = end - start;
    _THD_LOG(NDF_LOG_T_WARNING,"elaped[%lf]",duration.count());
    printf("elaped[%lf]\n",duration.count());

    plsConnMgr->ThreadStop();
    
    while(plsConnMgr->IsActive() == true)
    {
        sleep(1);
    }
  //delete  plsConnMgr;
  //plsConnMgr = nullptr;
    return 0;
}
