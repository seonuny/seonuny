#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "CPDBConnMgr.hpp"

namespace PDB 
{
    /**
     * \brief         Construction 
     * \detail         
     * \param[in]     type:void 
     * \return        type:void 
     *
     * */
    CPDBConnMgr::CPDBConnMgr()
    {
        m_ulConnID    = 0;
        m_nMaxConnCnt = 0;
        m_nCurConnCnt = 0;
        m_nCurConnCnt = 0;

        m_nRetryInterval = 5;
        m_nConnMonitoringPeriod = 5;

        m_nConnPercent = 100;
        m_bAutoCommit  = true;
        m_bThreadRun   = false;
        _THD_LOG(NDF_LOG_T_INFO,"Construction");
    }

    /**
     * \brief         Destruction 
     * \detail         
     * \param[in]     type:void
     * \return        type:void 
     *
     * */
    CPDBConnMgr::~CPDBConnMgr()
    {
        _THD_LOG(NDF_LOG_T_INFO,"Destruction");
        unique_lock<std::mutex> ul(m_mutex);
        if(m_listUsed.size()>0)
        {
            for(CPDBConn *conn : m_listUsed)
            {
                m_listPool.emplace_front(conn);
            }
            m_listUsed.clear();
        }
        if(m_listPool.size()>0)
        {
            for(CPDBConn *conn : m_listPool)
            {
                if(m_bAutoCommit==false)
                {
                    conn->Commit();
                }

                SQLRETURN sqlReturn = conn->Disconnect();
                if(sqlReturn != SQL_SUCCESS)
                {
                    _THD_LOG(NDF_LOG_T_WARNING,"[%d:%s]",sqlReturn,conn->GetErrMsg());
                }
                delete conn;
            }
            m_listPool.clear();
        }

        m_setRetryCode.clear();
        m_setReconnCode.clear();

        m_nCurConnCnt = 0;
        m_nMaxConnCnt = 0;
    }

    /**
     * \brief         CPDBConn  생성하여 Connect를 호출 한다.
     * \detail        최돼연결개수와 현재연결, 연결 퍼센트를 확인한다 연결 한다. 
     * \param[in]     type:void 
     * \return        type:bool    true/false 
     *
     * */
    bool CPDBConnMgr::Connect()
    {
        bool bRtn = true;
        unique_lock<std::mutex> ul(m_mutex);
        m_nMaxConnCnt = m_nMaxConnCnt + 1;
        int nConnPercent = ((float)m_nCurConnCnt/(float)m_nMaxConnCnt)*100 ;
        SQLRETURN sqlReturn;
        if(nConnPercent < m_nConnPercent)
        {
            m_ulConnID.fetch_add(1);
            _THD_LOG(NDF_LOG_T_DEBUG,"CONN_ID[%lu]",m_ulConnID.load());
            CPDBConn *plsPDBConn = new  CPDBConn(ULONG_MAX-m_ulConnID.load());
            plsPDBConn->SetConfig(m_stringODBC);
            plsPDBConn->SetAutoCommit(m_bAutoCommit);
            sqlReturn = plsPDBConn->PreConnect();
            if(sqlReturn != SQL_SUCCESS)
            {
                _THD_LOG(NDF_LOG_T_WARNING,"[%d:%s]",sqlReturn,plsPDBConn->GetErrMsg());
                delete plsPDBConn;
                return false;
            }
            sqlReturn = plsPDBConn->Connect();
            if(sqlReturn != SQL_SUCCESS)
            {
                _THD_LOG(NDF_LOG_T_WARNING,"[%d:%s]",sqlReturn,plsPDBConn->GetErrMsg());
                delete plsPDBConn;
                return false;
            }
            m_listPool.emplace_back(plsPDBConn);
            m_nCurConnCnt = m_nCurConnCnt+1;
        }

        return bRtn;
    }

