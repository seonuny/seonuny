
#include "CPreparedSQL.hpp"
#include "PDBOdbcErr.hpp"

namespace PDB {

    CPreparedSQL::CPreparedSQL()
    {

        sret_ = SQL_SUCCESS;
        className_ = typeid(*this).name();
    }

    CPreparedSQL::~CPreparedSQL() 
    {
        clear(__func__);
    }

    void CPreparedSQL::clear(const char * a_pszFunc) 
    {
        _THD_LOG(NDF_LOG_T_INFO,"called by [%s:%s]",GetClassName(),a_pszFunc);
        SQLRETURN sqlReturn;
        for(stStmtInfo &info : vecStmtInfo_)
        {
            if(info.hstmt != nullptr)
            {
                if(*info.hstmt != SQL_NULL_HSTMT)
                {
                    sqlReturn = SQLFreeHandle(SQL_HANDLE_STMT,*info.hstmt);
                    if(SQL_SUCCESS == sqlReturn )
                    {
                        _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] SQLFreeHandle succ [%s]"
                                ,info.uuidForConnection
                                ,*info.hstmt
                                ,info.hstmt
                                ,a_pszFunc);
                        *info.hstmt = SQL_NULL_HSTMT;
                    }
                    else
                    {
                        _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] SQLFreeHandle fail[%d:%s] [%s]"
                                ,info.uuidForConnection
                                ,*info.hstmt
                                ,info.hstmt
                                ,sqlReturn,CODBCErr::GetStringSTMT(*info.hstmt).c_str(),a_pszFunc);

                    }
                }
                else
                {
                    _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p:%-20p] SQL_NULL_HSTMT [%s]"
                            ,info.uuidForConnection
                            ,*info.hstmt
                            ,info.hstmt
                            ,a_pszFunc);
                    delete info.hstmt;
                }
            }
            else
            {
                _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p] nullptr [%s]"
                        ,info.uuidForConnection
                        ,info.hstmt
                        ,a_pszFunc);
            }
        }
        vecStmtInfo_.clear();
    }

    void CPreparedSQL::Lock()
    {
        m_mutex.lock();
    }
    void CPreparedSQL::UnLock()
    {
        m_mutex.unlock();
    }

    bool CPreparedSQL::SetSQL(const char * _sql, size_t _sqlLength) 
    {

        if(_sql == nullptr || _sqlLength == 0)
            return false;

        clear(__func__);

        sql_ = std::string(_sql, _sqlLength);

        return true;
    }

    void CPreparedSQL::AllPrint() 
    {
        int n =0;
        for(const stStmtInfo & rStInfo : vecStmtInfo_) 
        {
            if(rStInfo.hstmt != SQL_NULL_HSTMT) 
                _THD_LOG(NDF_LOG_T_INFO, 
                        "[%lu][%-20p:%-20p] [%s] conn [%d]----"
                        ,rStInfo.uuidForConnection
                        ,*rStInfo.hstmt
                        ,rStInfo.hstmt
                        ,GetClassName()
                        ,n
                        );
            ++n;
        } 
    }

    void CPreparedSQL::SetBindParameter(STBindParameter * _arrP, size_t _cnt) 
    {

        paramInfo_.ptr = _arrP;
        paramInfo_.cnt = _cnt;

        for(stStmtInfo &info : vecStmtInfo_)
        {
            if(*info.hstmt != SQL_NULL_HSTMT)
            {
                bindParameter(*info.hstmt);
            }
        }
    }

    void CPreparedSQL::SetBindColumn(STBindColumn * _arrP, size_t _cnt) 
    {

        colInfo_.ptr = _arrP;
        colInfo_.cnt = _cnt;

        for(stStmtInfo &info : vecStmtInfo_)
        {
            if(*info.hstmt != SQL_NULL_HSTMT)
            {
                bindColumn(*info.hstmt);
            }
        }
    }

    const char * CPreparedSQL::GetSQL() 
    {
        return sql_.c_str();
    }

    size_t CPreparedSQL::GetSQLSize() 
    {
        return sql_.size();
    }

    bool CPreparedSQL::Prepared(SQLHSTMT &hstmt)
    {
        unsigned long ulUUID=ULONG_MAX;
        for(const stStmtInfo &stInfo: vecStmtInfo_)
        {
            if(hstmt == *stInfo.hstmt)
            {
                ulUUID = stInfo.uuidForConnection;
            }
        }
        bool bRtn = true;
        sret_ = SQLPrepare(hstmt, (SQLCHAR *)sql_.c_str(), sql_.size());
        if(SQL_SUCCESS == sret_)
        {
            //_THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] SQLPrepare succ[%lu:%s]"
            //    ,ulUUID,hstmt,&hstmt,sql_.size(),sql_.data());
        }
        else
        {
            _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] SQLPrepare error[%lu:%s][%s]"
                    ,ulUUID,hstmt,&hstmt,sql_.size(),sql_.data(),CODBCErr::GetStringSTMT(hstmt).c_str());
        }

        bool bparam = false;
        bool bcol = false; 
        if(paramInfo_.cnt > 0)
        {
            bparam = bindParameter(hstmt);
        }

        if(colInfo_.cnt > 0)
        {
            bcol = bindColumn(hstmt);
        }

        _THD_LOG(NDF_LOG_T_INFO, "[%lu][%-20p:%-20p] [%s] succ [%s:%s] "
                ,ulUUID
                ,hstmt
                ,&hstmt
                ,GetClassName()
                ,paramInfo_.cnt>0?(bparam?"true":"false"):"N/A"
                ,colInfo_.cnt>0?(bcol?"true":"false"):"N/A"
                );

        return bRtn;
    }

    SQLHSTMT &  CPreparedSQL::AllocStatement(CPDBConn* a_plsPDBConn)
    {
        try
        {
            unsigned long ulUUID = a_plsPDBConn->GetUuid();
            for(stStmtInfo &stInfo: vecStmtInfo_)
            {
                if(ulUUID == stInfo.uuidForConnection)
                {
                    if(*stInfo.hstmt == SQL_NULL_HSTMT)
                    {
                        THROW(E_ERROR_LOG,"hstmt is null");
                    }
                    else
                    {
                        _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p:%-20p] [%s] get Statement succ"
                                ,stInfo.uuidForConnection
                                ,*stInfo.hstmt
                                ,&stInfo.hstmt
                                ,GetClassName()
                                );
                    }
                    return *stInfo.hstmt;
                }
            }



            stStmtInfo stInfo ;
            stInfo.uuidForConnection = ulUUID;
            stInfo.hstmt = new SQLHSTMT;
           *stInfo.hstmt = SQL_NULL_HSTMT;

            _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p:%-20p] [%s] new Statement "
                    ,stInfo.uuidForConnection
                    ,*stInfo.hstmt
                    ,stInfo.hstmt
                    ,GetClassName()
                    );
            vecStmtInfo_.emplace_back(stInfo);

            for(stStmtInfo &stInfo: vecStmtInfo_)
            {
                if(ulUUID == stInfo.uuidForConnection)
                {
                    if(*stInfo.hstmt == SQL_NULL_HSTMT)
                    {
                        if(a_plsPDBConn->AllocStmt(*stInfo.hstmt) != true)
                        {
                            THROW(E_ERROR_LOG,"AllocStmt fail");
                        }
                        if(Prepared(*stInfo.hstmt) != true)
                        {
                            _THD_LOG(NDF_LOG_T_ERROR,"[%lu][%-20p:%-20p] [%s] fail"
                                    ,stInfo.uuidForConnection
                                    ,*stInfo.hstmt
                                    ,stInfo.hstmt
                                    ,GetClassName()
                                    ) ;
                        }
                    }
                    else
                    {
                        _THD_LOG(NDF_LOG_T_INFO,"[%lu][%-20p:%-20p] [%s] get Statement succ"
                                ,stInfo.uuidForConnection
                                ,*stInfo.hstmt
                                ,stInfo.hstmt
                                ,GetClassName()
                                );
                    }
                    return *stInfo.hstmt;
                }
            }
        }
        catch(CPCFException &e)
        {
            if ((e.m_nECode & E_ERROR_LOG) == E_ERROR_LOG)
            {
                _THD_LOG(NDF_LOG_T_ERROR, "[%s]", e.what());
            }
        }
        catch(std::exception &e)
        {
            _THD_LOG(NDF_LOG_T_ERROR, "[%s][%s]",typeid(e).name(), e.what());
        }
                
        m_sqlHStmt = SQL_NULL_HSTMT;
        return m_sqlHStmt;
    }


    void CPreparedSQL::CloseStatement(SQLHSTMT & _stmt) 
    {
        if(_stmt != SQL_NULL_HSTMT) 
        {
            bool bFind = false;
            for(stStmtInfo & info : vecStmtInfo_) 
            {
                if(*info.hstmt == _stmt) 
                {
                    SQLFreeStmt(_stmt, SQL_CLOSE);
                    _THD_LOG(NDF_LOG_T_DEBUG,"[%lu][%-20p:%-20p] [%s] info.hstmt.SQLFreeStmt"
                            ,info.uuidForConnection
                            ,_stmt
                            ,&_stmt
                            ,GetClassName()
                            );
                    bFind = true;
                }
            }
            if(bFind == false)
            {
                SQLFreeStmt(_stmt, SQL_CLOSE);
                _THD_LOG(NDF_LOG_T_DEBUG,"[%-20p:%-20p] [%s]_stmt.SQLFreeStmt",_stmt,&_stmt,GetClassName());
            }
        }
    }


    bool CPreparedSQL::BindParameter(SQLHSTMT & _hstmt,
            STBindParameter * _arrP,
            size_t _cnt) 
    {

        paramInfo_.ptr = _arrP;
        paramInfo_.cnt = _cnt;

        return bindParameter(_hstmt);
    }

    bool CPreparedSQL::BindColumn(SQLHSTMT & _hstmt,
            STBindColumn * _arrP,
            size_t _cnt) 
    {

        colInfo_.ptr = _arrP;
        colInfo_.cnt = _cnt;

        return bindColumn(_hstmt);
    }

    bool CPreparedSQL::bindParameter(SQLHSTMT & _stmt) 
    {

        STBindParameter * arr = paramInfo_.ptr;
        size_t  arrCnt = paramInfo_.cnt;

        unsigned long ulUUID=ULONG_MAX;
        for(const stStmtInfo &stInfo: vecStmtInfo_)
        {
            if(_stmt == *stInfo.hstmt)
            {
                ulUUID = stInfo.uuidForConnection;
            }
        }
        if(arr == nullptr || arrCnt == 0) 
        {
            _THD_LOG(NDF_LOG_T_ERROR, "[%lu][%-20p:%-20p] [%s] invalid arguemnt [%s] [%-20p:%zu]"
                    ,ulUUID
                    ,_stmt
                    ,&_stmt
                    ,GetClassName()
                    ,GetSQL(), arr, arrCnt);
            return false;
        }

      //SQLFreeStmt(_stmt, SQL_RESET_PARAMS);

        for(size_t i=0; i < arrCnt; ++i) 
        {
            sret_ = SQLBindParameter(_stmt,
                    arr[i].ipar,
                    SQL_PARAM_INPUT,
                    arr[i].fCType,
                    arr[i].fSQLType,
                    arr[i].pLen,
                    arr[i].pScale,
                    arr[i].pData,
                    arr[i].cbValueMax,
                    arr[i].cbValueLen);

            if(sret_ == SQL_SUCCESS ) 
            {
                //_THD_LOG(NDF_LOG_T_DEBUG,"[%lu:%-20p] SQLBindParameter succ[%u][%-20p]"
                //             ,ulUUID,_stmt
                //             ,arr[i].ipar ,arr[i].pData);
            }
            else
            {
                if(sret_ == SQL_SUCCESS_WITH_INFO)
                {
                    _THD_LOG(NDF_LOG_T_INFO, "[%lu][%-20p:%-20p] SQLBindParameter succ_with_info [%s] [%d] [%s]"
                            ,ulUUID
                            ,_stmt
                            ,&_stmt
                            ,GetSQL()
                            ,sret_
                            ,CODBCErr::GetStringSTMT(_stmt).c_str());
                }
                else
                {
                    _THD_LOG(NDF_LOG_T_ERROR, "[%lu][%-20p:%-20p] SQLBindParameter fail [%s] [%d] [%s]"
                            ,ulUUID
                            ,_stmt
                            ,&_stmt
                            ,GetSQL()
                            ,sret_
                            ,CODBCErr::GetStringSTMT(_stmt).c_str());

                  //SQLFreeStmt(_stmt, SQL_RESET_PARAMS);
                    return false;
                }
            }
        }

        _THD_LOG(NDF_LOG_T_DEBUG, "[%lu][%-20p:%-20p] [%s] succ "
                ,ulUUID
                ,_stmt
                ,&_stmt
                ,GetClassName()
                );
        return true;

    }

    bool CPreparedSQL::bindColumn(SQLHSTMT & _stmt) 
    {
        STBindColumn * arr = colInfo_.ptr;
        size_t arrCnt = colInfo_.cnt;

        unsigned long ulUUID=ULONG_MAX;
        for(const stStmtInfo &stInfo: vecStmtInfo_)
        {
            if(_stmt == *stInfo.hstmt)
            {
                ulUUID = stInfo.uuidForConnection;
            }
        }
        if(arr == nullptr || arrCnt == 0) 
        {
            _THD_LOG(NDF_LOG_T_ERROR, "[%lu][%-20p:%-20p] [%s] invalid arguemnt [%s] [%-20p:%zu]"
                    ,ulUUID
                    ,_stmt
                    ,&_stmt
                    ,GetClassName()
                    ,GetSQL(), arr, arrCnt);
            return false;
        }

      //SQLFreeStmt(_stmt, SQL_UNBIND);

        for(size_t i=0; i < arrCnt; ++i) 
        {
            sret_ = SQLBindCol(_stmt,
                    arr[i].ipar,
                    arr[i].fCType,
                    arr[i].pData,
                    arr[i].cbValueMax,
                    arr[i].pInd);

            if(sret_ == SQL_SUCCESS ) 
            {
                //_THD_LOG(NDF_LOG_T_DEBUG,"[%lu:%-20p] SQLBindCol succ[%u] [%-20p]"
                //             ,ulUUID,_stmt
                //             ,arr[i].ipar ,arr[i].pData);
            }
            else
            {
                if(sret_ == SQL_SUCCESS_WITH_INFO)
                {
                    _THD_LOG(NDF_LOG_T_INFO, "[%lu][%-20p:%-20p] SQLBindCol succ_with_info[%s] [%d] [%s]"
                            ,ulUUID
                            ,_stmt
                            ,&_stmt
                            ,GetSQL()
                            ,sret_
                            ,CODBCErr::GetStringSTMT(_stmt).c_str());
                }
                else
                {
                    _THD_LOG(NDF_LOG_T_ERROR, "[%lu][%-20p:%-20p] SQLBindCol fail[%s] [%d] [%s]"
                            ,ulUUID
                            ,_stmt
                            ,&_stmt
                            ,GetSQL()
                            ,sret_
                            ,CODBCErr::GetStringSTMT(_stmt).c_str());
                  //SQLFreeStmt(_stmt, SQL_UNBIND);
                    return false;
                }
            }
        }

        _THD_LOG(NDF_LOG_T_DEBUG, "[%lu][%-20p:%-20p] [%s] succ"
                , ulUUID
                , _stmt
                ,&_stmt
                ,GetClassName()
                );

        return true;
    }

}
