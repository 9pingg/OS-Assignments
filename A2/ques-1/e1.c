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
#include <fcntl.h>
#include <sys/stat.h>
#define INTERVAL 1000
int p_S1;
void printString(char *str){
    syscall(1, STDOUT_FILENO, str, strlen(str));
}
void signalHandlerr()

{
    uint64_t ret;
    int x;
    // int x;
    // __asm__("rdrand %0\n\t"
    //         "mov %0, %rax\n\t"
    //         :"=r"(x)::);
    asm("rdrand %0\n\t"
            :"=a"(ret)::);
    // x = ret;        
    // printf("%" PRIu64 "\n", ret);
    // printf("%d \n", x);
    char buff[25];
    // copy to buffer
    
    sprintf(buff, "%" PRIu64, ret);
    //printString("\nsr: ");
    //printString(buff);
    //printString("\n");
    union sigval value;
    value.sival_ptr = (void*) buff;
    
    int fd;
    char * myfifo = "file.txt";

    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);

    /* write "Hi" to the FIFO */
    fd = open(myfifo, O_WRONLY);
    write(fd, buff, 25);
    close(fd);
    //unlink(myfifo);
    /* remove the FIFO */
    //unlink(myfifo);


    // if(sigqueue(p_S1, SIGTERM, value) == 0) {
    //             printf("signal sent successfully!!\n");
    //     } else {
    //             perror("SIGSENT-ERROR:");
    // }
    kill(p_S1-1, SIGTERM);
}
void main(int argc, char const *argv[])
{ 
    p_S1 = atoi(argv[1]);
    struct itimerval it_val;
    if (signal(SIGALRM, (void (*)(int)) signalHandlerr) == SIG_ERR) {
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
    
    // char *str;
    // int p = fork();
    // if( p == 0){
    //     int a = getpid();
    //     sprintf(str, "%d", a);
    //     printf("%d\n %s", a, str);    
    //     // printf("%s", str);      
    // }   
    // else{
    //     // printf("p process : %d", getpid());
    //     waitpid(-1, NULL, 0);
    // }
    
}