    /**
     * \brief         CPDBConn의 Disconnect를 호출 한다. 
     * \detail        최대 연결개수와 현재연결 , 연결 퍼센트를 확인하여 연결 해제한다.
     * \param[in]     type:void 
     * \return        type:bool   true/false 
     *
     * */
    bool CPDBConnMgr::Disconnect()
    {
        bool bRtn =true;
        unique_lock<std::mutex> ul(m_mutex);
        _THD_LOG(NDF_LOG_T_DEBUG,"Disconnect");
        if(m_nCurConnCnt>=m_nMaxConnCnt)
        {
            for(CPDBConn *plsPDBConn: m_listPool)
            {
                plsPDBConn->ChkStmt(__LINE__);
            }

            if(m_listPool.size() > 0)
            {
                CPDBConn *plsPDBConn = m_listPool.back();
                m_listPool.pop_back();
                if(m_bAutoCommit==false)
                {
                    plsPDBConn->Commit();
                }

                SQLRETURN sqlReturn = plsPDBConn->Disconnect();
                if(sqlReturn != SQL_SUCCESS)
                {
                    _THD_LOG(NDF_LOG_T_WARNING,"[%d:%s]",sqlReturn,plsPDBConn->GetErrMsg());
                    bRtn = false;
                    delete plsPDBConn;
                    return bRtn;
                }
                sqlReturn = plsPDBConn->Postdisconnect();
                if(sqlReturn != SQL_SUCCESS)
                {
                    _THD_LOG(NDF_LOG_T_WARNING,"[%d:%s]",sqlReturn,plsPDBConn->GetErrMsg());
                    bRtn = false;
                    delete plsPDBConn;
                    return bRtn;
                }
                delete plsPDBConn;
                m_nMaxConnCnt = m_nMaxConnCnt-1;
                m_nCurConnCnt = m_nCurConnCnt-1;
                _THD_LOG(NDF_LOG_T_DEBUG,"connection[%d:%d]",m_nCurConnCnt,m_nMaxConnCnt);
            }
            else
            {
                _THD_LOG(NDF_LOG_T_WARNING,">>pool empty>>");
            }
        }
        else
        {
            _THD_LOG(NDF_LOG_T_INFO,"ignore[%ld]",m_listPool.size());
            m_nMaxConnCnt = m_nMaxConnCnt-1;
        }

        return bRtn;
    }

    /**
     * \brief         pool에셔 미사용인 CPDBConn를 돌려준다. 
     * \detail        모두 사용중이면 wait_for를 이용하여 지정된 시간 및 CPDBConn이 pool에 입력 되면 깨어난다. 
     * \param[in]     type:void 
     * \return        type:CPDBConn*      CPDBConn pointer 
     *
     * */
    CPDBConn* CPDBConnMgr::GetConn()
    {
        unique_lock<std::mutex> ul(m_mutex);
        if (m_listPool.size() > 0)
        {
            CPDBConn *plsPDBConn = m_listPool.front();
            m_listUsed.emplace_back(plsPDBConn);
            m_listPool.pop_front();
            return plsPDBConn;
        }
        m_condV.wait_for(ul, std::chrono::milliseconds(1000), [this]() throw()->bool { return !this->m_listPool.empty(); });
        if (m_listPool.size() > 0)
        {
            CPDBConn *plsPDBConn = m_listPool.front();
            m_listUsed.emplace_back(plsPDBConn);
            m_listPool.pop_front();
            return plsPDBConn;
        }
        return nullptr;
    }

    /**
     * \brief         사용이 완료된 CPDBConn*를 pool에 저장한다.
     * \detail        pool이 비어서 wait_for에 대기중인 thread를 깨우기 위해 notify_all을 수행한다. 
     * \param[in]     type:CPDBConnMgr* CPDBConn pointer 
     * \return        type:void 
     *
     * */
    void CPDBConnMgr::ReleaseConn(CPDBConn *pConn)
    {
        unique_lock<std::mutex> ul(m_mutex,std::defer_lock);
        ul.lock();
        m_listPool.emplace_front(pConn);
        m_listUsed.remove(pConn);
        ul.unlock();
      //m_condV.notify_one();
        m_condV.notify_all();
    }

    /**
     * \brief         오류코드 수신 시 해당 코드가 retry할 코드인지 알져준다. 
     * \detail         
     * \param[in]     type:char*    error code 
     * \return        type:bool     true/false 
     *
     * */
    bool CPDBConnMgr::ChkRetryCode(char *a_pszErrorCode)
    {
        bool bRtn = false;
        std::set<std::string>::iterator it = m_setRetryCode.find(a_pszErrorCode);
        if(it != m_setRetryCode.end())
        {
            bRtn =true;
            return bRtn;
        }
        return bRtn;
    }

    /**
     * \brief         오류코드 수신 시 해당 코드가 reconnect할 코드인지 알져준다. 
     * \detail         
     * \param[in]     type:char*    error code 
     * \return        type:bool     true/false 
     *
     * */
    bool CPDBConnMgr::ChkReconnCode(char *a_pszErrorCode)
    {
        bool bRtn = false;
        std::set<std::string>::iterator it = m_setReconnCode.find(a_pszErrorCode);
        if(it != m_setReconnCode.end())
        {
            bRtn =true;
            return bRtn;
        }
        return bRtn;
    }

