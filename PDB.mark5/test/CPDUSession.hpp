#pragma once
#include <sqlext.h>

#include "CNDFODBCAdaptor.hpp"
#include "CODBClib.hpp"


#include "BaseDefine.hpp"
#include "define.hpp"
#include "CPreparedSQL.hpp"

class CPDUSessionInsert: public PDB::CPreparedSQL, public CODBClib
{
    private:
        SQL_TIMESTAMP_STRUCT    m_tmsCreateTime  ;
        SQL_TIMESTAMP_STRUCT    m_tmsUpdateTime  ;
        ST_SMF_SESSION_INFO     m_stSMFSessionInfo;
    public:
        CPDUSessionInsert();
        ~CPDUSessionInsert();
      //void SetLogName(char *a_pszLogName) { SNPRINTF(m_szLogNM,"%s",a_pszLogName); }
        bool Initialize();
        bool SetValue(ST_SMF_SESSION_INFO *a_pstSMFSessionInfo);
        ST_SMF_SESSION_INFO* 
             GetValue()                         { return &m_stSMFSessionInfo; }
        bool Prepare();
        bool SetParameter();
        bool SetBindParameter();
        bool Execute(SQLHSTMT &a_sqlHStmt);
};

class CPDUSessionUpdAll: public PDB::CPreparedSQL, public CODBClib
{
    private:
        ST_SMF_SESSION_INFO     m_stSMFSessionInfo;
        SQL_TIMESTAMP_STRUCT    m_tmsUpdateTime ;
    public:
        CPDUSessionUpdAll();
        ~CPDUSessionUpdAll();
      //void SetLogName(char *a_pszLogName) { SNPRINTF(m_szLogNM,"%s",a_pszLogName); }
        bool Initialize();
        bool SetValue(ST_SMF_SESSION_INFO *a_pstSMFSessionInfo);
        ST_SMF_SESSION_INFO* 
             GetValue()                         { return &m_stSMFSessionInfo; }
        bool Prepare();
        bool SetParameter();
        bool SetBindParameter();
        bool Execute(SQLHSTMT &a_sqlHStmt);
};

class CPDUSessionUpdate: public PDB::CPreparedSQL, public CODBClib
{
    private:
        ST_SMF_SESSION_INFO     m_stSMFSessionInfo;
        SQL_TIMESTAMP_STRUCT    m_tmsUpdateTime ;
    public:
        CPDUSessionUpdate();
        ~CPDUSessionUpdate();
      //void SetLogName(char *a_pszLogName) { SNPRINTF(m_szLogNM,"%s",a_pszLogName); }
        bool Initialize();
        bool SetValue(ST_SMF_SESSION_INFO *a_pstSMFSessionInfo);
        ST_SMF_SESSION_INFO* 
             GetValue()                         { return &m_stSMFSessionInfo; }
        bool Prepare();
        bool SetParameter();
        bool SetBindParameter();
        bool Execute(SQLHSTMT &a_sqlHStmt);
};

class CPDUSessionDelete: public PDB::CPreparedSQL, public CODBClib
{
    private:
        char                    m_szSMPolicyID[LEN_SM_POLICY_ID+1];
    public:
        CPDUSessionDelete();
        ~CPDUSessionDelete();
      //void SetLogName(char *a_pszLogName) { SNPRINTF(m_szLogNM,"%s",a_pszLogName); }
        bool Initialize();
        bool SetValue(char *a_pszSMPolicyID);
        bool Prepare();
        bool SetParameter();
        bool SetBindParameter();
        bool Execute(SQLHSTMT &a_sqlHStmt);
};


class CPDUSessionSelect: public PDB::CPreparedSQL, public CODBClib
{
    private:
        SQL_TIMESTAMP_STRUCT    m_tmsCreateTime  ;
        SQL_TIMESTAMP_STRUCT    m_tmsUpdateTime  ;
        char                    m_szSMPolicyID[LEN_SM_POLICY_ID +1];
        ST_SMF_SESSION_INFO     m_stSMFSessionInfo;
    public:
        CPDUSessionSelect();
        ~CPDUSessionSelect();
      //void SetLogName(char *a_pszLogName) { SNPRINTF(m_szLogNM,"%s",a_pszLogName); }
        bool Initialize();
        bool SetValue(char *a_pszSMPolicyID);
        ST_SMF_SESSION_INFO* 
             GetValue()                         { return &m_stSMFSessionInfo; }
        bool Prepare();
        bool SetParameter();
        bool SetColumn();
        bool SetBindParameter();
        bool SetBindColumn();
        bool Execute(SQLHSTMT &a_sqlHStmt);
};


class CPDUSessionSelectMDN: public PDB::CPreparedSQL, public CODBClib
{
    private:
        SQL_TIMESTAMP_STRUCT    m_tmsCreateTime  ;
        SQL_TIMESTAMP_STRUCT    m_tmsUpdateTime  ;
        char                    m_szMDN[LEN_MDN +1];
        char                    m_szDNN[LEN_DNN +1];
        ST_SMF_SESSION_INFO     m_stSMFSessionInfo;
    public:
        CPDUSessionSelectMDN();
        ~CPDUSessionSelectMDN();
      //void SetLogName(char *a_pszLogName) { SNPRINTF(m_szLogNM,"%s",a_pszLogName); }
        bool Initialize();
        bool SetValue(char *a_pszMDN,char *a_pszDNN);
        ST_SMF_SESSION_INFO* 
             GetValue()                         { return &m_stSMFSessionInfo; }
        bool Prepare();
        bool SetParameter();
        bool SetColumn();
        bool SetBindParameter();
        bool SetBindColumn();
        bool Execute(SQLHSTMT &a_sqlHStmt);
};

