#pragma once

#include <mutex>
#include <set>
#include <condition_variable>
#include <vector>
#include <list>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "CGBFSingleton.hpp" 

#include "define.hpp"

#include "CPDBConn.hpp"


namespace PDB 
{
    class CPDBConnMgr
    {
        private:
            atomic<unsigned long>  m_ulConnID;
            std::thread            m_thread;
            std::mutex             m_mutex;
            std::condition_variable m_condV;
            int                    m_nMaxConnCnt;
            int                    m_nCurConnCnt;
            int                    m_nConnPercent;

            int                    m_nRetryInterval;
            int                    m_nConnMonitoringPeriod;

            bool                   m_bThreadRun;
            bool                   m_bAutoCommit;

            char                   m_szErrMsg[1024];

            std::string            m_stringODBC;
            std::string            m_stringDRODBC;
            std::set<std::string>  m_setRetryCode;
            std::set<std::string>  m_setReconnCode;

            std::list<CPDBConn*>   m_listPool;
            std::list<CPDBConn*>   m_listUsed;
        public:
            CPDBConnMgr(void);
            ~CPDBConnMgr(void);
            bool SetConfig(char *a_pszCfgFile);
            bool Connect(void);
            bool Disconnect(void);
            CPDBConn* GetConn(void);
            void ReleaseConn(CPDBConn* a_pConn);
            bool IsAutoCommit() { return m_bAutoCommit; }
            bool ChkRetryCode(char *a_pszErrorCode);
            bool ChkReconnCode(char *a_pszErrorCode);
            int  GetMaxConnCnt() { return m_nMaxConnCnt; }
            int  GetCurConnCnt() { return m_nCurConnCnt; }
            void ThreadStart(void);
            void ThreadStop(void);
            bool IsActive(void) { return m_bThreadRun; }
            bool ChkConnectionCnt(void);
            bool Run(void);
    };
}
#define PDBConnMgrSTPtr()    CGBFSingleton<PDB::CPDBConnMgr>::GetInstancePtr() 