    /**
     * \brief         주어진 config 파일명을 이용하여 pdb config를 읽어 들인다. 
     * \detail        config을 json type이다.
     * \param[in]     type:char*    config file name  
     * \return        type:bool     true/false 
     *
     * */
    bool CPDBConnMgr::SetConfig(char *a_pszCfgFile)
    {
        unique_lock<std::mutex> ul(m_mutex);
        bool bRtn = true;
        FILE *pfFile = fopen(a_pszCfgFile,"r");

        if (pfFile == nullptr)
        {
            snprintf(m_szErrMsg,sizeof m_szErrMsg,"fopen:%d:%s",errno,strerror(errno));
            return false;
        }

        struct stat stFStat;
        int nfd = fileno(pfFile);
        if(nfd < 0 )
        {
            snprintf(m_szErrMsg,sizeof m_szErrMsg,"fileno:%d:%s",errno,strerror(errno));
            return false;
        }

        if (fstat(nfd,&stFStat) != 0)
        {
            snprintf(m_szErrMsg,sizeof m_szErrMsg,"fstat:%d:%s",errno,strerror(errno));
            return false;
        }

        char *pszJsonStr = new char[stFStat.st_size+1];

        if ( fread(pszJsonStr,stFStat.st_size,1,pfFile) != 1)
        {
            snprintf(m_szErrMsg,sizeof m_szErrMsg,"fread:%d:%s",errno,strerror(errno));
            return false;
        }
        fclose(pfFile);
        rapidjson::Document doc;

        if(doc.Parse(pszJsonStr).HasParseError())
        {
            snprintf(m_szErrMsg,sizeof m_szErrMsg,"rapidjson.Parse:%d:%s",errno,strerror(errno));
            delete []pszJsonStr;
            return false;
        }

        delete []pszJsonStr;
        for(const rapidjson::Value::Member &kv : doc.GetObject())
        {
            if(strcmp("odbc-config",kv.name.GetString()) == 0)
            {
                if(kv.value.IsObject() == true)
                {
                    m_stringODBC.clear();
                    bool bFirst = true;
                    for(const rapidjson::Value::Member &kvSub : kv.value.GetObject())
                    {
                        if( bFirst == true)
                        {
                            bFirst = false;
                        }
                        else
                        {
                            m_stringODBC.append(";");
                        }
                        m_stringODBC.append(kvSub.name.GetString());
                        m_stringODBC.append("=");
                        m_stringODBC.append(kvSub.value.GetString());
                    }
                }
                else
                {
                    snprintf(m_szErrMsg,sizeof m_szErrMsg,"Exception of type [%s:%d]"
                            ,kv.name.GetString()
                            ,kv.value.GetType());
                    return false;
                }
            }
            if(strcmp("dr-odbc-config",kv.name.GetString()) == 0)
            {
                if(kv.value.IsObject() == true)
                {
                    m_stringDRODBC.clear();
                    bool bFirst = true;
                    for(const rapidjson::Value::Member &kvSub : kv.value.GetObject())
                    {
                        if( bFirst == true)
                        {
                            bFirst = false;
                        }
                        else
                        {
                            m_stringDRODBC.append(";");
                        }
                        m_stringDRODBC.append(kvSub.name.GetString());
                        m_stringDRODBC.append("=");
                        m_stringDRODBC.append(kvSub.value.GetString());
                    }
                }
                else
                {
                    snprintf(m_szErrMsg,sizeof m_szErrMsg,"Exception of type [%s:%d]"
                            ,kv.name.GetString()
                            ,kv.value.GetType());
                    return false;
                }
            }
            else if(strcmp("pdblib-config",kv.name.GetString()) == 0)
            {
                if(kv.value.IsObject() == true)
                {
                    for(const rapidjson::Value::Member &kvSub : kv.value.GetObject())
                    {
                        if(strcmp(kvSub.name.GetString(),"conn-percent") == 0)
                        {
                            if(kvSub.value.IsInt() == true)
                            {
                                m_nConnPercent = kvSub.value.GetInt();
                                _THD_LOG(NDF_LOG_T_INFO,"[%s:%d]"
                                        ,kvSub.name.GetString() 
                                        ,m_nConnPercent);
                            }
                            else
                            {
                                snprintf(m_szErrMsg,sizeof m_szErrMsg,"Exception of type [%s:%d]"
                                        ,kvSub.name.GetString()
                                        ,kvSub.value.GetType());
                            }
                        }
                        else if(strcmp(kvSub.name.GetString(),"auto-commit") == 0)
                        {
                            if(kvSub.value.IsBool() == true)
                            {
                                m_bAutoCommit = kvSub.value.GetBool();
                                _THD_LOG(NDF_LOG_T_INFO,"[%s:%s]"
                                        ,kvSub.name.GetString() 
                                        ,m_bAutoCommit?"true":"false");
                            }
                            else
                            {
                                snprintf(m_szErrMsg,sizeof m_szErrMsg,"Exception of type [%s:%d]"
                                        ,kvSub.name.GetString()
                                        ,kvSub.value.GetType());
                            }
                        }
                        else if(strcmp(kvSub.name.GetString(),"retry-error-code") == 0)
                        {
                            if(kvSub.value.IsArray() == true)
                            {
                                for(const rapidjson::Value &v : kvSub.value.GetArray())
                                {
                                    m_setRetryCode.emplace(v.GetString());
                                }
                            }
                        }
                        else if(strcmp(kvSub.name.GetString(),"reconnect-error-code") == 0)
                        {
                            if(kvSub.value.IsArray() == true)
                            {
                                for(const rapidjson::Value &v : kvSub.value.GetArray())
                                {
                                    m_setReconnCode.emplace(v.GetString());
                                }
                            }
                        }
                        else if(strcmp(kvSub.name.GetString(),"retry-interval") == 0)
                        {
                            m_nRetryInterval = kvSub.value.GetInt();
                        }
                        else if(strcmp(kvSub.name.GetString(),"conn-monitoring-period") == 0)
                        {
                            m_nConnMonitoringPeriod = kvSub.value.GetInt();
                        }
                    }
                }
                else
                {
                    snprintf(m_szErrMsg,sizeof m_szErrMsg,"Exception of type [%s:%d]"
                            ,kv.name.GetString()
                            ,kv.value.GetType());
                    return false;
                }
            }
        }

        return bRtn;
    }


