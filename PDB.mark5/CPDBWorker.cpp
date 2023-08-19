#include "CPDBWorker.hpp"

namespace PDB 
{
    /**
     * \brief         Construction 
     * \detail         
     * \param[in]     type:void 
     * \return        type:void 
     *
     * */
    CPDBWorker::CPDBWorker()
    {
        m_plsPDBConnMgr = nullptr;
    }

    /**
     * \brief         Destruction 
     * \detail         
     * \param[in]     type:void
     * \return        type:void 
     *
     * */
    CPDBWorker::~CPDBWorker()
    {
    }

    /**
     * \brief         초기화  
     * \detail         
     * \param[in]     type:void 
     * \return        type:bool 
     *
     * */
    bool CPDBWorker::Initialize()
    {
        bool bRtn  = true;

        return bRtn;
    }

    /**
     * \brief         CPDBConnMgr를 Worker에 등록 한다. 
     * \detail         
     * \param[in]     type:CPDBConnMgr*   CPDBConnMgr pointer 
     * \return        type:bool           true/false 
     *
     * */
    bool CPDBWorker::Assign(CPDBConnMgr *a_plsPDBConnMgr)
    {
        bool bRtn = true;
        if(a_plsPDBConnMgr == nullptr)
        {
            bRtn = false;
            return bRtn;
        }
        if(m_plsPDBConnMgr == nullptr)
        {
            m_plsPDBConnMgr = a_plsPDBConnMgr;
        }
        return bRtn;
    }

    /**
     * \brief         Woker에서 DB연결을 수행한다.
     * \detail         
     * \param[in]     type:void 
     * \return        type:bool     true/false 
     *
     * */
    bool CPDBWorker::TurnOn()
    {
        bool bRtn  = true;

        if(m_plsPDBConnMgr == nullptr)
        {
            bRtn = false;
            return bRtn;
        }

        _THD_LOG(NDF_LOG_T_DEBUG,"TurnOn");
        bRtn = m_plsPDBConnMgr->Connect();

        return bRtn;
    }

    /**
     * \brief         Woker에서 DB연결 해제를 수행한다.
     * \detail         
     * \param[in]     type:void 
     * \return        type:bool     true/false 
     *
     * */
    bool CPDBWorker::TurnOff()
    {
        bool bRtn  = true;

        if(m_plsPDBConnMgr == nullptr)
        {
            bRtn = false;
            return bRtn;
        }

        _THD_LOG(NDF_LOG_T_DEBUG,"TurnOff");
        bRtn =m_plsPDBConnMgr->Disconnect();

        return bRtn;
    }

