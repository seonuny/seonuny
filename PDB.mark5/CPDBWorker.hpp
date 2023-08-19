#pragma once

#include <string>
#include <set>

#include "define.hpp"

#include "CPreparedSQL.hpp"

#include "CPDBConnMgr.hpp"

namespace PDB {
    class CPDBWorker
    {
        private:
            CPDBConnMgr              *m_plsPDBConnMgr;

        public:
            CPDBWorker();
            ~CPDBWorker();
            bool Initialize();
            bool Assign(CPDBConnMgr *a_plsPDBConnMgr);
            bool TurnOn();
            bool TurnOff();
            bool IsConnection() { return (m_plsPDBConnMgr==nullptr)?false:(m_plsPDBConnMgr->GetCurConnCnt()==0)?false:true; }
            bool Execute(CPreparedSQL&);
            bool DirectExecute(const char* a_szSql,size_t a_uSqlSize=0);

        private:
    };
}