    /**
     * \brief         Thread start 시킨다 
     * \detail         
     * \param[in]     type:void 
     * \return        type:void
     *
     * */
    void CPDBConnMgr::ThreadStart(void)
    {
        unique_lock<std::mutex> ul(m_mutex);
        if(m_bThreadRun == false)
        {
            m_bThreadRun = true;
            std::thread thd = std::thread(&CPDBConnMgr::Run,this);
            thd.detach();
        }
    }

    /**
     * \brief         Thread stop 시킨다 
     * \detail         
     * \param[in]     type:void 
     * \return        type:void
     *
     * */
    void CPDBConnMgr::ThreadStop(void)
    {
        unique_lock<std::mutex> ul(m_mutex);
        if(m_bThreadRun == true)
        {
            if(m_thread.joinable())
            {
                m_thread.join();
            }
            m_bThreadRun = false;
        }
    }

    /**
     * \brief         pool을 감시하면서 disconnect된 CPDBConn의 connection을  호출한다. 
     * \detail         
     * \param[in]     type:void 
     * \return        type:bool    true/false
     *
     * */
    bool CPDBConnMgr::ChkConnectionCnt(void)
    {
        bool bRtn = true;
        unique_lock<std::mutex> ul(m_mutex);
        SQLRETURN sqlReturn;
        for(CPDBConn* conn: m_listPool)
        {
            if(conn->IsConnection() == false)
            {
                sqlReturn = conn->PreConnect();
                if(sqlReturn != SQL_SUCCESS)
                {
                    _THD_LOG(NDF_LOG_T_WARNING,"[%d:%s]",sqlReturn,conn->GetErrMsg());
                    continue;
                }
                sqlReturn = conn->Connect();
                if(sqlReturn != SQL_SUCCESS)
                {
                    _THD_LOG(NDF_LOG_T_WARNING,"[%d:%s]",sqlReturn,conn->GetErrMsg());
                }
            }
        }
        return bRtn;
    }

    /**
     * \brief         CPDBConnMgr의 Thread 함수. 
     * \detail         
     * \param[in]     type:void 
     * \return        type:bool  true/false 
     *
     * */
    bool CPDBConnMgr::Run(void)
    {
        bool bRtn = true;
        while(m_bThreadRun)
        {
            ChkConnectionCnt();
            sleep(5);
        }
        return bRtn;
    }

}