    /**
     * \brief         CPreparedSQL 를 이용하여 DB 쿼리를 수행한다. 
     * \detail         
     * \param[in]     type:CPreparedSQL&     Insert/Update/Delete/Select 등의 작업이 만들어진 CPreparedSQL class 
     * \return        type:bool             true/false 
     *
     * */
    bool CPDBWorker::Execute(CPreparedSQL& a_preparedSQL)
    {
        CPDBConn *plsPDBConn = nullptr ;
        
        while(plsPDBConn == nullptr)
        {
            plsPDBConn = m_plsPDBConnMgr->GetConn();
            if(plsPDBConn == nullptr)
            {
                _THD_LOG(NDF_LOG_T_INFO,">>>>>[pdb conn nullptr]<<<<<");
                usleep(10);
            }
        }

        plsPDBConn->ChkStmt(__LINE__);
        unsigned long ulUUid = plsPDBConn->GetUuid();
        SQLHSTMT &hstmt = a_preparedSQL.AllocStatement(plsPDBConn);
        _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] [%s] AllocStatement"
                ,ulUUid,hstmt,&hstmt,a_preparedSQL.GetClassName());
        if(hstmt == SQL_NULL_HSTMT)
        {
            _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] [%s]AllocStatement fail"
                ,ulUUid,hstmt,&hstmt,a_preparedSQL.GetClassName());
            return false;
        }
        plsPDBConn->ChkStmt(__LINE__);

        if(a_preparedSQL.Execute(hstmt) == true && SQL_NO_DATA != a_preparedSQL.GetSQLReturn())
        {
            plsPDBConn->ChkStmt(__LINE__);
            _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] [%s] hstmt"
                    ,ulUUid,hstmt,&hstmt
                    ,a_preparedSQL.GetClassName()
                    );
            a_preparedSQL.CloseStatement(hstmt);
            plsPDBConn->ChkStmt(__LINE__);
            _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] [%s] hstmt"
                    ,ulUUid,hstmt,&hstmt
                    ,a_preparedSQL.GetClassName()
                    );
            m_plsPDBConnMgr->ReleaseConn(plsPDBConn);
            plsPDBConn->ChkStmt(__LINE__);
            return true;
        }

        plsPDBConn->ChkStmt(__LINE__);
        if(plsPDBConn->ReleaseStmt(hstmt) != true)
        {
            _THD_LOG(NDF_LOG_T_WARNING,"[%lu][%-20p:%-20p] [%s] ReleaseStmt fail"
                    ,ulUUid
                    ,hstmt
                    ,&hstmt
                    ,a_preparedSQL.GetClassName()
                    );
        }

        if(m_plsPDBConnMgr->ChkReconnCode((char*)plsPDBConn->GetSQLState()) == true)
        {
            _THD_LOG(NDF_LOG_T_ERROR,"[%lu] [%-20p:%-20p] [%s] reconnect "
                    ,ulUUid,hstmt,&hstmt
                    ,a_preparedSQL.GetClassName()
                    );
            plsPDBConn->Disconnect();
        }
        m_plsPDBConnMgr->ReleaseConn(plsPDBConn);
        if(m_plsPDBConnMgr->ChkRetryCode((char*)plsPDBConn->GetSQLState()) == true)
        {
            _THD_LOG(NDF_LOG_T_ERROR,"[%lu] [%-20p:%-20p] [%s] retry"
                    ,ulUUid,hstmt,&hstmt
                    ,a_preparedSQL.GetClassName()
                    );
            return Execute(a_preparedSQL);
        }
        _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] [%s] [%d:%s]"
                ,ulUUid,hstmt,&hstmt
                ,a_preparedSQL.GetClassName()
                ,a_preparedSQL.GetSQLReturn()
                ,plsPDBConn->GetSQLState()
                );


        return false;
    }

    /**
     * \brief         쿼리문을 직접 사용한다. 
     * \detail         
     * \param[in]     type:const char*     query string 
     * \param[in]     type:size_t          query string length.
     * \return        type:bool           true/false 
     *
     * */
    bool CPDBWorker::DirectExecute(const char* a_szSql,size_t a_uSqlSize)
    {
        bool bRtn  = true;
        
        CPDBConn *plsPDBConn = m_plsPDBConnMgr->GetConn();

        SQLHSTMT sqlHStmt = SQL_NULL_HSTMT;
        
        size_t uSqlSize = a_uSqlSize;
        if(a_uSqlSize == 0)
        {
            uSqlSize = strlen(a_szSql);
        }

        SQLRETURN sqlReturn;

        bRtn = plsPDBConn->AllocStmt(sqlHStmt);
        if(true != bRtn)
        {
            _THD_LOG(NDF_LOG_T_ERROR,"%s",plsPDBConn->GetErrMsg());
            return bRtn;
        }

        sqlReturn = SQLPrepare(sqlHStmt,(SQLCHAR*)a_szSql,uSqlSize);
        if(SQL_SUCCESS != sqlReturn)
        {
            bRtn = false;
            plsPDBConn->GetDiagRec(SQL_HANDLE_STMT,sqlHStmt,sqlReturn,__func__);
            _THD_LOG(NDF_LOG_T_ERROR,"%s",plsPDBConn->GetErrMsg());
            return bRtn;
        }
        sqlReturn = SQLExecute(sqlHStmt);
        if(SQL_SUCCESS != sqlReturn)
        {
            bRtn = false;
            plsPDBConn->GetDiagRec(SQL_HANDLE_STMT,sqlHStmt,sqlReturn,__func__);
            _THD_LOG(NDF_LOG_T_ERROR,"%s",plsPDBConn->GetErrMsg());
            if(m_plsPDBConnMgr->IsAutoCommit() == false)
            {
                plsPDBConn->Rollback();
            }
            return bRtn;
        }

        if(m_plsPDBConnMgr->IsAutoCommit() == false)
        {
            plsPDBConn->Commit();
        }

        m_plsPDBConnMgr->ReleaseConn(plsPDBConn);

        return bRtn;
    }
}
