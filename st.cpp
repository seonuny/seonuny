#include <stdio.h>


int main(int argc , char **argv)
{
typedef struct _stZoneNotiResp_                     
{                                                   
    char msg_type        [  1 ]; //
    char sys_id          [  6 ]; //
    char branch_name     [  2 ]; //
    char p_tid           [ 24 ]; //
    char tid             [ 34 ]; //
    char mdn             [ 11 ]; //
    char result_code     [  4 ]; //
} ST_ZN_OUT_RESP_MSG;                               

	printf("[%u]\n",sizeof(ST_ZN_OUT_RESP_MSG));

    return 0;

}
