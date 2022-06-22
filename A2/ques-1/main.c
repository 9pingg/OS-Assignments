#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include <sys/time.h>		
#include <string.h>
#include<sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h> 

#define MAX_BUF 1024

void printString(char *str){
    syscall(1, STDOUT_FILENO, str, strlen(str));
}
void signal_handler(int signo){

    int fd;
    char * myfifo = "file.txt";
    char buff[MAX_BUF];

        fd = open(myfifo, O_RDONLY) ;
        read(fd, buff, MAX_BUF);
        printf("Received sr: %s\n", buff);
        close(fd);
    
}
void signalHandler(int signo, siginfo_t *info, void *extra){
    void *ptr_val = info->si_value.sival_ptr;
    char *str = (char*) ptr_val;
    printString("In mainn's signal handler: ");
    printString(str);
}
void printInt(int number) {
    char buffer[20];
    sprintf(buffer, "%d", number);
    printString(buffer);
}
int main(){
    int count = 3;
    char str[25];
    // int pid_p = fork();
    pid_t p_S1;
    pid_t p_st;
    int ps1, pst, psr;
    ps1 = fork();
    if(ps1 < 0 ){
        perror("Error in forking");
    }
    p_S1 = ps1;    //check
    if(ps1 == 0)
    {   //sleep(50);
        p_S1 = getpid();
        printString("In S1 process: ");
        printInt(getpid());
        printString("\n");
        struct sigaction sa;
        //sa.sa_handler = signalHandler;
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = &signalHandler;
        if(sigaction(SIGTERM, &sa, NULL)== -1){
            perror("Unable to catch SIGTERM");
        }
        if (signal(SIGTERM, (void (*)(int)) signalHandler) == SIG_ERR) {
        perror("Unable to catch SIGALRM");
        exit(1);
        }
        sleep(50);
        // while(1){
        //     pause();
        // }
        // while(wait(NULL) > 0);
    }
    else{
        psr = fork();
        if(psr == 0){
        
            printString("In SR process: ");
            printInt(getpid());
            printString("\n");
            sprintf(str, "%d", p_S1);
            execl("e1", "e1", str, NULL);
        }
        else{
            pst = fork();
            if(pst == 0){
                p_st = getpid();
                printString("In ST process: ");
                printInt(getpid());
                printString("\n");
                sprintf(str, "%d", p_S1);
                execl("e2", "e2", str, NULL);
            }
            else{              
                sleep(1);  
                signal(SIGTERM, signal_handler);
                while(wait(NULL) > 0);
            }    
        }
    }
   
   
    return 0;

}