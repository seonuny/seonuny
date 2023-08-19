
#include "CBaseString.hpp"
#include "CPDUSession.hpp"


CPDUSessionInsert::CPDUSessionInsert()
{
    className_ = typeid(*this).name();
    //생성할 때 쿼리문을 등록한다.
    Initialize();
}

CPDUSessionInsert::~CPDUSessionInsert()
{
}

/**
 * \brief       PDU Session 테이블에 입력하는 쿼리문   
 * \detail         
 * \param[in]     type:void 
 * \return        type:bool true/false 
 *
 * */
bool CPDUSessionInsert::Initialize()
{
    bool bRtn = true;

    try
    {
        Prepare();
        SetParameter();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

bool CPDUSessionInsert::Prepare()
{
    bool bRtn = true;
    try
    {
        char *ptrQuery = (char*)
            "INSERT INTO T_SMF_SESSION_INFO ( "
            " SM_POLICY_ID      "   //  1
            ",SUPI              "   //  2
            ",GPSI              "   //  3
            ",PDU_SESSION_ID    "   //  4
            ",IP_ADDR           "   //  5
            ",MDN               "   //  6
            ",DNN               "   //  7
            ",S_NSSAI_SST       "   //  8
            ",S_NSSAI_SD        "   //  9
            ",LOC_ID            "   // 10
            ",MCC_MNC           "   // 11
            ",RAT_TYPE          "   // 12
            ",OCS_SUBS_STATUS   "   // 13
            ",IN_HTTP_HEADER    "   // 14
            ",IN_HTTP_PAYLOAD   "   // 15
            ",OUT_HTTP_PAYLOAD  "   // 16
            ",RES_URI           "   // 17
            ",NOTI_URI          "   // 18
            ",UDR_NOTI_URI      "   // 19
            ",TM_NOTI_URI       "   // 20
            ",STATUS            "   // 21
            ",AF_SUBSCRIBE      "   // 22
            ",NODE_ID           "   // 23
            ",PROC_ID           "   // 24
            ",SMF_ID            "   // 25
            ",CONN_ID           "   // 26
            ",STREAM_ID         "   // 27
            ",CREATE_TIME       "   // 28
            ",UPDATE_TIME       "   // 29
            ",DESCRIPTION       "   // 30
            ") VALUES (         "
            " ?                 "   //  1 SM_POLICY_ID
            ",?                 "   //  2 SUPI
            ",?                 "   //  3 GPSI
            ",?                 "   //  4 PDU_SESSION_ID
            ",?                 "   //  5 IP_ADDR
            ",?                 "   //  6 MDN
            ",?                 "   //  7 DNN
            ",?                 "   //  8 S_NSSAI_SST
            ",?                 "   //  9 S_NSSAI_SD
            ",?                 "   // 10 LOC_ID
            ",?                 "   // 11 MCC_MNC
            ",?                 "   // 12 RAT_TYPE
            ",?                 "   // 13 OCS_SUBS_STATUS
            ",?                 "   // 14 IN_HTTP_HEADER
            ",?                 "   // 15 IN_HTTP_PAYLOAD
            ",?                 "   // 16 OUT_HTTP_PAYLOAD
            ",?                 "   // 17 RES_URI 
            ",?                 "   // 18 NOTI_URI
            ",?                 "   // 19 UDR_NOTI_URI
            ",?                 "   // 20 TM_NOTI_URI
            ",?                 "   // 21 STATUS
            ",?                 "   // 22 AF_SUBSCRIBE
            ",?                 "   // 23 NODE_ID     
            ",?                 "   // 24 PROC_ID    
            ",?                 "   // 25 SMF_ID     
            ",?                 "   // 26 CONN_ID
            ",?                 "   // 27 STREAM_ID
            ",?                 "   // 28 CREATE_TIME
            ",?                 "   // 29 UPDATE_TIME
            ",?                 "   // 30 DESC
            ");                 ";

        bRtn = SetSQL(ptrQuery,strlen(ptrQuery));
        if(bRtn != true)
        {
            THROW(E_ERROR_LOG,"SqlSQL Error");
        }
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionInsert::SetParameter()
{
    bool bRtn = true;

    try
    {
        const int nParamCnt = 30;
        m_nParamCnt = nParamCnt;
        m_indParam = new SQLLEN[nParamCnt];
        for(int i=0;i<nParamCnt;i++)
        {
            m_indParam[i] = SQL_NTS;
        }
        m_pstBindParam = new STBindParameter[nParamCnt] {
        { 1, SQL_C_CHAR    , SQL_CHAR     , LEN_SM_POLICY_ID         , 0,  m_stSMFSessionInfo.szSMPolicyID     ,LEN_SM_POLICY_ID        +1, &m_indParam[ 0]},
        { 2, SQL_C_CHAR    , SQL_CHAR     , LEN_SUPI                 , 0,  m_stSMFSessionInfo.szSUPI           ,LEN_SUPI                +1, &m_indParam[ 1]},
        { 3, SQL_C_CHAR    , SQL_CHAR     , LEN_GPSI                 , 0,  m_stSMFSessionInfo.szGPSI           ,LEN_GPSI                +1, &m_indParam[ 2]},
        { 4, SQL_C_SLONG   , SQL_INTEGER  ,                         0, 0, &m_stSMFSessionInfo. nPDUSessionID   ,                         0, &m_indParam[ 3]},
        { 5, SQL_C_CHAR    , SQL_CHAR     , LEN_IP_ADDR              , 0,  m_stSMFSessionInfo.szIpAddr         ,LEN_IP_ADDR             +1, &m_indParam[ 4]},
        { 6, SQL_C_CHAR    , SQL_CHAR     , LEN_MDN                  , 0,  m_stSMFSessionInfo.szMDN            ,LEN_MDN                 +1, &m_indParam[ 5]},
        { 7, SQL_C_CHAR    , SQL_CHAR     , LEN_DNN                  , 0,  m_stSMFSessionInfo.szDNN            ,LEN_DNN                 +1, &m_indParam[ 6]},
        { 8, SQL_C_SLONG   , SQL_INTEGER  ,                         0, 0, &m_stSMFSessionInfo. nSNssaiSst      ,                         0, &m_indParam[ 7]},
        { 9, SQL_C_CHAR    , SQL_CHAR     , LEN_SNSSAI_SD            , 0,  m_stSMFSessionInfo.szSNssaiSd       ,LEN_SNSSAI_SD           +1, &m_indParam[ 8]},
        {10, SQL_C_CHAR    , SQL_CHAR     , LEN_LOC_ID               , 0,  m_stSMFSessionInfo.szLocId          ,LEN_LOC_ID              +1, &m_indParam[ 9]},
        {11, SQL_C_CHAR    , SQL_CHAR     , LEN_MCC_MNC              , 0,  m_stSMFSessionInfo.szMCCMNC         ,LEN_MCC_MNC             +1, &m_indParam[10]},
        {12, SQL_C_CHAR    , SQL_CHAR     , LEN_RATTYPE              , 0,  m_stSMFSessionInfo.szRatType        ,LEN_RATTYPE             +1, &m_indParam[11]},
        {13, SQL_C_CHAR    , SQL_CHAR     , LEN_FLAG                 , 0,  m_stSMFSessionInfo.szOcsSubsStatus  ,LEN_FLAG                +1, &m_indParam[12]},
        {14, SQL_C_CHAR    , SQL_CHAR     , LEN_HTTP_HEADER          , 0,  m_stSMFSessionInfo.szInHttpHeader   ,LEN_HTTP_HEADER         +1, &m_indParam[13]},
        {15, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_HTTP_IN_PAYLOAD  , 0,  m_stSMFSessionInfo.szInHttpPayload  ,LEN_PDU_HTTP_IN_PAYLOAD +1, &m_indParam[14]},
        {16, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_HTTP_OUT_PAYLOAD , 0,  m_stSMFSessionInfo.szOutHttpPayload ,LEN_PDU_HTTP_OUT_PAYLOAD+1, &m_indParam[15]},
        {17, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_RES_URI          , 0,  m_stSMFSessionInfo.szResUri         ,LEN_PDU_RES_URI         +1, &m_indParam[16]},
        {18, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_NOTIFY_URI       , 0,  m_stSMFSessionInfo.szNotifyUri      ,LEN_PDU_NOTIFY_URI      +1, &m_indParam[17]},
        {19, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_UDR_NOTI_URI     , 0,  m_stSMFSessionInfo.szUDRNotiUri     ,LEN_PDU_UDR_NOTI_URI    +1, &m_indParam[18]},
        {20, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_TM_NOTI_URI      , 0,  m_stSMFSessionInfo.szTMNotiUri      ,LEN_PDU_TM_NOTI_URI     +1, &m_indParam[19]},
        {21, SQL_C_CHAR    , SQL_CHAR     , LEN_FLAG                 , 0,  m_stSMFSessionInfo.szStatus         ,LEN_FLAG                +1, &m_indParam[20]},
        {22, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_AF_SUBSCRIBE     , 0,  m_stSMFSessionInfo.szAFSubscribe    ,LEN_PDU_AF_SUBSCRIBE    +1, &m_indParam[21]},
        {23, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_NODE_ID          , 0,  m_stSMFSessionInfo.szNodeID         ,LEN_PDU_NODE_ID         +1, &m_indParam[22]},
        {24, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_PROC_ID          , 0,  m_stSMFSessionInfo.szProcID         ,LEN_PDU_PROC_ID         +1, &m_indParam[23]},
        {25, SQL_C_CHAR    , SQL_CHAR     , LEN_SMF_ID               , 0,  m_stSMFSessionInfo.szSMFId          ,LEN_SMF_ID              +1, &m_indParam[24]},
        {26, SQL_C_SLONG   , SQL_INTEGER  ,                         0, 0, &m_stSMFSessionInfo. nConnectionID   ,                         0, &m_indParam[25]},
        {27, SQL_C_SLONG   , SQL_INTEGER  ,                         0, 0, &m_stSMFSessionInfo. nStreamID       ,                         0, &m_indParam[26]},
        {28, SQL_C_TMSTAMP , SQL_TIMESTAMP,                         0, 0, &m_tmsCreateTime                     ,                         0, &m_indParam[27]},
        {29, SQL_C_TMSTAMP , SQL_TIMESTAMP,                         0, 0, &m_tmsUpdateTime                     ,                         0, &m_indParam[28]},
        {30, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_DESC             , 0,  m_stSMFSessionInfo.szDesc           ,LEN_PDU_DESC            +1, &m_indParam[29]},
        };
        SetBindParameter();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}
/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionInsert::SetBindParameter()
{
    bool bRtn = true;

    try
    {
        PDB::CPreparedSQL::SetBindParameter(m_pstBindParam,m_nParamCnt);
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session 테이블에 입력할 데이터 
 * \detail         
 * \param[in]     type:ST_SMF_SESSION_INFO PDU Session 테이블 구조체. 
 * \return        type:bool                true/false 
 *
 * */
bool CPDUSessionInsert::SetValue(ST_SMF_SESSION_INFO *a_pstSMFSessionInfo)
{
    bool bRtn = true;
    try
    {
        memcpy(&m_stSMFSessionInfo, a_pstSMFSessionInfo,sizeof(ST_SMF_SESSION_INFO));
        TimeTToDBTime(m_stSMFSessionInfo.tCreateTime,m_tmsCreateTime  ); 
#if 0
        TimeTToDBTime(m_stSMFSessionInfo.tUpdateTime,m_tmsUpdateime  ); 
#else
        m_tmsUpdateTime = m_tmsCreateTime;
#endif
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session insert query 수행. 
 * \detail         
 * \param[in]     type:SQLHSTMT     statement  
 * \return        type:bool         true/false 
 *
 * */
bool CPDUSessionInsert::Execute(SQLHSTMT &a_sqlHStmt)
{
    bool bRtn = true;
    try
    {
        sret_ = SQLExecute(a_sqlHStmt);

        if(SQL_SUCCESS != sret_)
        {
            bRtn = false;
            GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,__func__);
            _THD_LOG(NDF_LOG_T_DEBUG, "SQLRETURN[%p][%d:%s]",a_sqlHStmt, sret_,m_szODBCError);
        }

    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,e.what());
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

CPDUSessionUpdAll::CPDUSessionUpdAll()
{
    className_ = typeid(*this).name();
    //생성할 때 쿼리문을 등록한다.
    Initialize();
}

CPDUSessionUpdAll::~CPDUSessionUpdAll()
{
}

/**
 * \brief       PDU Session 테이블에 입력하는 쿼리문   
 * \detail         
 * \param[in]     type:void 
 * \return        type:bool true/false 
 *
 * */
bool CPDUSessionUpdAll::Initialize()
{
    bool bRtn = true;

    try
    {
        Prepare();
        SetParameter();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

bool CPDUSessionUpdAll::Prepare()
{
    bool bRtn = true;
    try
    {
        char *ptrQuery = (char*)
            "UPDATE  T_SMF_SESSION_INFO   "
            " SET                         "
            " IP_ADDR                 = ? "   //  1
            ",LOC_ID                  = ? "   //  2
            ",MCC_MNC                 = ? "   //  3
            ",RAT_TYPE                = ? "   //  4
            ",OCS_SUBS_STATUS         = ? "   //  5
            ",IN_HTTP_HEADER          = ? "   //  6
            ",IN_HTTP_PAYLOAD         = ? "   //  7
            ",OUT_HTTP_PAYLOAD        = ? "   //  8
            ",RES_URI                 = ? "   //  9
            ",NOTI_URI                = ? "   // 10
            ",UDR_NOTI_URI            = ? "   // 11
            ",TM_NOTI_URI             = ? "   // 12
            ",STATUS                  = ? "   // 13
            ",AF_SUBSCRIBE            = ? "   // 14
            ",NODE_ID                 = ? "   // 15
            ",PROC_ID                 = ? "   // 16
            ",SMF_ID                  = ? "   // 17
            ",CONN_ID                 = ? "   // 18
            ",STREAM_ID               = ? "   // 19
            ",UPDATE_TIME             = ? "   // 20
            ",DESCRIPTION             = ? "   // 21
            "WHERE                        "
            " SM_POLICY_ID            = ? "   // 22
            "   ;                         ";

        bRtn = SetSQL(ptrQuery,strlen(ptrQuery));
        if(bRtn != true)
        {
            THROW(E_ERROR_LOG,"SqlSQL Error");
        }
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionUpdAll::SetParameter()
{
    bool bRtn = true;

    try
    {
        const int nParamCnt = 22;
        m_nParamCnt = nParamCnt;
        m_indParam = new SQLLEN[nParamCnt];
        for(int i=0;i<nParamCnt;i++)
        {
            m_indParam[i] = SQL_NTS;
        }
        m_pstBindParam = new STBindParameter[nParamCnt] {
        { 1, SQL_C_CHAR    , SQL_CHAR     , LEN_IP_ADDR              , 0,  m_stSMFSessionInfo.szIpAddr         , LEN_IP_ADDR             +1, &m_indParam[ 0]},
        { 2, SQL_C_CHAR    , SQL_CHAR     , LEN_LOC_ID               , 0,  m_stSMFSessionInfo.szLocId          , LEN_LOC_ID              +1, &m_indParam[ 1]},
        { 3, SQL_C_CHAR    , SQL_CHAR     , LEN_MCC_MNC              , 0,  m_stSMFSessionInfo.szMCCMNC         , LEN_MCC_MNC             +1, &m_indParam[ 2]},
        { 4, SQL_C_CHAR    , SQL_CHAR     , LEN_RATTYPE              , 0,  m_stSMFSessionInfo.szRatType        , LEN_RATTYPE             +1, &m_indParam[ 3]},
        { 5, SQL_C_CHAR    , SQL_CHAR     , LEN_FLAG                 , 0,  m_stSMFSessionInfo.szOcsSubsStatus  , LEN_FLAG                +1, &m_indParam[ 4]},
        { 6, SQL_C_CHAR    , SQL_CHAR     , LEN_HTTP_HEADER          , 0,  m_stSMFSessionInfo.szInHttpHeader   , LEN_HTTP_HEADER         +1, &m_indParam[ 5]},
        { 7, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_HTTP_IN_PAYLOAD  , 0,  m_stSMFSessionInfo.szInHttpPayload  , LEN_PDU_HTTP_IN_PAYLOAD +1, &m_indParam[ 6]},
        { 8, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_HTTP_OUT_PAYLOAD , 0,  m_stSMFSessionInfo.szOutHttpPayload , LEN_PDU_HTTP_OUT_PAYLOAD+1, &m_indParam[ 7]},
        { 9, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_RES_URI          , 0,  m_stSMFSessionInfo.szResUri         , LEN_PDU_RES_URI         +1, &m_indParam[ 8]},
        {10, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_NOTIFY_URI       , 0,  m_stSMFSessionInfo.szNotifyUri      , LEN_PDU_NOTIFY_URI      +1, &m_indParam[ 9]},
        {11, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_UDR_NOTI_URI     , 0,  m_stSMFSessionInfo.szUDRNotiUri     , LEN_PDU_UDR_NOTI_URI    +1, &m_indParam[10]},
        {12, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_TM_NOTI_URI      , 0,  m_stSMFSessionInfo.szTMNotiUri      , LEN_PDU_TM_NOTI_URI     +1, &m_indParam[11]},
        {13, SQL_C_CHAR    , SQL_CHAR     , LEN_FLAG                 , 0,  m_stSMFSessionInfo.szStatus         , LEN_FLAG                +1, &m_indParam[12]},
        {14, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_AF_SUBSCRIBE     , 0,  m_stSMFSessionInfo.szAFSubscribe    , LEN_PDU_AF_SUBSCRIBE    +1, &m_indParam[13]},
        {15, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_NODE_ID          , 0,  m_stSMFSessionInfo.szNodeID         , LEN_PDU_NODE_ID         +1, &m_indParam[14]},
        {16, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_PROC_ID          , 0,  m_stSMFSessionInfo.szProcID         , LEN_PDU_PROC_ID         +1, &m_indParam[15]},
        {17, SQL_C_CHAR    , SQL_CHAR     , LEN_SMF_ID               , 0,  m_stSMFSessionInfo.szSMFId          , LEN_SMF_ID              +1, &m_indParam[16]},
        {18, SQL_C_SLONG   , SQL_INTEGER  ,                         0, 0, &m_stSMFSessionInfo. nConnectionID   ,                          0, &m_indParam[17]},
        {19, SQL_C_SLONG   , SQL_INTEGER  ,                         0, 0, &m_stSMFSessionInfo. nStreamID       ,                          0, &m_indParam[18]},
        {20, SQL_C_TMSTAMP , SQL_TIMESTAMP,                         0, 0, &m_tmsUpdateTime                     ,                          0, &m_indParam[19]},
        {21, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_DESC             , 0,  m_stSMFSessionInfo.szDesc           , LEN_PDU_DESC            +1, &m_indParam[20]},
        {22, SQL_C_CHAR    , SQL_CHAR     , LEN_SM_POLICY_ID         , 0,  m_stSMFSessionInfo.szSMPolicyID     , LEN_SM_POLICY_ID        +1, &m_indParam[21]},
        };
        SetBindParameter();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionUpdAll::SetBindParameter()
{
    bool bRtn = true;

    try
    {
        PDB::CPreparedSQL::SetBindParameter(m_pstBindParam,m_nParamCnt);
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session 테이블에 입력할 데이터 
 * \detail         
 * \param[in]     type:ST_SMF_SESSION_INFO PDU Session 테이블 구조체. 
 * \return        type:bool                true/false 
 *
 * */
bool CPDUSessionUpdAll::SetValue(ST_SMF_SESSION_INFO *a_pstSMFSessionInfo)
{
    bool bRtn = true;
    try
    {
        memcpy(&m_stSMFSessionInfo, a_pstSMFSessionInfo,sizeof(ST_SMF_SESSION_INFO));
        TimeTToDBTime(m_stSMFSessionInfo.tUpdateTime,m_tmsUpdateTime  ); 
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session insert query 수행. 
 * \detail         
 * \param[in]     type:SQLHSTMT     statement  
 * \return        type:bool         true/false 
 *
 * */
bool CPDUSessionUpdAll::Execute(SQLHSTMT &a_sqlHStmt)
{
    bool bRtn = true;
    try
    {

        sret_ = SQLExecute(a_sqlHStmt);

        if(SQL_SUCCESS != sret_)
        {
            if(SQL_NO_DATA != sret_)
            {
                bRtn = false;
                GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,__func__);
                _THD_LOG(NDF_LOG_T_WARNING, "SQLRETURN[%p][%d:%s]",a_sqlHStmt, sret_,m_szODBCError);
            }
        }

    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,e.what());
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

CPDUSessionUpdate::CPDUSessionUpdate()
{
    className_ = typeid(*this).name();
    //생성할 때 쿼리문을 등록한다.
    Initialize();
}

CPDUSessionUpdate::~CPDUSessionUpdate()
{
}

/**
 * \brief       PDU Session 테이블에 입력하는 쿼리문   
 * \detail         
 * \param[in]     type:void 
 * \return        type:bool true/false 
 *
 * */
bool CPDUSessionUpdate::Initialize()
{
    bool bRtn = true;

    try
    {
        Prepare();
        SetParameter();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

bool CPDUSessionUpdate::Prepare()
{
    bool bRtn = true;
    try
    {
        char *ptrQuery = (char*)
            "UPDATE  T_SMF_SESSION_INFO   "
            " SET                         "
            " IP_ADDR                 = ? "   //  1
            ",LOC_ID                  = ? "   //  2
            ",MCC_MNC                 = ? "   //  3
            ",RAT_TYPE                = ? "   //  4
            ",OCS_SUBS_STATUS         = ? "   //  5
            ",IN_HTTP_HEADER          = ? "   //  6
            ",IN_HTTP_PAYLOAD         = ? "   //  7
            ",OUT_HTTP_PAYLOAD        = ? "   //  8
            ",RES_URI                 = ? "   //  9
            ",UDR_NOTI_URI            = ? "   // 10
            ",TM_NOTI_URI             = ? "   // 11
            ",STATUS                  = ? "   // 12
            ",AF_SUBSCRIBE            = ? "   // 13
            ",NODE_ID                 = ? "   // 14
            ",PROC_ID                 = ? "   // 15
            ",SMF_ID                  = ? "   // 16
            ",CONN_ID                 = ? "   // 17
            ",STREAM_ID               = ? "   // 18
            ",UPDATE_TIME             = ? "   // 19
            ",DESCRIPTION             = ? "   // 20
            "WHERE                        "
            " SM_POLICY_ID            = ? "   // 21
            "   ;                         ";

        bRtn = SetSQL(ptrQuery,strlen(ptrQuery));
        if(bRtn != true)
        {
            THROW(E_ERROR_LOG,"SqlSQL Error");
        }
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionUpdate::SetParameter()
{
    bool bRtn = true;

    try
    {
        const int nParamCnt = 21;
        m_nParamCnt = nParamCnt;
        m_indParam = new SQLLEN[nParamCnt];
        for(int i=0;i<nParamCnt;i++)
        {
            m_indParam[i] = SQL_NTS;
        }
        m_pstBindParam = new STBindParameter[nParamCnt] {
        { 1, SQL_C_CHAR    , SQL_CHAR     , LEN_IP_ADDR              , 0,  m_stSMFSessionInfo.szIpAddr         , LEN_IP_ADDR             +1, &m_indParam[ 0]},
        { 2, SQL_C_CHAR    , SQL_CHAR     , LEN_LOC_ID               , 0,  m_stSMFSessionInfo.szLocId          , LEN_LOC_ID              +1, &m_indParam[ 1]},
        { 3, SQL_C_CHAR    , SQL_CHAR     , LEN_MCC_MNC              , 0,  m_stSMFSessionInfo.szMCCMNC         , LEN_MCC_MNC             +1, &m_indParam[ 2]},
        { 4, SQL_C_CHAR    , SQL_CHAR     , LEN_RATTYPE              , 0,  m_stSMFSessionInfo.szRatType        , LEN_RATTYPE             +1, &m_indParam[ 3]},
        { 5, SQL_C_CHAR    , SQL_CHAR     , LEN_FLAG                 , 0,  m_stSMFSessionInfo.szOcsSubsStatus  , LEN_FLAG                +1, &m_indParam[ 4]},
        { 6, SQL_C_CHAR    , SQL_CHAR     , LEN_HTTP_HEADER          , 0,  m_stSMFSessionInfo.szInHttpHeader   , LEN_HTTP_HEADER         +1, &m_indParam[ 5]},
        { 7, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_HTTP_IN_PAYLOAD  , 0,  m_stSMFSessionInfo.szInHttpPayload  , LEN_PDU_HTTP_IN_PAYLOAD +1, &m_indParam[ 6]},
        { 8, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_HTTP_OUT_PAYLOAD , 0,  m_stSMFSessionInfo.szOutHttpPayload , LEN_PDU_HTTP_OUT_PAYLOAD+1, &m_indParam[ 7]},
        { 9, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_RES_URI          , 0,  m_stSMFSessionInfo.szResUri         , LEN_PDU_RES_URI         +1, &m_indParam[ 8]},
        {10, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_UDR_NOTI_URI     , 0,  m_stSMFSessionInfo.szUDRNotiUri     , LEN_PDU_UDR_NOTI_URI    +1, &m_indParam[ 9]},
        {11, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_TM_NOTI_URI      , 0,  m_stSMFSessionInfo.szTMNotiUri      , LEN_PDU_TM_NOTI_URI     +1, &m_indParam[10]},
        {12, SQL_C_CHAR    , SQL_CHAR     , LEN_FLAG                 , 0,  m_stSMFSessionInfo.szStatus         , LEN_FLAG                +1, &m_indParam[11]},
        {13, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_AF_SUBSCRIBE     , 0,  m_stSMFSessionInfo.szAFSubscribe    , LEN_PDU_AF_SUBSCRIBE    +1, &m_indParam[12]},
        {14, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_NODE_ID          , 0,  m_stSMFSessionInfo.szNodeID         , LEN_PDU_NODE_ID         +1, &m_indParam[13]},
        {15, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_PROC_ID          , 0,  m_stSMFSessionInfo.szProcID         , LEN_PDU_PROC_ID         +1, &m_indParam[14]},
        {16, SQL_C_CHAR    , SQL_CHAR     , LEN_SMF_ID               , 0,  m_stSMFSessionInfo.szSMFId          , LEN_SMF_ID              +1, &m_indParam[15]},
        {17, SQL_C_SLONG   , SQL_INTEGER  ,                         0, 0, &m_stSMFSessionInfo. nConnectionID   ,                          0, &m_indParam[16]},
        {18, SQL_C_SLONG   , SQL_INTEGER  ,                         0, 0, &m_stSMFSessionInfo. nStreamID       ,                          0, &m_indParam[17]},
        {19, SQL_C_TMSTAMP , SQL_TIMESTAMP,                         0, 0, &m_tmsUpdateTime                     ,                          0, &m_indParam[18]},
        {20, SQL_C_CHAR    , SQL_CHAR     , LEN_PDU_DESC             , 0,  m_stSMFSessionInfo.szDesc           , LEN_PDU_DESC            +1, &m_indParam[19]},
        {21, SQL_C_CHAR    , SQL_CHAR     , LEN_SM_POLICY_ID         , 0,  m_stSMFSessionInfo.szSMPolicyID     , LEN_SM_POLICY_ID        +1, &m_indParam[20]},
        };
        SetBindParameter();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionUpdate::SetBindParameter()
{
    bool bRtn = true;

    try
    {
        PDB::CPreparedSQL::SetBindParameter(m_pstBindParam,m_nParamCnt);
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session 테이블에 입력할 데이터 
 * \detail         
 * \param[in]     type:ST_SMF_SESSION_INFO PDU Session 테이블 구조체. 
 * \return        type:bool                true/false 
 *
 * */
bool CPDUSessionUpdate::SetValue(ST_SMF_SESSION_INFO *a_pstSMFSessionInfo)
{
    bool bRtn = true;
    try
    {
        memcpy(&m_stSMFSessionInfo, a_pstSMFSessionInfo,sizeof(ST_SMF_SESSION_INFO));
        TimeTToDBTime(m_stSMFSessionInfo.tUpdateTime,m_tmsUpdateTime  ); 
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session insert query 수행. 
 * \detail         
 * \param[in]     type:SQLHSTMT     statement  
 * \return        type:bool         true/false 
 *
 * */
bool CPDUSessionUpdate::Execute(SQLHSTMT &a_sqlHStmt)
{
    bool bRtn = true;
    try
    {

        sret_ = SQLExecute(a_sqlHStmt);

        if(SQL_SUCCESS != sret_)
        {
            if(SQL_NO_DATA != sret_)
            {
                bRtn = false;
                GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,__func__);
                _THD_LOG(NDF_LOG_T_WARNING, "SQLRETURN[%p][%d:%s]",a_sqlHStmt, sret_,m_szODBCError);
            }
        }

    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,e.what());
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

CPDUSessionDelete::CPDUSessionDelete()
{
    className_ = typeid(*this).name();
    //생성할 때 쿼리문을 등록한다.
    Initialize();
}

CPDUSessionDelete::~CPDUSessionDelete()
{
}

/**
 * \brief       PDU Session 테이블에 입력하는 쿼리문   
 * \detail         
 * \param[in]     type:void 
 * \return        type:bool true/false 
 *
 * */
bool CPDUSessionDelete::Initialize()
{
    bool bRtn = true;

    try
    {
        Prepare();
        SetParameter();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

bool CPDUSessionDelete::Prepare()
{
    bool bRtn = true;
    try
    {
        char *ptrQuery = (char*)
            "DELETE FROM T_SMF_SESSION_INFO      "
            "   WHERE                            "
            "   SM_POLICY_ID                 = ? " // 1
            "   ;                                ";

        bRtn = SetSQL(ptrQuery,strlen(ptrQuery));
        if(bRtn != true)
        {
            THROW(E_ERROR_LOG,"SqlSQL Error");
        }
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionDelete::SetParameter()
{
    bool bRtn = true;

    try
    {
        const int nParamCnt = 1;
        m_nParamCnt = nParamCnt;
        m_indParam = new SQLLEN[nParamCnt];
        for(int i=0;i<nParamCnt;i++)
        {
            m_indParam[i] = SQL_NTS;
        }
        m_pstBindParam = new STBindParameter[nParamCnt] {
            { 1, SQL_C_CHAR  , SQL_CHAR    , LEN_SM_POLICY_ID  , 0,  m_szSMPolicyID     , LEN_SM_POLICY_ID  +1, &m_indParam[ 0]},
        };
        SetBindParameter();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionDelete::SetBindParameter()
{
    bool bRtn = true;

    try
    {
        PDB::CPreparedSQL::SetBindParameter(m_pstBindParam,m_nParamCnt);
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session 테이블에 입력할 데이터 
 * \detail         
 * \param[in]     type:ST_SMF_SESSION_INFO PDU Session 테이블 구조체. 
 * \return        type:bool                true/false 
 *
 * */
bool CPDUSessionDelete::SetValue(char *a_pszSMPolicyID)
{
    bool bRtn = true;
    try
    {
        SNPRINTF(m_szSMPolicyID,"%s",a_pszSMPolicyID);
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session insert query 수행. 
 * \detail         
 * \param[in]     type:SQLHSTMT     statement  
 * \return        type:bool         true/false 
 *
 * */
bool CPDUSessionDelete::Execute(SQLHSTMT &a_sqlHStmt)
{
    bool bRtn = true;
    try
    {
        sret_ = SQLExecute(a_sqlHStmt);

        if(SQL_SUCCESS != sret_)
        {
            if(SQL_NO_DATA != sret_)
            {
                bRtn = false;
                GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,__func__);
                _THD_LOG(NDF_LOG_T_WARNING, "SQLRETURN[%p][%d:%s]",a_sqlHStmt, sret_,m_szODBCError);
            }
        }

    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
                if(SQL_NO_DATA == sret_)
                {
                    _THD_LOG(NDF_LOG_T_INFO,"[%s]",e.what());
                }
                else
                {
                    GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,e.what());
                }
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

CPDUSessionSelect::CPDUSessionSelect()
{
    className_ = typeid(*this).name();
    //생성할 때 쿼리문을 등록한다.
    Initialize();
}

CPDUSessionSelect::~CPDUSessionSelect()
{
}

/**
 * \brief       PDU Session 테이블에 입력하는 쿼리문   
 * \detail         
 * \param[in]     type:void 
 * \return        type:bool true/false 
 *
 * */
bool CPDUSessionSelect::Initialize()
{
    bool bRtn = true;

    try
    {
        Prepare();
        SetParameter();
        SetColumn();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

bool CPDUSessionSelect::Prepare()
{
    bool bRtn = true;
    try
    {
        char *ptrQuery = (char*)
            "SELECT                   "
            " SM_POLICY_ID            "   //  1
            ",SUPI                    "   //  2
            ",GPSI                    "   //  3
            ",PDU_SESSION_ID          "   //  4
            ",IP_ADDR                 "   //  5
            ",MDN                     "   //  6
            ",DNN                     "   //  7
            ",S_NSSAI_SST             "   //  8
            ",S_NSSAI_SD              "   //  9
            ",LOC_ID                  "   // 10
            ",MCC_MNC                 "   // 11
            ",RAT_TYPE                "   // 12
            ",OCS_SUBS_STATUS         "   // 13
            ",IN_HTTP_HEADER          "   // 14
            ",IN_HTTP_PAYLOAD         "   // 15
            ",OUT_HTTP_PAYLOAD        "   // 16
            ",RES_URI                 "   // 17
            ",NOTI_URI                "   // 18
            ",UDR_NOTI_URI            "   // 19
            ",TM_NOTI_URI             "   // 20
            ",STATUS                  "   // 21
            ",AF_SUBSCRIBE            "   // 22
            ",NODE_ID                 "   // 23
            ",PROC_ID                 "   // 24
            ",SMF_ID                  "   // 25
            ",CONN_ID                 "   // 26
            ",STREAM_ID               "   // 27
            ",DESCRIPTION             "   // 28
            ",CREATE_TIME             "   // 29
            ",UPDATE_TIME             "   // 30
            " FROM T_SMF_SESSION_INFO "
            " WHERE SM_POLICY_ID = ?  "
            " ;                       ";

        bRtn = SetSQL(ptrQuery,strlen(ptrQuery));
        if(bRtn != true)
        {
            THROW(E_ERROR_LOG,"SqlSQL Error");
        }
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionSelect::SetParameter()
{
    bool bRtn = true;

    try
    {
        const int nParamCnt = 1;
        m_nParamCnt = nParamCnt;
        m_indParam = new SQLLEN[nParamCnt];
        for(int i=0;i<nParamCnt;i++)
        {
            m_indParam[i] = SQL_NTS;
        }
        m_pstBindParam = new STBindParameter[nParamCnt] {
        { 1, SQL_C_CHAR  , SQL_CHAR    , LEN_SM_POLICY_ID  , 0,  m_szSMPolicyID     , LEN_SM_POLICY_ID  +1, &m_indParam[ 0]},
        };
        SetBindParameter();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

bool CPDUSessionSelect::SetColumn()
{
    bool bRtn = true;

    try
    {
        const int nColumnCnt = 30;
        m_nColumnCnt = nColumnCnt;
        m_indColumn = new SQLLEN[nColumnCnt];
        for(int i=0;i<nColumnCnt;i++)
        {
            m_indColumn[i] = SQL_NTS;
        }
        m_pstBindColumn = new STBindColumn[nColumnCnt] {
            { 1, SQL_C_CHAR     ,  m_stSMFSessionInfo.szSMPolicyID     , LEN_SM_POLICY_ID        +1,&m_indColumn[ 0]},
            { 2, SQL_C_CHAR     ,  m_stSMFSessionInfo.szSUPI           , LEN_SUPI                +1,&m_indColumn[ 1]},
            { 3, SQL_C_CHAR     ,  m_stSMFSessionInfo.szGPSI           , LEN_GPSI                +1,&m_indColumn[ 2]},
            { 4, SQL_C_SLONG    , &m_stSMFSessionInfo. nPDUSessionID   ,                          0,&m_indColumn[ 3]},
            { 5, SQL_C_CHAR     ,  m_stSMFSessionInfo.szIpAddr         , LEN_IP_ADDR             +1,&m_indColumn[ 4]},
            { 6, SQL_C_CHAR     ,  m_stSMFSessionInfo.szMDN            , LEN_MDN                 +1,&m_indColumn[ 5]},
            { 7, SQL_C_CHAR     ,  m_stSMFSessionInfo.szDNN            , LEN_DNN                 +1,&m_indColumn[ 6]},
            { 8, SQL_C_SLONG    , &m_stSMFSessionInfo. nSNssaiSst      ,                          0,&m_indColumn[ 7]},
            { 9, SQL_C_CHAR     ,  m_stSMFSessionInfo.szSNssaiSd       , LEN_SNSSAI_SD           +1,&m_indColumn[ 8]},
            {10, SQL_C_CHAR     ,  m_stSMFSessionInfo.szLocId          , LEN_LOC_ID              +1,&m_indColumn[ 9]},
            {11, SQL_C_CHAR     ,  m_stSMFSessionInfo.szMCCMNC         , LEN_MCC_MNC             +1,&m_indColumn[10]},
            {12, SQL_C_CHAR     ,  m_stSMFSessionInfo.szRatType        , LEN_RATTYPE             +1,&m_indColumn[11]},
            {13, SQL_C_CHAR     ,  m_stSMFSessionInfo.szOcsSubsStatus  , LEN_FLAG                +1,&m_indColumn[12]},
            {14, SQL_C_CHAR     ,  m_stSMFSessionInfo.szInHttpHeader   , LEN_HTTP_HEADER         +1,&m_indColumn[13]},
            {15, SQL_C_CHAR     ,  m_stSMFSessionInfo.szInHttpPayload  , LEN_PDU_HTTP_IN_PAYLOAD +1,&m_indColumn[14]},
            {16, SQL_C_CHAR     ,  m_stSMFSessionInfo.szOutHttpPayload , LEN_PDU_HTTP_OUT_PAYLOAD+1,&m_indColumn[15]},
            {17, SQL_C_CHAR     ,  m_stSMFSessionInfo.szResUri         , LEN_PDU_RES_URI         +1,&m_indColumn[16]},
            {18, SQL_C_CHAR     ,  m_stSMFSessionInfo.szNotifyUri      , LEN_PDU_NOTIFY_URI      +1,&m_indColumn[17]},
            {19, SQL_C_CHAR     ,  m_stSMFSessionInfo.szUDRNotiUri     , LEN_PDU_UDR_NOTI_URI    +1,&m_indColumn[18]},
            {20, SQL_C_CHAR     ,  m_stSMFSessionInfo.szTMNotiUri      , LEN_PDU_TM_NOTI_URI     +1,&m_indColumn[19]},
            {21, SQL_C_CHAR     ,  m_stSMFSessionInfo.szStatus         , LEN_FLAG                +1,&m_indColumn[20]},
            {22, SQL_C_CHAR     ,  m_stSMFSessionInfo.szAFSubscribe    , LEN_PDU_AF_SUBSCRIBE    +1,&m_indColumn[21]},
            {23, SQL_C_CHAR     ,  m_stSMFSessionInfo.szNodeID         , LEN_PDU_NODE_ID         +1,&m_indColumn[22]},
            {24, SQL_C_CHAR     ,  m_stSMFSessionInfo.szProcID         , LEN_PDU_PROC_ID         +1,&m_indColumn[23]},
            {25, SQL_C_CHAR     ,  m_stSMFSessionInfo.szSMFId          , LEN_SMF_ID              +1,&m_indColumn[24]},
            {26, SQL_C_SLONG    , &m_stSMFSessionInfo. nConnectionID   ,                          0,&m_indColumn[25]},
            {27, SQL_C_SLONG    , &m_stSMFSessionInfo. nStreamID       ,                          0,&m_indColumn[26]},
            {28, SQL_C_CHAR     ,  m_stSMFSessionInfo.szDesc           , LEN_PDU_DESC            +1,&m_indColumn[27]},
            {29, SQL_C_TMSTAMP  , &m_tmsCreateTime                     ,                          0,&m_indColumn[28]},
            {30, SQL_C_TMSTAMP  , &m_tmsUpdateTime                     ,                          0,&m_indColumn[29]},
        };
        SetBindColumn();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}
/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionSelect::SetBindParameter()
{
    bool bRtn = true;

    try
    {
        PDB::CPreparedSQL::SetBindParameter(m_pstBindParam,m_nParamCnt);
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

bool CPDUSessionSelect::SetBindColumn()
{
    bool bRtn = true;

    try
    {
        PDB::CPreparedSQL::SetBindColumn(m_pstBindColumn,m_nColumnCnt);
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session 테이블에 입력할 데이터 
 * \detail         
 * \param[in]     type:ST_SMF_SESSION_INFO PDU Session 테이블 구조체. 
 * \return        type:bool                true/false 
 *
 * */
bool CPDUSessionSelect::SetValue(char *a_pszSMPolicyID)
{
    bool bRtn = true;
    try
    {
        SNPRINTF(m_szSMPolicyID,"%s",a_pszSMPolicyID);
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session insert query 수행. 
 * \detail         
 * \param[in]     type:SQLHSTMT     statement  
 * \return        type:bool         true/false 
 *
 * */
bool CPDUSessionSelect::Execute(SQLHSTMT &a_sqlHStmt)
{
    bool bRtn = true;
    try
    {
        sret_ = SQLExecute(a_sqlHStmt);

        if(SQL_SUCCESS == sret_)
        {
            sret_ = SQLFetch(a_sqlHStmt);
            if(SQL_SUCCESS != sret_)
            {
                if(SQL_NO_DATA != sret_)
                {
                    bRtn = false;
                    GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,__func__);
                    _THD_LOG(NDF_LOG_T_WARNING, "SQLRETURN[%p][%d:%s]",a_sqlHStmt, sret_,m_szODBCError);
                }
            }
            else
            {
                DBTimeToTimeT(m_tmsCreateTime,m_stSMFSessionInfo.tCreateTime);
                DBTimeToTimeT(m_tmsUpdateTime,m_stSMFSessionInfo.tUpdateTime);
            }

        }
        else
        {
            if(SQL_NO_DATA != sret_)
            {
                bRtn = false;
                GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,__func__);
                printf("[%p][%d][%s]\n",a_sqlHStmt,sret_,m_szODBCError);
                _THD_LOG(NDF_LOG_T_WARNING, "SQLRETURN[%p][%d:%s]",a_sqlHStmt, sret_,m_szODBCError);
            }
        }

    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
                if(SQL_NO_DATA == sret_)
                {
                    _THD_LOG(NDF_LOG_T_INFO,"[%s]",e.what());
                }
                else
                {
                    GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,e.what());
                }
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

CPDUSessionSelectMDN::CPDUSessionSelectMDN()
{
    className_ = typeid(*this).name();
    //생성할 때 쿼리문을 등록한다.
    Initialize();
}

CPDUSessionSelectMDN::~CPDUSessionSelectMDN()
{
}

/**
 * \brief       PDU Session 테이블에 입력하는 쿼리문   
 * \detail         
 * \param[in]     type:void 
 * \return        type:bool true/false 
 *
 * */
bool CPDUSessionSelectMDN::Initialize()
{
    bool bRtn = true;

    try
    {
        Prepare();
        SetParameter();
        SetColumn();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

bool CPDUSessionSelectMDN::Prepare()
{
    bool bRtn = true;
    try
    {
        char *ptrQuery = (char*)
            "SELECT                   "
            " SM_POLICY_ID            "   //  1
            ",SUPI                    "   //  2
            ",GPSI                    "   //  3
            ",PDU_SESSION_ID          "   //  4
            ",IP_ADDR                 "   //  5
            ",MDN                     "   //  6
            ",DNN                     "   //  7
            ",S_NSSAI_SST             "   //  8
            ",S_NSSAI_SD              "   //  9
            ",LOC_ID                  "   // 10
            ",MCC_MNC                 "   // 11
            ",RAT_TYPE                "   // 12
            ",OCS_SUBS_STATUS         "   // 13
            ",IN_HTTP_HEADER          "   // 14
            ",IN_HTTP_PAYLOAD         "   // 15
            ",OUT_HTTP_PAYLOAD        "   // 16
            ",RES_URI                 "   // 17
            ",NOTI_URI                "   // 18
            ",UDR_NOTI_URI            "   // 19
            ",TM_NOTI_URI             "   // 20
            ",STATUS                  "   // 21
            ",AF_SUBSCRIBE            "   // 22
            ",NODE_ID                 "   // 23
            ",PROC_ID                 "   // 24
            ",SMF_ID                  "   // 25
            ",CONN_ID                 "   // 26
            ",STREAM_ID               "   // 27
            ",DESCRIPTION             "   // 28
            ",CREATE_TIME             "   // 29
            ",UPDATE_TIME             "   // 30
            " FROM T_SMF_SESSION_INFO "
            " WHERE                   "
            "       MDN =  ?          "
            " AND   DNN =  ?          "
            " ORDER BY CREATE_TIME    "
            "              DESC       "
            " LIMIT 1                 "
            " ;                       ";

        bRtn = SetSQL(ptrQuery,strlen(ptrQuery));
        if(bRtn != true)
        {
            THROW(E_ERROR_LOG,"SqlSQL Error");
        }
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionSelectMDN::SetParameter()
{
    bool bRtn = true;

    try
    {
        const int nParamCnt = 2;
        m_nParamCnt = nParamCnt;
        m_indParam = new SQLLEN[nParamCnt];
        for(int i=0;i<nParamCnt;i++)
        {
            m_indParam[i] = SQL_NTS;
        }
        m_pstBindParam = new STBindParameter[nParamCnt] {
        { 1, SQL_C_CHAR  , SQL_CHAR    , LEN_MDN  , 0,  m_szMDN     , LEN_MDN  +1, &m_indParam[ 0]},
        { 2, SQL_C_CHAR  , SQL_CHAR    , LEN_DNN  , 0,  m_szDNN     , LEN_DNN  +1, &m_indParam[ 1]},
        };
        SetBindParameter();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

bool CPDUSessionSelectMDN::SetColumn()
{
    bool bRtn = true;

    try
    {
        const int nColumnCnt = 30;
        m_nColumnCnt = nColumnCnt;
        m_indColumn = new SQLLEN[nColumnCnt];
        for(int i=0;i<nColumnCnt;i++)
        {
            m_indColumn[i] = SQL_NTS;
        }
        m_pstBindColumn = new STBindColumn[nColumnCnt] {
            { 1, SQL_C_CHAR     ,  m_stSMFSessionInfo.szSMPolicyID     , LEN_SM_POLICY_ID        +1,&m_indColumn[ 0]},
            { 2, SQL_C_CHAR     ,  m_stSMFSessionInfo.szSUPI           , LEN_SUPI                +1,&m_indColumn[ 1]},
            { 3, SQL_C_CHAR     ,  m_stSMFSessionInfo.szGPSI           , LEN_GPSI                +1,&m_indColumn[ 2]},
            { 4, SQL_C_SLONG    , &m_stSMFSessionInfo. nPDUSessionID   ,                          0,&m_indColumn[ 3]},
            { 5, SQL_C_CHAR     ,  m_stSMFSessionInfo.szIpAddr         , LEN_IP_ADDR             +1,&m_indColumn[ 4]},
            { 6, SQL_C_CHAR     ,  m_stSMFSessionInfo.szMDN            , LEN_MDN                 +1,&m_indColumn[ 5]},
            { 7, SQL_C_CHAR     ,  m_stSMFSessionInfo.szDNN            , LEN_DNN                 +1,&m_indColumn[ 6]},
            { 8, SQL_C_SLONG    , &m_stSMFSessionInfo. nSNssaiSst      ,                          0,&m_indColumn[ 7]},
            { 9, SQL_C_CHAR     ,  m_stSMFSessionInfo.szSNssaiSd       , LEN_SNSSAI_SD           +1,&m_indColumn[ 8]},
            {10, SQL_C_CHAR     ,  m_stSMFSessionInfo.szLocId          , LEN_LOC_ID              +1,&m_indColumn[ 9]},
            {11, SQL_C_CHAR     ,  m_stSMFSessionInfo.szMCCMNC         , LEN_MCC_MNC             +1,&m_indColumn[10]},
            {12, SQL_C_CHAR     ,  m_stSMFSessionInfo.szRatType        , LEN_RATTYPE             +1,&m_indColumn[11]},
            {13, SQL_C_CHAR     ,  m_stSMFSessionInfo.szOcsSubsStatus  , LEN_FLAG                +1,&m_indColumn[12]},
            {14, SQL_C_CHAR     ,  m_stSMFSessionInfo.szInHttpHeader   , LEN_HTTP_HEADER         +1,&m_indColumn[13]},
            {15, SQL_C_CHAR     ,  m_stSMFSessionInfo.szInHttpPayload  , LEN_PDU_HTTP_IN_PAYLOAD +1,&m_indColumn[14]},
            {16, SQL_C_CHAR     ,  m_stSMFSessionInfo.szOutHttpPayload , LEN_PDU_HTTP_OUT_PAYLOAD+1,&m_indColumn[15]},
            {17, SQL_C_CHAR     ,  m_stSMFSessionInfo.szResUri         , LEN_PDU_RES_URI         +1,&m_indColumn[16]},
            {18, SQL_C_CHAR     ,  m_stSMFSessionInfo.szNotifyUri      , LEN_PDU_NOTIFY_URI      +1,&m_indColumn[17]},
            {19, SQL_C_CHAR     ,  m_stSMFSessionInfo.szUDRNotiUri     , LEN_PDU_UDR_NOTI_URI    +1,&m_indColumn[18]},
            {20, SQL_C_CHAR     ,  m_stSMFSessionInfo.szTMNotiUri      , LEN_PDU_TM_NOTI_URI     +1,&m_indColumn[19]},
            {21, SQL_C_CHAR     ,  m_stSMFSessionInfo.szStatus         , LEN_FLAG                +1,&m_indColumn[20]},
            {22, SQL_C_CHAR     ,  m_stSMFSessionInfo.szAFSubscribe    , LEN_PDU_AF_SUBSCRIBE    +1,&m_indColumn[21]},
            {23, SQL_C_CHAR     ,  m_stSMFSessionInfo.szNodeID         , LEN_PDU_NODE_ID         +1,&m_indColumn[22]},
            {24, SQL_C_CHAR     ,  m_stSMFSessionInfo.szProcID         , LEN_PDU_PROC_ID         +1,&m_indColumn[23]},
            {25, SQL_C_CHAR     ,  m_stSMFSessionInfo.szSMFId          , LEN_SMF_ID              +1,&m_indColumn[24]},
            {26, SQL_C_SLONG    , &m_stSMFSessionInfo. nConnectionID   ,                          0,&m_indColumn[25]},
            {27, SQL_C_SLONG    , &m_stSMFSessionInfo. nStreamID       ,                          0,&m_indColumn[26]},
            {28, SQL_C_CHAR     ,  m_stSMFSessionInfo.szDesc           , LEN_PDU_DESC            +1,&m_indColumn[27]},
            {29, SQL_C_TMSTAMP  , &m_tmsCreateTime                     ,                          0,&m_indColumn[28]},
            {30, SQL_C_TMSTAMP  , &m_tmsUpdateTime                     ,                          0,&m_indColumn[29]},
        };
        SetBindColumn();
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}
/**
 * \brief         pdu session insert query bind parameter
 * \detail         
 * \param[in]     type:void
 * \return        type:bool true/false
 *
 * */
bool CPDUSessionSelectMDN::SetBindParameter()
{
    bool bRtn = true;

    try
    {
        PDB::CPreparedSQL::SetBindParameter(m_pstBindParam,m_nParamCnt);
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

bool CPDUSessionSelectMDN::SetBindColumn()
{
    bool bRtn = true;

    try
    {
        PDB::CPreparedSQL::SetBindColumn(m_pstBindColumn,m_nColumnCnt);
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session 테이블에 입력할 데이터 
 * \detail         
 * \param[in]     type:ST_SMF_SESSION_INFO PDU Session 테이블 구조체. 
 * \return        type:bool                true/false 
 *
 * */
bool CPDUSessionSelectMDN::SetValue(char *a_pszMDN,char *a_pszDNN)
{
    bool bRtn = true;
    try
    {
        SNPRINTF(m_szMDN,"%s",a_pszMDN);
        SNPRINTF(m_szDNN,"%s",a_pszDNN);
    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
            _THD_LOG(NDF_LOG_T_WARNING, "[%s]", e.what());
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }
    return bRtn;
}

/**
 * \brief         PDU Session insert query 수행. 
 * \detail         
 * \param[in]     type:SQLHSTMT     statement  
 * \return        type:bool         true/false 
 *
 * */
bool CPDUSessionSelectMDN::Execute(SQLHSTMT &a_sqlHStmt)
{
    bool bRtn = true;
    try
    {
        sret_ = SQLExecute(a_sqlHStmt);

        if(SQL_SUCCESS == sret_)
        {
            sret_ = SQLFetch(a_sqlHStmt);
            if(SQL_SUCCESS != sret_)
            {
                if(SQL_NO_DATA != sret_)
                {
                    bRtn = false;
                    GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,__func__);
                    _THD_LOG(NDF_LOG_T_WARNING, "SQLRETURN[%p][%d:%s]",a_sqlHStmt, sret_,m_szODBCError);
                }
            }
            else
            {
                DBTimeToTimeT(m_tmsCreateTime,m_stSMFSessionInfo.tCreateTime);
                DBTimeToTimeT(m_tmsUpdateTime,m_stSMFSessionInfo.tUpdateTime);
            }
        }
        else
        {
            if(SQL_NO_DATA != sret_)
            {
                bRtn = false;
                GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,__func__);
                _THD_LOG(NDF_LOG_T_WARNING, "SQLRETURN[%p][%d:%s]",a_sqlHStmt, sret_,m_szODBCError);
            }
        }

    }
    catch(CPCFException &e)
    {
        if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
        {
                if(SQL_NO_DATA == sret_)
                {
                    _THD_LOG(NDF_LOG_T_INFO,"[%s]",e.what());
                }
                else
                {
                    GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,(char*)className_.data(),sret_,e.what());
                }
        }
        if ((e.m_nECode & E_ERROR_STAT_ADD) == E_ERROR_STAT_ADD)
        {
        }
        if ((e.m_nECode & E_ERROR_SEND) == E_ERROR_SEND)
        {
        }
        bRtn = false;
    }
    catch(std::exception &e)
    {
        _THD_LOG(NDF_LOG_T_INFO, "[%s][%s]",typeid(e).name(), e.what());
        bRtn = false;
    }

    return bRtn;
}

