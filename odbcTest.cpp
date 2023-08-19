#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <typeinfo>
#include <iostream>
#include <thread>
#include <mutex>

#include <unistd.h>
#include <sys/syscall.h>
#define LWP                     syscall(SYS_gettid)

#define PRINTF(fmt,...) printf("[%s:%-15s:%03d][%lu] " fmt,__FILE__,__func__,__LINE__,LWP,__VA_ARGS__) 

#define MAXBUFLEN 1024

std::mutex g_mutex;

class CConnection 
{
    private:
        int       m_nUuid;
        char      m_szClassName[128];
        bool      m_bConnection;
        unsigned char *m_odbcDSN ;
        SQLHENV   m_henv;
        SQLHDBC   m_hdbc;
        SQLRETURN m_sret;
        UCHAR     m_SQLState[MAXBUFLEN];
        UCHAR     m_MessageText[MAXBUFLEN];
        SDWORD    m_NativeErrorPtr;
        SWORD     m_TextLengthPtr;


    public:
        char      m_szODBCError[MAXBUFLEN+1];
    public:
        CConnection(int a_nUuid);
        int  GetUuid() { return m_nUuid; }
        bool Initialize(unsigned char *a_odbcDSN);
        bool Finalize();
        void Clear();
        bool GetDiagRec(
                SQLSMALLINT handleType,
                SQLHANDLE  handle,
                const char *a_pszClassName,
                SQLRETURN   a_sqlReturn,
                const char *a_pszCatch);
        bool AllocStmt(SQLHSTMT &sqlHstmt);
        bool FreeStmt(SQLHSTMT &sqlHstmt);
        bool Connect();
        bool Disconnect();
};

CConnection::CConnection(int a_nUuid)
{
    m_nUuid = a_nUuid;
    snprintf(m_szClassName,sizeof m_szClassName,"%s",typeid(*this).name());
    m_bConnection = false;
    m_henv = SQL_NULL_HENV;
    m_hdbc = SQL_NULL_HDBC;
}

bool CConnection::Initialize(unsigned char *a_odbcDSN)
{
    bool bRtn = true;
    try
    {
        m_odbcDSN = a_odbcDSN;
        m_sret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv); 
        if(SQL_SUCCEEDED(m_sret) != true)
        {
            GetDiagRec(SQL_HANDLE_ENV,m_henv,"SQLAllocHandle",m_sret,__func__);
            throw "SQLAllocHandle(SQL_HANDLE_ENV";
        }
        PRINTF("[%p]\n",m_henv);
        m_sret = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
        if(SQL_SUCCEEDED(m_sret) != true)
        {
            GetDiagRec(SQL_HANDLE_ENV,m_henv,"SQLSetEnvAttr",m_sret,__func__);
            throw "SQLAllocHandle(SQL_HANDLE_DBC";
        }

        m_sret = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc); 
        if(SQL_SUCCEEDED(m_sret) != true)
        {
            GetDiagRec(SQL_HANDLE_ENV,m_henv,"SQLAllocHandle",m_sret,__func__);
            throw "SQLAllocHandle(SQL_HANDLE_DBC";
        }
        PRINTF("[%p]\n",m_hdbc);
    }
    catch(const char * e)
    {
        PRINTF("ERROR [%s][%s]\n",e,m_szODBCError);
        bRtn = false;
    }
    return bRtn;
}

bool CConnection::Finalize()
{
    bool bRtn = true;
    try
    {
        PRINTF("[%p]\n",m_hdbc);
        m_sret = SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc); 
        if(SQL_SUCCEEDED(m_sret) != true)
        {
            GetDiagRec(SQL_HANDLE_DBC,m_hdbc,"SQLFreeHandle",m_sret,__func__);
            throw "SQLFreeHandle(SQL_HANDLE_DBC)";
        }
        m_hdbc = SQL_NULL_HDBC;
        PRINTF("[%p]\n",m_henv);
        m_sret = SQLFreeHandle(SQL_HANDLE_ENV, m_henv); 
        if(SQL_SUCCEEDED(m_sret) != true)
        {
            GetDiagRec(SQL_HANDLE_ENV,m_henv,"SQLFreeHandle",m_sret,__func__);
            throw "SQLFreeHandle(SQL_HANDLE_ENV)";
        }
        m_henv = SQL_NULL_HENV;
    }
    catch(const char * e)
    {
        PRINTF("ERROR [%s][%s]\n",e,m_szODBCError);
        bRtn = false;
    }
    return bRtn;
}

