#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <setjmp.h>

// Define
#define TICK_NS_TIME (10000000)                                                 // 0.01 sec = 10 ms (100 times per second)
#define NS_PER_SEC   (1000000000)                                               // Nano sec per second.
#define TICK_PER_SEC (NS_PER_SEC/TICK_NS_TIME)                                  // Number of tick per second (Ex:100)
#define TIMEOUT_COUNT (30*TICK_PER_SEC)                                         // 30 seconds timeout (with 100 tick per second)

// Env set/long jmp
#define ENV_SZ (2)
#define ENV_TIMEOUT (0)
#define ENV_CTRLC (1)
static jmp_buf env[ENV_SZ];

// Variables
int timeout_val;


// sig handler.
void signal_handler(int signo)
{
    pthread_t self = pthread_self();
    printf("Thread %lu in signal handler\n", (long)self);
    if (signo == SIGINT) {
        longjmp(env[ENV_CTRLC], 1);                                             // Q?: Is it in the same thread ? (Never, Always, Sometimes?)
    }
    else 
    {
        printf("Other signal received..quitting.");                             // Ex: kill -9 pid
        exit(0);
    }
}

// thread timer function
void* timer_function(void* in_param)
{
    // Loop approx 100x per second.
    for (;;) {
        nanosleep((const struct timespec[]){{0, TICK_NS_TIME }}, NULL);         // Sleep 10 ms seconds.
        if (timeout_val) {
            if (!--timeout_val) {
                longjmp(env[ENV_TIMEOUT], 1);                                   // longjmp when timer reaches 0. (Q?: Is this valid with multithread?)
            }
        }
    }
}

// main
int main(int argc, char **argv)
{
    int i;
    int val;
    struct sigaction actions;
    pthread_t thread;
    setvbuf (stdout, NULL, _IONBF, 0);                                          // Make sure stdout is not buffered (ex:printf, etc.)
    printf("[Program started]\r\n");

    memset(&actions, 0, sizeof(actions));
    sigemptyset(&actions.sa_mask);
    actions.sa_flags = 0;
    actions.sa_handler = signal_handler;
    val = sigaction(SIGINT, &actions, NULL);  
    pthread_create(&thread, NULL, timer_function, NULL);                        // timer thread for example
    printf("[Timer thread started]\r\n");

    // setting env.
    val = setjmp(env[ENV_TIMEOUT]);
    if (val!=0){ printf("[JMP TIMEOUT]\r\n"); }

    val = setjmp(env[ENV_CTRLC]);
    if (val!=0){ printf("[JMP CTRLC]\r\n"); }

    // main loop
    timeout_val = TIMEOUT_COUNT;
    i = 0;
    for (;;)
    {
        i++;
        if (i > 10){ i = 0; printf("[%d]", timeout_val/TICK_PER_SEC); }         // Number of seconds before time out.
        sleep(1);
        printf(".");
    }
    printf("Main completed\n");
    return 0;
}
//Compile: g++ -pthread jmp.cpp -o jmp
