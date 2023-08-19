#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "CPDBConn.hpp"
#include "define.hpp"

namespace PDB 
{

    /**
     * \brief         Construction 
     * \detail         
     * \param[in]     type:unsigned long   uuid 
     * \return        type:void 
     *
     * */
    CPDBConn::CPDBConn(unsigned long a_ulUUID)
    {
        m_bConnection = false;
        m_bAutoCommit = false;
        m_ulUUID = a_ulUUID;
        _THD_LOG(NDF_LOG_T_INFO,"[%lu] uuid",m_ulUUID);
        m_sqlHEnv = SQL_NULL_HENV;
        m_sqlHDbc = SQL_NULL_HDBC;
    }

    /**
     * \brief         Destruction 
     * \detail         
     * \param[in]     type:void
     * \return        type:void 
     *
     * */
    CPDBConn::~CPDBConn()
    {
    }

    bool CPDBConn::ChkStmt(int a_nLine)
    {
        bool bRtn = false;
        return bRtn;
        char szStmt[1024];
        for (const PA_STMT &kv : m_mapStmt)
        {
            snprintf(szStmt,sizeof szStmt,"%p",*kv.second);
            if(kv.second != nullptr && *kv.second != SQL_NULL_HSTMT)
            {
                if(strcmp(szStmt,kv.first.data()) != 0)
                {
                    _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p:%-20p][%s] diff line[%d] "
                            ,m_ulUUID
                            ,*kv.second
                            ,kv.second
                            ,kv.first.data()
                            ,a_nLine
                            );
                    bRtn = true;
                }
                else
                {
                    _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p:%-20p][%s] same line[%d] "
                            ,m_ulUUID
                            ,*kv.second
                            ,kv.second
                            ,kv.first.data()
                            ,a_nLine
                            );
                }
            }
        }
        return bRtn;
    }

    /**
     * \brief         보관중인 stmt를 모두 free한다. 
     * \detail         
     * \param[in]     type:const char* 호출한 function
     * \param[in]     type:int         호출한 line
     * \return        tye:void 
     *
     * */
    void CPDBConn::FreeStmt(const char *a_szFunc,int a_nLine)
    {
        if(m_mapStmt.size()>0)
        {
            _THD_LOG(NDF_LOG_T_INFO,"[%lu] SQLHSTMT map.size(%lu) [%s:%d]",m_ulUUID,m_mapStmt.size(),a_szFunc,a_nLine);
            for (PA_STMT &kv : m_mapStmt)
            {
                if(kv.second != nullptr )
                {
                    if(*kv.second != SQL_NULL_HSTMT)
                    {
                        m_sqlReturn = SQLFreeHandle(SQL_HANDLE_STMT,*kv.second);
                        if(SQL_SUCCESS == m_sqlReturn )
                        {
                            _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p][%s] SQLFreeHandle succ [%s:%d]"
                                    ,m_ulUUID
                                    ,*kv.second
                                    ,kv.second
                                    ,kv.first.data()
                                    ,a_szFunc,a_nLine);
                            *kv.second = SQL_NULL_HSTMT;
                        }
                        else
                        {
                            GetDiagRec(SQL_HANDLE_STMT,*kv.second,m_sqlReturn,__func__);
                            _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p][%s] SQLFreeHandle fail[%d:%s] [%s:%d]"
                                    ,m_ulUUID
                                    ,*kv.second
                                    ,kv.second
                                    ,kv.first.data()
                                    ,m_sqlReturn,m_szErrMsg,a_szFunc,a_nLine);
                        }
                    }
                    else
                    {
                        _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p:%-20p][%s] SQL_NULL_HSTMT [%s:%d]"
                                ,m_ulUUID
                                ,*kv.second
                                ,kv.second
                                ,kv.first.data()
                                ,a_szFunc,a_nLine);
                        delete kv.second;
                    }
                }
                else
                {
                    _THD_LOG(NDF_LOG_T_WARNING,"[%lu][%-20p][%s] nullptr [%s:%d]"
                            ,m_ulUUID
                            ,kv.second
                            ,kv.first.data()
                            ,a_szFunc,a_nLine);
                }
            }

            m_mapStmt.clear();
        }
    }

    /**
     * \brief         error msg를 초기화 한다.
     * \detail         
     * \param[in]     type:void 
     * \return        type:void 
     *
     * */
    void CPDBConn::Clear()
    {
        m_szErrMsg[0] = 0x00;
        m_SQLState[0]    = 0x00;
        m_MessageText[0] = 0x00;
        m_NativeErrorPtr = 0L;
        m_TextLengthPtr  = 0;
    }

    /**
     * \brief         DB 연결전 handle 생성. 
     * \detail         
     * \param[in]     type:void 
     * \return        type:SQLRETURN     ODBC 명령 결과 값 
     *
     * */
    SQLRETURN CPDBConn::PreConnect()
    {
        if ( m_sqlHDbc == SQL_NULL_HDBC)
        {
            m_sqlReturn = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_sqlHEnv);
            if(m_sqlReturn == SQL_ERROR || m_sqlReturn == SQL_INVALID_HANDLE)
            {
                GetDiagRec(SQL_HANDLE_ENV,m_sqlHEnv,m_sqlReturn,__func__);
                _THD_LOG(NDF_LOG_T_ERROR,"[%lu] SQLAllocHandle[%d:%s]",m_ulUUID,m_sqlReturn,m_szErrMsg);
                return m_sqlReturn;
            }
            _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] SQLENV",m_ulUUID,m_sqlHEnv,&m_sqlHEnv);

            m_sqlReturn = SQLSetEnvAttr(m_sqlHEnv, SQL_ATTR_ODBC_VERSION,(SQLPOINTER*)SQL_OV_ODBC3,0);
            if(m_sqlReturn == SQL_ERROR || m_sqlReturn == SQL_INVALID_HANDLE)
            {
                GetDiagRec(SQL_HANDLE_ENV,m_sqlHEnv,m_sqlReturn,__func__);
                _THD_LOG(NDF_LOG_T_ERROR,"[%lu:%-20p] [%d:%s]",m_ulUUID,m_sqlHEnv,m_sqlReturn,m_szErrMsg);
                return m_sqlReturn;
            }

            if(m_sqlHDbc != SQL_NULL_HDBC)
            {
                SQLDisconnect(m_sqlHDbc);
                SQLFreeHandle(SQL_HANDLE_DBC, m_sqlHDbc);
                m_sqlHDbc = SQL_NULL_HDBC;
            }

            m_sqlReturn = SQLAllocHandle(SQL_HANDLE_DBC, m_sqlHEnv, &m_sqlHDbc);
            if(m_sqlReturn == SQL_ERROR || m_sqlReturn == SQL_INVALID_HANDLE)
            {
                GetDiagRec(SQL_HANDLE_ENV,m_sqlHEnv,m_sqlReturn,__func__);
                _THD_LOG(NDF_LOG_T_ERROR,"[%lu:%-20p] [%d:%s]",m_ulUUID,m_sqlHEnv,m_sqlReturn,m_szErrMsg);
                /*
                 * 필요한가?
                 */
                Disconnect();
                return m_sqlReturn;
            }
            _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] SQLDBC",m_ulUUID,m_sqlHDbc,&m_sqlHDbc);
        }
        return m_sqlReturn;
    }

    /**
     * \brief         DB에 연결 
     * \detail         
     * \param[in]     type:int           연결 시도 횟수 
     * \return        type:SQLRETURN     ODBC 명령 결과 값 
     *
     * */
    SQLRETURN CPDBConn::Connect(int a_nTryCnt)
    {
        if(a_nTryCnt==0)
        {
            //snprintf(m_szErrMsg,sizeof m_szErrMsg,"[%s:%d] %s",__FILE__,__LINE__,"Connect fail");
            return SQL_ERROR;
        }
        m_sqlReturn = SQLDriverConnect(
                m_sqlHDbc
                ,nullptr
                ,(SQLCHAR*)m_stringODBC.data()
                ,SQL_NTS
                ,nullptr
                ,0
                ,nullptr
                ,SQL_DRIVER_NOPROMPT
                );

        _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] ODBC_CONFIG[%s]",m_ulUUID,m_sqlHDbc,&m_sqlHDbc,m_stringODBC.data());

        if(m_sqlReturn == SQL_SUCCESS)
        {
            if(m_bAutoCommit == true) {
                m_sqlReturn = SQLSetConnectAttr(m_sqlHDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);

                if(m_sqlReturn != SQL_SUCCESS) {
                    _THD_LOG(NDF_LOG_T_ERROR, "[%lu][%-20p:%-20p] SQLSetConnectAttr SQL_AUTOCOMMIT_ON fail [%d]"
                            ,m_ulUUID
                            ,m_sqlHDbc
                            ,&m_sqlHDbc
                            ,m_sqlReturn);
                }
            }
            else if(m_bAutoCommit == false) {
                m_sqlReturn = SQLSetConnectAttr(m_sqlHDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);

                if(m_sqlReturn != SQL_SUCCESS) {
                    _THD_LOG(NDF_LOG_T_ERROR, "[%lu][%-20p:%-20p] SQLSetConnectAttr SQL_AUTOCOMMIT_OFF fail [%d]"
                            ,m_ulUUID
                            ,m_sqlHDbc
                            ,&m_sqlHDbc
                            ,m_sqlReturn);
                }
            }
            _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] SQLHDC",m_ulUUID,m_sqlHDbc,&m_sqlHDbc);
            m_bConnection = true;
            return  m_sqlReturn;
        }
        else if(m_sqlReturn == SQL_SUCCESS_WITH_INFO)
        {
            GetDiagRec(SQL_HANDLE_DBC,m_sqlHDbc,m_sqlReturn,__func__);
            _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p:%-20p] [%d:%s]",m_ulUUID,m_sqlHDbc,&m_sqlHDbc,m_sqlReturn,m_szErrMsg);
            return m_sqlReturn;
        }
        if(a_nTryCnt == 1)
        {
            GetDiagRec(SQL_HANDLE_DBC,m_sqlHDbc,m_sqlReturn,__func__);
            _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] [%d:%s]",m_ulUUID,m_sqlHDbc,&m_sqlHDbc,m_sqlReturn,m_szErrMsg);
        }

        return Connect(--a_nTryCnt);
    }

    /**
     * \brief         DB 연결을 해제  
     * \detail         
     * \param[in]     type:void 
     * \return        type:SQLRETURN     ODBC 명령 결과 값 
     *
     * */
    SQLRETURN CPDBConn::Disconnect()
    {
        if(m_bConnection == true)
        {
            if (m_sqlHDbc != SQL_NULL_HDBC)
            {
                FreeStmt(__func__,__LINE__);
                _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] SQLHDBC",m_ulUUID,m_sqlHDbc,&m_sqlHDbc);
                m_sqlReturn = SQLDisconnect(m_sqlHDbc);
                if (m_sqlReturn != SQL_SUCCESS)
                {
                    GetDiagRec(SQL_HANDLE_DBC, m_sqlHDbc, m_sqlReturn, __func__);
                    _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] [%d:%s]",m_ulUUID,m_sqlHDbc,&m_sqlHDbc,m_sqlReturn,m_szErrMsg);
                }
            }
            m_bConnection = false;
        }
        return m_sqlReturn;
    }

    /**
     * \brief         Disconnect 이후 handle 정리. 
     * \detail         
     * \param[in]     type:void 
     * \return        type:SQLRETURN     ODBC 명령 결과 값 
     *
     * */
    SQLRETURN CPDBConn::Postdisconnect()
    {
        if(m_sqlHDbc != SQL_NULL_HDBC)
        {
            _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] SQLHDBC",m_ulUUID,m_sqlHDbc,&m_sqlHDbc);
            m_sqlReturn = SQLFreeHandle(SQL_HANDLE_DBC,m_sqlHDbc);
            if(m_sqlReturn != SQL_SUCCESS)
            {
                GetDiagRec(SQL_HANDLE_DBC,m_sqlHDbc,m_sqlReturn,__func__);
                _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p][%d:%s]"
                        ,m_ulUUID
                        ,m_sqlHDbc
                        ,&m_sqlHDbc
                        ,m_sqlReturn,m_szErrMsg);
            }
            m_sqlHDbc = SQL_NULL_HDBC;
        }
        if(m_sqlHEnv != SQL_NULL_HENV)
        {
            _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] SQLHENV"
                    ,m_ulUUID
                    ,m_sqlHEnv
                    ,&m_sqlHEnv
                    );
            m_sqlReturn = SQLFreeHandle(SQL_HANDLE_ENV,m_sqlHEnv);
            if(m_sqlReturn != SQL_SUCCESS)
            {
                GetDiagRec(SQL_HANDLE_ENV,m_sqlHEnv,m_sqlReturn,__func__);
                _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] [%d:%s]"
                        ,m_ulUUID
                        ,m_sqlHEnv
                        ,&m_sqlHEnv
                        ,m_sqlReturn,m_szErrMsg);
            }
            m_sqlHEnv = SQL_NULL_HENV;
        }
        return m_sqlReturn;
    }

    /**
     * \brief         autocommit 이 아니거나 , transaction 인경우 commit 수행. 
     * \detail         
     * \param[in]     type:void 
     * \return        type:bool         true/false 
     *
     * */
    bool CPDBConn::Commit()
    {
        m_sqlReturn = SQLEndTran(SQL_HANDLE_DBC,m_sqlHDbc,SQL_COMMIT);
        if(m_sqlReturn != SQL_SUCCESS)
        {
            GetDiagRec(SQL_HANDLE_DBC,m_sqlHDbc,m_sqlReturn,__func__);
            _THD_LOG(NDF_LOG_T_ERROR, "[%lu][%-20p:%-20p] Commit.SQLEndTran fail [%d] [%s]"
                    ,m_ulUUID
                    ,m_sqlHDbc
                    ,&m_sqlHDbc
                    ,m_sqlReturn,m_szErrMsg);
            return false;
        }

        return false;
    }

    /**
     * \brief         autocommit 이 아니거나 , transaction 인경우 rollback 수행. 
     * \detail         
     * \param[in]     type:void 
     * \return        type:bool         true/false 
     *
     * */
    bool CPDBConn::Rollback()
    {
        m_sqlReturn = SQLEndTran(SQL_HANDLE_DBC,m_sqlHDbc,SQL_ROLLBACK);
        if(m_sqlReturn != SQL_SUCCESS)
        {
            GetDiagRec(SQL_HANDLE_DBC,m_sqlHDbc,m_sqlReturn,__func__);
            _THD_LOG(NDF_LOG_T_ERROR, "[%lu][%-20p:%-20p] Rollback.SQLEndTran fail [%d] [%s]"
                    ,m_ulUUID
                    ,m_sqlHDbc
                    ,&m_sqlHDbc
                    ,m_sqlReturn,m_szErrMsg);
            return false;
        }

        return false;
    }

    /**
     * \brief         stmt를 인자로 받아 STMT를 alloc한다.
     * \detail         
     * \param[in]     type:SQLHSTMT      stmt 
     * \return        type:bool         true/false 
     *
     * */
    bool CPDBConn::AllocStmt(SQLHSTMT &a_sqlHStmt)
    {
        if(m_sqlHDbc == SQL_NULL_HDBC)
        {
            return false;
        }
        m_sqlReturn = SQLAllocHandle(SQL_HANDLE_STMT,m_sqlHDbc,&a_sqlHStmt);
        if(m_sqlReturn != SQL_SUCCESS)
        {
            GetDiagRec(SQL_HANDLE_DBC,m_sqlHDbc,m_sqlReturn,__func__);
            _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] [%d:%s]",m_ulUUID,m_sqlHDbc,&m_sqlHDbc,m_sqlReturn,m_szErrMsg);
            return false;
        }
        char szStmt[1024];
        snprintf(szStmt,sizeof szStmt,"%p",a_sqlHStmt);
        RT_STMT ret = m_mapStmt.emplace(PA_STMT(szStmt,&a_sqlHStmt));
        if(ret.second == false)
        {
            _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] mapStmt.emplace error[%s:%s][%-20p:%-20p]"
                    ,m_ulUUID
                    ,a_sqlHStmt
                    ,&a_sqlHStmt
                    ,ret.first->first.data(),szStmt
                    ,*ret.first->second,a_sqlHStmt);
            return false;
        } 
        else
        {
            _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p:%-20p] [%-20p:%-20p] [%s] [%ld]"
                    ,m_ulUUID,a_sqlHStmt,&a_sqlHStmt
                    ,ret.first->second
                    ,&ret.first->second
                    ,szStmt,m_mapStmt.size());
        }

        return true;
    }


    /**
     * \brief         보관 중인 stmt를 돌려준다. 
     * \detail         
     * \param[in]    type:SQLHSTMT&     stmt&
     * \return       type:SQLHSTMT&     stmt&  
     *
     * */
    SQLHSTMT& CPDBConn::FindStmt(SQLHSTMT &a_sqlHStmt)
    {
        char szStmt[1024];
        snprintf(szStmt,sizeof szStmt,"%p",a_sqlHStmt);
        IT_STMT it = m_mapStmt.find(szStmt);
        if(it == m_mapStmt.end() )
        {
            m_sqlHStmt = SQL_NULL_HSTMT;
            return m_sqlHStmt;
        }
        return *it->second;
    }

    /**
     * \brief         alloc한 stmt를 free 한다.
     * \detail         
     * \param[in]     type:SQLHSTMT& stmt& 
     * \return        type:bool      true/false 
     *
     * */
    bool CPDBConn::ReleaseStmt(SQLHSTMT &a_sqlHStmt)
    {
        if(a_sqlHStmt == SQL_NULL_HSTMT)
        {
            _THD_LOG(NDF_LOG_T_ERROR,"[%lu:%-20p] SQL_NULL_HSTMT",m_ulUUID,a_sqlHStmt);
            return false;
        }
        char szStmt[1024];
        snprintf(szStmt,sizeof szStmt,"%p",a_sqlHStmt);
        IT_STMT it = m_mapStmt.find(szStmt);
        if(it != m_mapStmt.end())
        {
            _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p:%-20p][%s]",m_ulUUID,*it->second,it->second,szStmt);
            m_sqlReturn = SQLFreeHandle(SQL_HANDLE_STMT,*it->second);
            if(SQL_SUCCESS == m_sqlReturn)
            {
                *it->second = SQL_NULL_HSTMT;
                _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] [%s]",m_ulUUID,*it->second,it->second,szStmt);
                m_mapStmt.erase(it);
                _THD_LOG(NDF_LOG_T_DEBUG,"[%lu] mapsize[%ld]",m_ulUUID,m_mapStmt.size());
                return true;
            }
            else
            {
                GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,m_sqlReturn,__func__);
                _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] [%s]"
                        ,m_ulUUID
                        ,a_sqlHStmt
                        ,&a_sqlHStmt
                        ,m_szErrMsg);
                return false;
            }
        }
        else
        {
            m_sqlReturn = SQLFreeHandle(SQL_HANDLE_STMT,a_sqlHStmt);
            if(SQL_SUCCESS == m_sqlReturn)
            {
                _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p:%-20p] [%s] m_mapStmt.find is none succ",m_ulUUID,a_sqlHStmt,&a_sqlHStmt,szStmt);
                return true;
            }
            else
            {
                GetDiagRec(SQL_HANDLE_STMT,a_sqlHStmt,m_sqlReturn,__func__);
                _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] [%s ]m_mapStmt.find is none fail[%s]",m_ulUUID,a_sqlHStmt,&a_sqlHStmt,szStmt,m_szErrMsg);
                return false;
            }
        }
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
     */
    bool CPDBConn::GetDiagRec(
            SQLSMALLINT handleType,
            SQLHANDLE  handle,
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
            snprintf(m_szErrMsg,sizeof(m_szErrMsg),
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
            recCount = 1;
        }

        SQLSMALLINT  recIdx;
        int nOffset =  0;

        for(recIdx = 0 ; recIdx < recCount;++recIdx)
        {
            sqlReturn = SQLGetDiagRec(handleType,
                    handle,
                    recIdx+1,
                    m_SQLState,
                    &m_NativeErrorPtr,
                    m_MessageText,
                    PDBMAXBUFLEN-1,
                    &m_TextLengthPtr);

            if(a_pszCatch != nullptr)
            {
                nOffset += snprintf(m_szErrMsg+nOffset,sizeof(m_szErrMsg)- nOffset,
                        "[%s][%lu] %d/%s/%s",a_pszCatch,m_ulUUID,a_sqlReturn,m_SQLState,m_MessageText);

            }
            else
            {
                nOffset += snprintf(m_szErrMsg+nOffset,sizeof(m_szErrMsg)- nOffset,
                        "[%lu] %d/%s/%s",m_ulUUID,a_sqlReturn,m_SQLState,m_MessageText);
            }
        }

        return bRtn;
    }
}
