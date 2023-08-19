#include <thread>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <execinfo.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <setjmp.h>

//setjmp와 longjmp가 같이 사용하는 jmp_buf로
//전역적으로 사용
jmp_buf jmpbuf;

void sigCapture1(int sigNo)
{
	printf("[%s:%s:%d] signal[%d]\n",__FILE__,__func__,__LINE__,sigNo);
}

// 2014.01.07 19:38:42 write by seonuny@ntels.com
// sigbus,sigsegv 이벤트 발생시 호출 되는 함수
void sigCapture2(int nSigNo)
{
	//printf("[%s:%s:%d] signo[%d]\n",__FILE__,__func__,__LINE__,nSigNo);

	void           *array[100];    /* Array to store backtrace symbols */
	size_t          size;     /* To store the exact no of values stored */

	size = backtrace(array, 100);

	char strDate[8+1];
	struct timeval  localTimeValue;
	struct timezone localTimeZone;
	struct tm       TTm;
	struct tm*      pTm;
	gettimeofday(&localTimeValue, &localTimeZone);
	pTm = localtime_r(&localTimeValue.tv_sec, &TTm);

	memset( strDate, 0x00, sizeof(strDate) );
	sprintf(strDate, "%04d%02d%02d", pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday);

	printf("********** backtrace symbols start **********\n");
    int nFd = fileno(stdout);
	backtrace_symbols_fd(array, size,nFd);

	printf("********** backtrace symbols end **********\n");

	int nPid = getpid();

	char strCommand[100];
	char strBuffer[1024];

	sprintf(strCommand,"pstack %d\n",nPid);

	printf("********** pstack symbols start **********\n");
	FILE * pfFileD = popen(strCommand,"r");
	int nLen=0;
	if(pfFileD)
	{
		while(fgets(strBuffer,1024,pfFileD) != NULL)
		{
			nLen = strlen(strBuffer);
            strBuffer[nLen-1] = 0x00;
			printf("%s\n",strBuffer);
		}
	}
	pclose(pfFileD);
	printf("********** pstack symbols end **********\n");
    fflush(stdout);

    longjmp(jmpbuf,1);

  //exit(-1);
}
    char szData[2] = {0,};

void Thread()
{
    for(int i = 0 ; i < 1000; ++i)
    {
        printf("[%d]\n",i);
        sleep(1);
    }
}

int main(int argc, char **argv)
{
    bool bFlag = true;
    if(0)
    {
        bFlag = false;
    }
    ////////////////////////////////////////////////////
    // signal handler
    ////////////////////////////////////////////////////
    static struct sigaction act1, act2, act3;
    int nLoopCnt = 0;

  //std::thread t1(Thread);

    act1.sa_handler  = SIG_IGN     ;    // SIG_IGN : ignore signal
    sigaction(SIGPIPE, &act1, NULL);

    act2.sa_handler  = sigCapture1  ;

    sigaction(SIGUSR1, &act2, NULL);    // 16번   : PF&M Stop Command
    sigaction(SIGINT , &act2, NULL);    // SIGINT : Ctrl C
    sigaction(SIGALRM, &act2, NULL);    // 14번   : alarm clock timeout  //시그널 정의

    act3.sa_handler  = sigCapture2 ;

    sigaction(SIGSEGV, &act3, NULL);
    sigaction(SIGABRT, &act3, NULL);
    sigaction(SIGBUS , &act3, NULL);

    char *ptr = nullptr;
	int ret = 0;	
	if((ret = setjmp(jmpbuf)) < 0)
    {
		printf("setjmp error\n");
		return -1;
	}
    else
    {
		//longjmp에서 두번째 인자 val이 ret값이 됨.
		switch(ret){
            case 0:
                printf("succ\n");
                break;
			case 1:
                bFlag = false;
				printf("longjmp.\n",0);
				exit(1);
		}
	}
	while (true)
    {
        ++nLoopCnt;
        printf("[%d]\n",bFlag);
        if(bFlag == false)
        {
            break;
        }
        {
            printf("before\n");
            memcpy(ptr,"012345678901234567890",20);
            printf("[%s]\n",ptr+15);
        }
        if(nLoopCnt>1000000)
        {
            printf("break[%d]\n",nLoopCnt);
            break;
        }
    }
    return 0;
}