bool CConnection::AllocStmt(SQLHSTMT &sqlHstmt)
{
    bool bRtn = true;
    try
    {
        if(m_bConnection==true)
        {
            if(sqlHstmt != SQL_NULL_HSTMT)
            {
                FreeStmt(sqlHstmt);
            }
          //std::lock_guard<std::mutex> lg(g_mutex);
            m_sret = SQLAllocHandle(SQL_HANDLE_STMT,m_hdbc,&sqlHstmt);
            if(SQL_SUCCEEDED(m_sret) != true)
            {
                bRtn = false;
                throw "SQLAllocHandle(SQL_HANDLE_STMT)";
            }
          //PRINTF("[%p]\n",sqlHstmt);
        }
    }
    catch(const char * e)
    {
        GetDiagRec(SQL_HANDLE_STMT,sqlHstmt,"SQLAllocHandle",m_sret,__func__);
        PRINTF("ERROR [%s][%s]\n",e,m_szODBCError);
        bRtn = false;
    }
    return bRtn;
}

bool CConnection::FreeStmt(SQLHSTMT &sqlHstmt)
{
    bool bRtn = true;
    try
    {
        if(m_bConnection==true)
        {
          //PRINTF("[%p]\n",sqlHstmt);
            m_sret = SQLFreeHandle(SQL_HANDLE_STMT,sqlHstmt);
            if(SQL_SUCCEEDED(m_sret) != true)
            {
                bRtn = false;
                throw "SQLFreeHandle(SQL_HANDLE_STMT)";
            }
        }
        sqlHstmt = SQL_NULL_HSTMT;
    }
    catch(const char * e)
    {
        GetDiagRec(SQL_HANDLE_STMT,sqlHstmt,"SQLFreeHandle",m_sret,__func__);
        PRINTF("ERROR [%s][%s]\n",e,m_szODBCError);
        bRtn = false;
    }
    return bRtn;
}

bool CConnection::Connect()
{
    bool bRtn = true;
    try
    {
        for(int i = 0 ; i < 3; ++i)
        {
            m_sret = SQLDriverConnect(m_hdbc,
                    nullptr,
                    m_odbcDSN,
                    SQL_NTS,
                    nullptr,
                    0,
                    nullptr,
                    SQL_DRIVER_NOPROMPT);
            if(SQL_SUCCEEDED(m_sret) != true)
            {
                if(i==2)
                {
                    throw "SQLDriverConnect";
                }
                continue;
            }
            break;
        }
        m_bConnection = true;
    }
    catch(const char * e)
    {
        GetDiagRec(SQL_HANDLE_DBC,m_hdbc,"SQLDisconnect",m_sret,__func__);
        PRINTF("ERROR [%s][%s]\n",e,m_szODBCError);
        bRtn = false;
    }
    return bRtn;
}

bool CConnection::Disconnect()
{
    bool bRtn = true;
    try
    {
        m_bConnection = false;
        m_sret = SQLDisconnect(m_hdbc);
        if(SQL_SUCCEEDED(m_sret) != true)
        {
            bRtn = false;
            throw "SQLDisconnect";
        }
    }
    catch(const char * e)
    {
        GetDiagRec(SQL_HANDLE_DBC,m_hdbc,"SQLDisconnect",m_sret,__func__);
        PRINTF("ERROR [%s][%s]\n",e,m_szODBCError);
        bRtn = false;
    }
    return bRtn;
}


