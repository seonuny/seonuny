#pragma once

#include <string>
#include <vector>
#include <functional>

#include <sql.h>
#include <sqlext.h>

#include "define.hpp"

#include "CNDFODBCAdaptor.hpp"

#include "CODBCErr.hpp"

#include "CPDBConn.hpp"

namespace PDB 
{

    struct stParameterInfo 
    {
        STBindParameter * ptr;
        size_t            cnt;

        stParameterInfo()
            : ptr(nullptr),
            cnt(0) 
        {
        }
    };

    struct stColumnInfo 
    {
        STBindColumn *  ptr;
        size_t          cnt;

        stColumnInfo()
            : ptr(nullptr),
            cnt(0) 
        {
        }
    };

    struct stStmtInfo 
    {
        SQLHSTMT       *hstmt;
        unsigned long   uuidForConnection;
        stStmtInfo() :
            uuidForConnection(0)
        {
            hstmt = nullptr;
        }
    };

    class CPreparedSQL 
    {
        public:
            explicit CPreparedSQL();
            virtual ~CPreparedSQL();

            bool        SetSQL(const char * _sql, size_t _sqlLength);
            const char * GetSQL();
            size_t      GetSQLSize();

            void        SetBindParameter(STBindParameter * _arrP, size_t _cnt);
            void        SetBindColumn(STBindColumn * _arrP, size_t _cnt);

            SQLHSTMT &  AllocStatement(CPDBConn* a_plsPDBConn);
            void        CloseStatement(SQLHSTMT & stmt);

            bool        Prepared(SQLHSTMT &hstmt);
            bool        BindParameter(SQLHSTMT & _stmt,
                    STBindParameter * _arrP,
                    size_t _cnt);
            bool        BindColumn(SQLHSTMT & _stmt,
                    STBindColumn * _arrP,
                    size_t _cnt);

            SQLRETURN   GetSQLReturn() { return sret_; }
            const char * GetClassName() { return className_.c_str(); }

            virtual bool Execute(SQLHSTMT & _hstmt) = 0;

            void        AllPrint();

        protected:
            std::string     className_;
            SQLRETURN       sret_;

        private:
            void        clear(const char * a_pszFunc);
            bool        bindParameter(SQLHSTMT & _stmt);
            bool        bindColumn(SQLHSTMT & _stmt);
            void        Lock();
            void        UnLock();

        private:
            std::mutex          m_mutex;

            std::string         sql_;

            stParameterInfo     paramInfo_;
            stColumnInfo        colInfo_;

            std::vector<stStmtInfo>         vecStmtInfo_;
            stStmtInfo                      m_stInfo;
            SQLHSTMT                        m_sqlHStmt;

            std::function<bool(SQLHSTMT & _stmt)> func_;

    };

}
