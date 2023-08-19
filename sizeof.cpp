#include <stdio.h>
#include <time.h>


typedef struct _st_smf_session_info_
{
    char szData01 [ 128 + 1];
    char szData02 [  32 + 1];
    char szData04 [  22 + 1];
    char szData05 [  15 + 1];
    char szData06 [  64 + 1];
    char szData07 [  64 + 1];
    char szData09 [   6 + 1];
    char szData10 [  32 + 1];
    char szData11 [   6 + 1];
    char szData12 [  12 + 1];
    char szData13 [   1 + 1];
    char szData14 [1024 + 1];
    char szData15 [ 896 + 1];
    char szData16 [ 160 + 1];
    char szData17 [ 192 + 1];
    char szData18 [ 160 + 1];
    char szData19 [ 160 + 1];
    char szData20 [   1 + 1];
    char szData21 [  32 + 1];
    char szData22 [  32 + 1];
    char szData23 [  16 + 1];
    char szData24 [  64 + 1];
    char szData29 [  64 + 1];
    int   nData03           ;
    int   nData08           ;
    int   nData25           ;
    int   nData26           ;
    time_t Data27           ;
    time_t Data28           ;
} ST_SMF_SESSION_INFO;


int main(int argc, char **argv)
{
    printf("[%ld][%ld][%ld][%ld]\n",sizeof(int),sizeof(time_t),sizeof(ST_SMF_SESSION_INFO));
    return 0;
}