/**
 * \brief         이전 에러 클리어 
 * \detail         
 * \param[in]     type:void 
 * \return        type:void 
 *
 * */
void CConnection::Clear()
{
    m_szODBCError[0] = 0x00;
    m_SQLState[0]    = 0x00;
    m_MessageText[0] = 0x00;
    m_NativeErrorPtr = 0L;
    m_TextLengthPtr  = 0;
}
/**
 * \brief         핸들타입과 핸들을 받아서 ODBC 에러를 확인한다.
 * \detail         
 * \param[in]     type:SQLSMALLINT  handleType 
 * \param[in]     type:SQLHANDLE    handle 
 * \param[in]     type:char         수행중인 작업. 
 * \param[in]     type:SQLRETURN    수행중에 받은 에러결과
 * \return        type:bool         true/false 
 *
 * */
bool CConnection::GetDiagRec(
                SQLSMALLINT handleType,
                SQLHANDLE  handle,
          const char       *a_pszClassName,
                SQLRETURN   a_sqlReturn,
          const char       *a_pszCatch)
{
    bool bRtn = true;

    Clear();

    /*
     * 데이터 없는 경우 처리
     */
    if(a_sqlReturn==SQL_NO_DATA)
    {
        snprintf(m_szODBCError,sizeof(m_szODBCError),
                "%s",a_pszCatch);
        return bRtn;
    }
    SQLRETURN sqlReturn = SQL_SUCCESS;
    SQLINTEGER recCount;
    sqlReturn = SQLGetDiagField( handleType, handle, 1,
            SQL_DIAG_NUMBER, &recCount,
            0, 0);  
    if(sqlReturn != SQL_SUCCESS)
    {
#if 0
        snprintf(m_szODBCError,sizeof(m_szODBCError),
                "SQLGetDiagField fail[%s][%d][%d]",a_pszCatch,a_sqlReturn,sqlReturn);
        bRtn = false;
        return bRtn;
#else
        recCount = 1;
#endif
    }

    SQLSMALLINT  recIdx;
    int nOffset =  0;

    for(recIdx = 0 ; recIdx < recCount;recIdx++)
    {

        sqlReturn = SQLGetDiagRec(handleType,
                handle,
                recIdx+1,
                m_SQLState,
                &m_NativeErrorPtr,
                m_MessageText,
                MAXBUFLEN-1,
                &m_TextLengthPtr);

        if(a_pszCatch!=NULL)
        {
            nOffset += snprintf(m_szODBCError+nOffset,sizeof(m_szODBCError)- nOffset,
                    "[%s:%s] %d/%s/%s",a_pszClassName,a_pszCatch,a_sqlReturn,m_SQLState,m_MessageText);

        }
        else
        {
            nOffset += snprintf(m_szODBCError+nOffset,sizeof(m_szODBCError)- nOffset,
                    "[%s] %d/%s/%s",a_pszClassName,a_sqlReturn,m_SQLState,m_MessageText);
        }
    }

    return bRtn;
}

typedef struct __st_stmt_info__
{
    SQLHSTMT hstmt;
    int      uuid;
} stStmtInfo;

#include <vector>

class CPreparedSQL
{
    private:
        int m_nmaxCnt;
    public:
        CPreparedSQL();
    SQLHSTMT & AllocStmt(CConnection *a_c,int a_nIdx);
    std::vector<stStmtInfo>         vecStmtInfo_;
};

CPreparedSQL::CPreparedSQL()
{
    m_nmaxCnt = 12;
}

SQLHSTMT & CPreparedSQL::AllocStmt(CConnection *a_c,int a_nIdx)
{
    stStmtInfo & info = vecStmtInfo_.at(a_nIdx);

    int uuid = a_c->GetUuid();

    if(info.hstmt == SQL_NULL_HSTMT || uuid != info.uuid)
    {
        if(a_c->AllocStmt(info.hstmt) == false)
        {
            info.hstmt = SQL_NULL_HSTMT;
            return info.hstmt;
        }
    }
    return info.hstmt;
}


