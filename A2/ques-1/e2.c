#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <inttypes.h>
#include<string.h>
#include<time.h>
#define INTERVAL 1000 // 1s
int p_S1;
int p_st;
void printString(char *str){
    syscall(1, STDOUT_FILENO, str, strlen(str));
}
void signalHandlerForSIGTERM(int signo, siginfo_t *info, void *extra){
    void *ptr_val = info->si_value.sival_ptr;
    char *str = (char*) ptr_val;
    printString("Received st: ");
    printString(str);
    printString("\n");
}
void signalHandlerForSIGALRM()

{
    uint64_t ret;
    asm volatile ( "rdtsc\n\t"    // Returns the time in EDX:EAX.
            "shl $32, %%rdx\n\t"  // Shift the upper bits left.
            "or %%rdx, %0"        // 'Or' in the lower bits.
            : "=a" (ret)
            : 
            : "rdx");
    //printf("\nst: %llx", ret);
    time_t t;
    time(&t);
    char buff[50];
    // copy to buffer
    // sm volatile ( "rdtsc\n\t"    // Returns the time in EDX:EAX.
    //         "shl $32, %%rdx\n\t"  // Shift the upper bits left.
    //         "or %%rdx, %0"        // 'Or' in the lower bits.
    //         : "=a" (ret)
    //         : 
    //         : "rdx");


    sprintf(buff, "%" PRIu64, ret);
    strcat(buff, " Readable format: ");
    strcat(buff, ctime(&t));
    //printString("\nst: ");
    //printString(buff);
    union sigval value;
    value.sival_ptr = (void*) buff;
    if(sigqueue(p_st, SIGTERM, value) == 0) {
                printf("\n");
        } else {
                perror("SIGSENT-ERROR:");
    }

}
void main(int argc, char const *argv[])
{ 
    p_S1 = atoi(argv[1]);
    p_st = getpid();
    struct sigaction sa;
                //sa.sa_handler = signalHandler;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &signalHandlerForSIGTERM;
    if(sigaction(SIGTERM, &sa, NULL)== -1){
        perror("Unable to catch SIGTERM");
    }  
    struct itimerval it_val;
    if (signal(SIGALRM, (void (*)(int)) signalHandlerForSIGALRM) == SIG_ERR) {
        perror("Unable to catch SIGALRM");
        exit(1);
    }
    it_val.it_value.tv_sec =     INTERVAL/1000;
    it_val.it_value.tv_usec =    (INTERVAL*1000) % 1000000;	
    it_val.it_interval = it_val.it_value;
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        perror("error calling setitimer()");
        exit(1);
    }

    while (1) 
        pause();
    
}



