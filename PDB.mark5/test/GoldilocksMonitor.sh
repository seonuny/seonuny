#!/bin/bash

#monitoring sh 파일 작성

#sh 파일 작성 후에 execute 권한을 부여해야 한다
#sudo chmod +x GoldilocksMonitor.sh
#

. ~/.bash_profile


HOST="192.168.10.56"
PORT=22582
GSQLNET="/pcf/goldilocks/bin/gsqlnet"
ID="test"
PWD="test"
GSQL="${GSQLNET} test test --conn-string HOST=${HOST};PORT=${PORT};UID=${ID};PWD=${PWD} --no-prompt";


doIt()
{
    START_DATE=`date +"%Y/%m/%d %H:%M:%S"`

    echo "=============================================="
    echo "= CHECK DATE:" $START_DATE       "           ="
    echo "=============================================="
    echo ""

    echo "#================================="
    echo "# total session count"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE", NVL(CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME, CONNECTION, STATUS, COUNT(*) 
  FROM X\$SESSION@GLOBAL[IGNORE_INACTIVE_MEMBER] GROUP BY CLUSTER_MEMBER_NAME, CONNECTION, STATUS ORDER BY 2,3,4;
\q
EOF
    echo ""

    echo "#================================="
    echo "# ager status check"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE",
       to_number ( split_part ( agable_scn_gap, '.', 1 )) GLOBAL_GAP,
       to_number ( split_part ( agable_scn_gap, '.', 2 )) DOMAIN_GAP,
       to_number ( split_part ( agable_scn_gap, '.', 3 )) LOCAL_GAP
       FROM GV\$AGABLE_INFO ORDER BY 1;
\q
EOF
    echo ""
    
    echo "#================================="
    echo "# undo segemnt status check"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE", NVL(CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME,
       SEGMENT_ID, PHYSICAL_ID, ALLOC_PAGE_COUNT, AGABLE_PAGE_COUNT
  FROM X\$UNDO_SEGMENT@GLOBAL[IGNORE_INACTIVE_MEMBER] 
 WHERE ALLOC_PAGE_COUNT > 1280 
GROUP BY CLUSTER_MEMBER_NAME, SEGMENT_ID, PHYSICAL_ID, ALLOC_PAGE_COUNT, AGABLE_PAGE_COUNT 
ORDER BY ALLOC_PAGE_COUNT DESC LIMIT 5;
\q
EOF
    echo ""
    
    echo "#================================="
    echo "# lock wait status check"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE", XS.CLIENT_PROCESS "CLIENT PID", XS.SERVER_PROCESS "SERVER PID", XS.ID,
       XLW.GRANTED_TRANSACTION_SLOT_ID, XLW.REQUEST_TRANSACTION_SLOT_ID "REQUEST_TX_ID", XTT.BEGIN_TIME "BEGIN_TIME"
  FROM X\$LOCK_WAIT XLW, X\$TRANSACTION XTT, X\$SESSION XS
 WHERE XLW.GRANTED_TRANSACTION_SLOT_ID = XTT.SLOT_ID AND XTT.LOGICAL_TRANS_ID = XS.TRANS_ID;
\q
EOF
    echo ""
    
    
    echo "#================================="
    echo "# long run transaction status check"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE", NVL(CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME, LOGICAL_TRANS_ID,
       DRIVER_TRANS_ID, STATE, BEGIN_TIME
  FROM X\$TRANSACTION@GLOBAL[IGNORE_INACTIVE_MEMBER]
 WHERE DATEDIFF( SECOND, BEGIN_TIME, LOCALTIMESTAMP ) > 5
 ORDER BY 4,2,5;
\q
EOF
    echo ""
    
    echo "#================================="
    echo "# long run statement status check"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT DECODE( ST.EXECUTIONS, 0 , NULL, ROUND( ST.TOTAL_EXEC_TIME/ST.EXECUTIONS , 3 ) ) COST
     , ST.EXECUTIONS, ST.TOTAL_EXEC_TIME, ST.LAST_EXEC_TIME, ST.START_TIME
     , LENGTHB( ST.SQL_TEXT ) LEN
     , SUBSTRB( ST.SQL_TEXT, 1, 80 ) SQLN
     , SS.PROGRAM_NAME
  FROM V\$STATEMENT ST
     , V\$SESSION SS
WHERE SS.SESSION_ID = ST.SESSION_ID
  AND ST.STMT_VIEW_SCN != '-1.-1.-1'
  AND DATEDIFF( MILLISECOND, START_TIME, LOCALTIMESTAMP ) > 100
ORDER BY ST.START_TIME, 1 DESC, 2 DESC
LIMIT 10;
\q
EOF
    echo ""
    
    echo "#================================="
    echo "# redo logfile status check"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE", NVL(CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME, A.*
  FROM X\$LOG_GROUP@GLOBAL[IGNORE_INACTIVE_MEMBER] A
  WHERE STATE NOT IN ( 'UNUSED', 'INACTIVE' )
 ORDER BY 2;
\q
EOF
    echo ""
    
    echo "#================================="
    echo "# redo log stream status check"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE", NVL(CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME,
       WAIT_COUNT_BY_BUFFER_FULL, BLOCKED_LOGGING_COUNT
  FROM X\$LOG_STREAM@GLOBAL[IGNORE_INACTIVE_MEMBER]
 ORDER BY 2,3,4;
\q
EOF
    echo ""
    
    
    echo "#================================="
    echo "# system lsn count check"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE", NVL(CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME,
       NAME, VALUE
  FROM X\$SM_SYSTEM_INFO@GLOBAL[IGNORE_INACTIVE_MEMBER]
 WHERE NAME = 'SYSTEM_LSN'
 ORDER BY 2;
\q
EOF
    echo ""
    
    echo "#================================="
    echo "# cluster queue check"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE", NVL(CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME,
       C.NAME, C.QUEUED, C.WAIT_COUNT
  FROM X\$CLUSTER_QUEUE@GLOBAL[IGNORE_INACTIVE_MEMBER] C
 WHERE CLUSTER_MEMBER_NAME LIKE '%N1' ORDER BY 2;
\q
EOF
    echo ""
    
    echo "#================================="
    echo "# cluster cm buffer check"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT NVL(CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME, A.* 
  FROM X\$CL_SYSTEM_INFO A ORDER BY 1;
\q
EOF
    echo ""
    
    echo "#================================="
    echo "# total statement count"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE", NVL(CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME,
       STMT_TYPE, EXECUTE_COUNT
  FROM X\$SQL_SYSTEM_STAT_EXEC_STMT@GLOBAL[IGNORE_INACTIVE_MEMBER]
 WHERE EXECUTE_COUNT > 10
 ORDER BY 2,3,4 DESC;
\q
EOF
    echo ""
    
    echo "#================================="
    echo "# processor session count"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE", NVL(CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME,
       PROGRAM, CONNECTION, STATUS, COUNT(*)
  FROM X\$SESSION
 WHERE PROGRAM NOT LIKE '%gmaster%'
   AND PROGRAM NOT LIKE '%cdispatcher%'
   AND PROGRAM NOT LIKE '%server%'
 GROUP BY CLUSTER_MEMBER_NAME, PROGRAM, CONNECTION, STATUS
 ORDER BY 2,4;
\q
EOF
    echo ""
    
    echo "#================================="
    echo "# total session count"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT '['||TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS' ) ||']' "DATE", NVL(CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME,
       COUNT(*)
  FROM X\$SESSION
 WHERE PROGRAM NOT LIKE '%gmaster%'
   AND PROGRAM NOT LIKE '%cdispatcher%'
   AND PROGRAM NOT LIKE '%server%'
 GROUP BY CLUSTER_MEMBER_NAME
 ORDER BY 2;
\q
EOF
        echo ""
        
        echo "#================================="
        echo "# transaction shard check"
        echo "#================================="
        $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT NVL(S.CLUSTER_MEMBER_NAME, 'STANDALONE') MEMBER_NAME, A.DRIVER_ID, A.DRIVER_COUNT,S.*
  FROM (
       SELECT DRIVER_TRANS_ID AS DRIVER_ID, COUNT(1) AS DRIVER_COUNT
         FROM X\$TRANSACTION
        WHERE DRIVER_TRANS_ID IN (
                                 SELECT DISTINCT(DRIVER_TRANS_ID)
                                   FROM X\$TRANSACTION
                                 )
       GROUP BY DRIVER_TRANS_ID
       ) A, X\$SESSION S
 WHERE S.TRANS_ID = A.DRIVER_ID
   AND A.DRIVER_COUNT > 2; 
\q
EOF
    echo ""
    
    echo "#================================="
    echo "# TABLESPACE SIZE CHECK"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT * FROM TECH_TABLESPACE;
EOF
    echo ""
    
    echo "#================================="
    echo "# TABLE SIZE CHECK"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT * FROM TECH_TABLE;
EOF
    echo ""
    
    echo "#================================="
    echo "# INDEX SIZE CHECK"
    echo "#================================="
    $GSQL << EOF
\set linesize 2048
\set pagesize 1000
SELECT * FROM TECH_INDEX;
EOF
    echo ""

    END_DATE=`date +"%Y/%m/%d %H:%M:%S"`
    printf "check complete\n[${START_DATE}]\n[${END_DATE}]\n"
}

TO_DATE=`date +%d%H%M%S`
TO_DATE1=`date +%Y%m`
C_DIR=`dirname ${0}`
LOG_DIR="${C_DIR}/log/${TO_DATE1}"
echo "${C_DIR}:${LOG_DIR}"
if [ ! -d "${LOG_DIR}" ]
then
    mkdir -p ${LOG_DIR}
fi

C_PWD=$PWD
echo ${C_PWD}
doIt | tee ${LOG_DIR}/${TO_DATE}.montest.log