void Test1()
{
    SQLCHAR * odbcDSN         = (SQLCHAR*)"DSN=GOLD_GLOBAL;HOST=192.168.15.185;UID=pdb;PWD=pdb1234;PORT=22581;CONNECTION_TIMEOUT=5;LOGIN_TIMEOUT=3;PROTOCOL=TCP;FAILOVER_TYPE=SESSION;USE_GLOBAL_SESSION=1;LOCALITY_AWARE_TRANSACTION=1;LOCATOR_DSN=LOCATOR";
    SQLHSTMT            hstmtA=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA1=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA2=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA3=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA4=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA5=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA6=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA7=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA8=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA9=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA10=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB1=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB2=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB3=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB4=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB5=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB6=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB7=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB8=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB9=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB10=SQL_NULL_HSTMT;


    CConnection clsConn20(20);
    CConnection clsConn21(21);
    CConnection clsConn22(22);

    clsConn20.Initialize(odbcDSN);
    clsConn20.Connect();
    clsConn21.Initialize(odbcDSN);
    clsConn21.Connect();
    clsConn22.Initialize(odbcDSN);
    clsConn22.Connect();

    clsConn20.AllocStmt(hstmtA);
    clsConn22.AllocStmt(hstmtB);
    clsConn20.AllocStmt(hstmtA1);
    clsConn22.AllocStmt(hstmtB1);
    clsConn20.AllocStmt(hstmtA2);
    clsConn22.AllocStmt(hstmtB2);
    clsConn20.AllocStmt(hstmtA3);
    clsConn22.AllocStmt(hstmtB3);
    clsConn20.AllocStmt(hstmtA4);
    clsConn22.AllocStmt(hstmtB4);
    clsConn20.AllocStmt(hstmtA5);
    clsConn22.AllocStmt(hstmtB5);
    clsConn20.AllocStmt(hstmtA6);
    clsConn22.AllocStmt(hstmtB6);
    clsConn20.AllocStmt(hstmtA7);
    clsConn22.AllocStmt(hstmtB7);
    clsConn20.AllocStmt(hstmtA8);
    clsConn22.AllocStmt(hstmtB8);
    clsConn20.AllocStmt(hstmtA9);
    clsConn22.AllocStmt(hstmtB9);
    clsConn20.AllocStmt(hstmtA10);
    clsConn22.AllocStmt(hstmtB10);
    PRINTF("hstmtA [%p]\n",hstmtA);
    PRINTF("hstmtA 1[%p]\n",hstmtA1);
    PRINTF("hstmtA 2[%p]\n",hstmtA2);
    PRINTF("hstmtA 3[%p]\n",hstmtA3);
    PRINTF("hstmtA 4[%p]\n",hstmtA4);
    PRINTF("hstmtA 5[%p]\n",hstmtA5);
    PRINTF("hstmtA 6[%p]\n",hstmtA6);
    PRINTF("hstmtA 7[%p]\n",hstmtA7);
    PRINTF("hstmtA 8[%p]\n",hstmtA8);
    PRINTF("hstmtA 9[%p]\n",hstmtA9);
    PRINTF("hstmtA10[%p]\n",hstmtA10);
    PRINTF("hstmtB [%p]\n" ,hstmtB);
    PRINTF("hstmtB 1[%p]\n",hstmtB1);
    PRINTF("hstmtB 2[%p]\n",hstmtB2);
    PRINTF("hstmtB 3[%p]\n",hstmtB3);
    PRINTF("hstmtB 4[%p]\n",hstmtB4);
    PRINTF("hstmtB 5[%p]\n",hstmtB5);
    PRINTF("hstmtB 6[%p]\n",hstmtB6);
    PRINTF("hstmtB 7[%p]\n",hstmtB7);
    PRINTF("hstmtB 8[%p]\n",hstmtB8);
    PRINTF("hstmtB 9[%p]\n",hstmtB9);
    PRINTF("hstmtB10[%p]\n",hstmtB10);
    clsConn21.AllocStmt(hstmtA);
    clsConn21.AllocStmt(hstmtA1);
    clsConn21.AllocStmt(hstmtA2);
    clsConn21.AllocStmt(hstmtA3);
    clsConn21.AllocStmt(hstmtA4);
    clsConn21.AllocStmt(hstmtA5);
  //clsConn21.AllocStmt(hstmtA6);
  //clsConn21.AllocStmt(hstmtA7);
  //clsConn21.AllocStmt(hstmtA8);
  //clsConn21.AllocStmt(hstmtA9);
  //clsConn21.AllocStmt(hstmtA10);
    PRINTF("hstmtA [%p]\n",hstmtA);
    PRINTF("hstmtA 1[%p]\n",hstmtA1);
    PRINTF("hstmtA 2[%p]\n",hstmtA2);
    PRINTF("hstmtA 3[%p]\n",hstmtA3);
    PRINTF("hstmtA 4[%p]\n",hstmtA4);
    PRINTF("hstmtA 5[%p]\n",hstmtA5);
    PRINTF("hstmtA 6[%p]\n",hstmtA6);
    PRINTF("hstmtA 7[%p]\n",hstmtA7);
    PRINTF("hstmtA 8[%p]\n",hstmtA8);
    PRINTF("hstmtA 9[%p]\n",hstmtA9);
    PRINTF("hstmtA10[%p]\n",hstmtA10);

    clsConn20.Disconnect();
    clsConn20.Connect();
    clsConn20.AllocStmt(hstmtA);
    clsConn20.AllocStmt(hstmtA1);
    clsConn20.AllocStmt(hstmtA2);
  //clsConn20.AllocStmt(hstmtA3);
  //clsConn20.AllocStmt(hstmtA4);
    clsConn20.AllocStmt(hstmtA5);
    clsConn20.AllocStmt(hstmtA6);
    clsConn20.AllocStmt(hstmtA7);
  //clsConn20.AllocStmt(hstmtA8);
  //clsConn20.AllocStmt(hstmtA9);
    clsConn20.AllocStmt(hstmtA10);
    PRINTF("hstmtA [%p]\n",hstmtA);
    PRINTF("hstmtA 1[%p]\n",hstmtA1);
    PRINTF("hstmtA 2[%p]\n",hstmtA2);
    PRINTF("hstmtA 3[%p]\n",hstmtA3);
    PRINTF("hstmtA 4[%p]\n",hstmtA4);
    PRINTF("hstmtA 5[%p]\n",hstmtA5);
    PRINTF("hstmtA 6[%p]\n",hstmtA6);
    PRINTF("hstmtA 7[%p]\n",hstmtA7);
    PRINTF("hstmtA 8[%p]\n",hstmtA8);
    PRINTF("hstmtA 9[%p]\n",hstmtA9);
    PRINTF("hstmtA10[%p]\n",hstmtA10);

    clsConn20.Disconnect();
    clsConn21.Disconnect();
    clsConn22.Disconnect();

    clsConn20.Finalize();
    clsConn21.Finalize();
    clsConn22.Finalize();
}

