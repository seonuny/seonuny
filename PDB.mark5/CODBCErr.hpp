#pragma once

#include <string>
#include <sql.h>
#include <sqlext.h>

namespace PDB {

    class CODBCErr {
        public:
            explicit CODBCErr() = delete;
            ~CODBCErr() = delete;

            static std::string GetStateDBC(SQLHDBC _hdbc);
            static std::string GetStringDBC(SQLHDBC _hdbc);

            static std::string GetStateSTMT(SQLHSTMT _hstmt);
            static std::string GetStringSTMT(SQLHSTMT _hstmt);


    };

}

