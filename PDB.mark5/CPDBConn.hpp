#pragma once


#include <sql.h>
#include <sqlext.h>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include <string>
#include <unordered_set>

#include "define.hpp"


namespace PDB 
{
    #define PDBMAXBUFLEN 1024*64*1
    typedef std::unordered_map<std::string,SQLHSTMT*> MAP_STMT;
    typedef MAP_STMT::iterator                         IT_STMT;
    typedef std::pair<IT_STMT,bool>                    RT_STMT;
    typedef std::pair<const std::string,SQLHSTMT*>     PA_STMT;
    class CPDBConn
    {
        private:
            unsigned long m_ulUUID;
            char          m_szErrMsg[PDBMAXBUFLEN];
            SQLCHAR       m_SQLState[PDBMAXBUFLEN];
            SQLCHAR       m_MessageText[PDBMAXBUFLEN];
            SQLINTEGER    m_NativeErrorPtr;
            SQLSMALLINT   m_TextLengthPtr;

            std::string   m_stringODBC;

            bool          m_bAutoCommit;
            bool          m_bConnection;
            SQLHENV       m_sqlHEnv;
            SQLHDBC       m_sqlHDbc;
            SQLHSTMT      m_sqlHStmt;
            SQLRETURN     m_sqlReturn;

            MAP_STMT      m_mapStmt;

        public:
            CPDBConn(unsigned long a_ulUUID);
            ~CPDBConn();
            void SetConfig(std::string &a_stringODBC)
            {
                m_stringODBC = a_stringODBC;
            }
            unsigned long GetUuid(void) { return m_ulUUID; }

            char* GetErrMsg() { return m_szErrMsg; }
            UCHAR* GetSQLState() { return m_SQLState; }
            SQLRETURN PreConnect();
            SQLRETURN Connect(int a_nTryCnt=3);
            SQLRETURN Disconnect();
            SQLRETURN Postdisconnect();
            bool      IsConnection() { return m_bConnection; }
            void SetAutoCommit(bool a_bAutoCommit=true)
            {
                m_bAutoCommit = a_bAutoCommit;
            }
            bool Commit();
            bool Rollback();
            bool AllocStmt(SQLHSTMT &a_sqlHStmt);
            bool ReleaseStmt(SQLHSTMT &a_sqlHStmt);
            bool ChkStmt(int a_nLine);
            SQLHSTMT& FindStmt(SQLHSTMT &a_sqlHStmt);
            bool GetDiagRec(
                    SQLSMALLINT handleType,
                    SQLHANDLE   handle,
                    SQLRETURN   a_sqlReturn,
                    const  char       *a_pszCatch=NULL);

        private:
            void      FreeStmt(const char *a_szFunc,int a_nLine);
            void      Clear();

    };

}