void Test2(int a_nIdx)
{
    SQLCHAR * odbcDSN         = (SQLCHAR*)"DSN=GOLD_GLOBAL;HOST=192.168.15.185;UID=pdb;PWD=pdb1234;PORT=22581;CONNECTION_TIMEOUT=5;LOGIN_TIMEOUT=3;PROTOCOL=TCP;FAILOVER_TYPE=SESSION;USE_GLOBAL_SESSION=1;LOCALITY_AWARE_TRANSACTION=1;LOCATOR_DSN=LOCATOR";
    SQLHSTMT            hstmtA=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA1=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA2=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA3=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA4=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA5=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA6=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA7=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA8=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA9=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtA10=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB1=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB2=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB3=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB4=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB5=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB6=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB7=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB8=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB9=SQL_NULL_HSTMT;
    SQLHSTMT            hstmtB10=SQL_NULL_HSTMT;


    CConnection clsConn20(20);
    CConnection clsConn21(21);
    CConnection clsConn22(22);

    clsConn20.Initialize(odbcDSN);
    clsConn20.Connect();
    clsConn21.Initialize(odbcDSN);
    clsConn21.Connect();
    clsConn22.Initialize(odbcDSN);
    clsConn22.Connect();

    clsConn20.AllocStmt(hstmtA);
    clsConn20.AllocStmt(hstmtB);
    clsConn20.AllocStmt(hstmtA1);
    clsConn20.AllocStmt(hstmtB1);
    clsConn20.AllocStmt(hstmtA2);
    clsConn20.AllocStmt(hstmtB2);
    clsConn20.AllocStmt(hstmtA3);
    clsConn20.AllocStmt(hstmtB3);
    clsConn20.AllocStmt(hstmtA4);
    clsConn20.AllocStmt(hstmtB4);
    clsConn20.AllocStmt(hstmtA5);
    clsConn22.AllocStmt(hstmtB5);
    clsConn22.AllocStmt(hstmtA6);
    clsConn22.AllocStmt(hstmtB6);
    clsConn22.AllocStmt(hstmtA7);
    clsConn22.AllocStmt(hstmtB7);
    clsConn22.AllocStmt(hstmtA8);
    clsConn22.AllocStmt(hstmtB8);
    clsConn22.AllocStmt(hstmtA9);
    clsConn22.AllocStmt(hstmtB9);
    clsConn22.AllocStmt(hstmtA10);
    clsConn22.AllocStmt(hstmtB10);
    PRINTF("[%d] hstmtA  [%p]\n",a_nIdx,hstmtA);
    PRINTF("[%d] hstmtA 1[%p]\n",a_nIdx,hstmtA1);
    PRINTF("[%d] hstmtA 2[%p]\n",a_nIdx,hstmtA2);
    PRINTF("[%d] hstmtA 3[%p]\n",a_nIdx,hstmtA3);
    PRINTF("[%d] hstmtA 4[%p]\n",a_nIdx,hstmtA4);
    PRINTF("[%d] hstmtA 5[%p]\n",a_nIdx,hstmtA5);
    PRINTF("[%d] hstmtA 6[%p]\n",a_nIdx,hstmtA6);
    PRINTF("[%d] hstmtA 7[%p]\n",a_nIdx,hstmtA7);
    PRINTF("[%d] hstmtA 8[%p]\n",a_nIdx,hstmtA8);
    PRINTF("[%d] hstmtA 9[%p]\n",a_nIdx,hstmtA9);
    PRINTF("[%d] hstmtA10[%p]\n",a_nIdx,hstmtA10);
    PRINTF("[%d] hstmtB  [%p]\n",a_nIdx,hstmtB);
    PRINTF("[%d] hstmtB 1[%p]\n",a_nIdx,hstmtB1);
    PRINTF("[%d] hstmtB 2[%p]\n",a_nIdx,hstmtB2);
    PRINTF("[%d] hstmtB 3[%p]\n",a_nIdx,hstmtB3);
    PRINTF("[%d] hstmtB 4[%p]\n",a_nIdx,hstmtB4);
    PRINTF("[%d] hstmtB 5[%p]\n",a_nIdx,hstmtB5);
    PRINTF("[%d] hstmtB 6[%p]\n",a_nIdx,hstmtB6);
    PRINTF("[%d] hstmtB 7[%p]\n",a_nIdx,hstmtB7);
    PRINTF("[%d] hstmtB 8[%p]\n",a_nIdx,hstmtB8);
    PRINTF("[%d] hstmtB 9[%p]\n",a_nIdx,hstmtB9);
    PRINTF("[%d] hstmtB10[%p]\n",a_nIdx,hstmtB10);

    clsConn21.AllocStmt(hstmtA);
    clsConn21.AllocStmt(hstmtB);
    clsConn21.AllocStmt(hstmtA1);
    clsConn21.AllocStmt(hstmtB1);
    clsConn21.AllocStmt(hstmtA2);
    clsConn21.AllocStmt(hstmtB2);
    clsConn21.AllocStmt(hstmtA3);
    clsConn21.AllocStmt(hstmtB3);
    clsConn21.AllocStmt(hstmtA4);
    clsConn21.AllocStmt(hstmtB4);
    clsConn21.AllocStmt(hstmtA5);
    clsConn21.AllocStmt(hstmtB5);
    PRINTF("[%d] hstmtA  [%p]\n",a_nIdx,hstmtA);
    PRINTF("[%d] hstmtB  [%p]\n",a_nIdx,hstmtB);
    PRINTF("[%d] hstmtA 1[%p]\n",a_nIdx,hstmtA1);
    PRINTF("[%d] hstmtB 1[%p]\n",a_nIdx,hstmtB1);
    PRINTF("[%d] hstmtA 2[%p]\n",a_nIdx,hstmtA2);
    PRINTF("[%d] hstmtB 2[%p]\n",a_nIdx,hstmtB2);
    PRINTF("[%d] hstmtA 3[%p]\n",a_nIdx,hstmtA3);
    PRINTF("[%d] hstmtB 3[%p]\n",a_nIdx,hstmtB3);
    PRINTF("[%d] hstmtA 4[%p]\n",a_nIdx,hstmtA4);
    PRINTF("[%d] hstmtB 4[%p]\n",a_nIdx,hstmtB4);
    PRINTF("[%d] hstmtA 5[%p]\n",a_nIdx,hstmtA5);
    PRINTF("[%d] hstmtB 5[%p]\n",a_nIdx,hstmtB5);

    clsConn20.Disconnect();
    clsConn20.Connect();

    clsConn20.AllocStmt(hstmtA);
    clsConn20.AllocStmt(hstmtB);
    clsConn20.AllocStmt(hstmtA1);
    clsConn20.AllocStmt(hstmtB1);
    clsConn20.AllocStmt(hstmtA2);
    clsConn20.AllocStmt(hstmtB2);
    clsConn20.AllocStmt(hstmtA3);
    clsConn20.AllocStmt(hstmtB3);
    clsConn20.AllocStmt(hstmtA4);
    clsConn20.AllocStmt(hstmtB4);
    clsConn20.AllocStmt(hstmtA5);
    clsConn20.AllocStmt(hstmtB5);
    PRINTF("[%d] hstmtA [%p]\n" ,a_nIdx,hstmtA);
    PRINTF("[%d] hstmtB [%p]\n" ,a_nIdx,hstmtB);
    PRINTF("[%d] hstmtA 1[%p]\n",a_nIdx,hstmtA1);
    PRINTF("[%d] hstmtB 1[%p]\n",a_nIdx,hstmtB1);
    PRINTF("[%d] hstmtA 2[%p]\n",a_nIdx,hstmtA2);
    PRINTF("[%d] hstmtB 2[%p]\n",a_nIdx,hstmtB2);
    PRINTF("[%d] hstmtA 3[%p]\n",a_nIdx,hstmtA3);
    PRINTF("[%d] hstmtB 3[%p]\n",a_nIdx,hstmtB3);
    PRINTF("[%d] hstmtB 4[%p]\n",a_nIdx,hstmtB4);
    PRINTF("[%d] hstmtA 5[%p]\n",a_nIdx,hstmtA5);
    PRINTF("[%d] hstmtB 5[%p]\n",a_nIdx,hstmtB5);

    clsConn20.Disconnect();
    clsConn21.Disconnect();
    clsConn22.Disconnect();

    clsConn20.Finalize();
    clsConn21.Finalize();
    clsConn22.Finalize();
}



int main(int argc, char **argv)
{
  //Test1();
    std::thread t1(Test2,1);
    std::thread t2(Test2,2);
    std::thread t3(Test2,3);
    std::thread t4(Test2,4);
    std::thread t5(Test2,5);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    return 0;
}
