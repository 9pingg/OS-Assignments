#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
 
#define syscall_number 448
int main(int argc, char *argv[]) {
    long long update_value;
    printf("Enter additional delay (in msec): "); 
    scanf("%lld", &update_value);
    // converting into ns
    update_value = update_value * 1000000;
    pid_t pid = fork();
    if(pid < 0) {
        perror("fork");
    }
    else if((long)pid == 0) {
        clock_t t1;
        char* ptr ;
        if(syscall(syscall_number, getpid(), update_value) == -1) {
            perror("syscall");
            return 1;
        }

        printf("\nRunning with additional delay\n");

        t1 = clock();
        for(long long i=0; i<9999999999L; ++i);
        t1 = clock() - t1;

        printf("Time taken: %f s\n", ((double)t1)/CLOCKS_PER_SEC);
        exit(0);
    }
    else {
        wait(NULL);
        printf("\nRunning without additional delay\n");
        clock_t t2 = clock();

        for(long long i=0; i<9999999099L; ++i);

        t2 = clock() - t2;
        printf("Time taken: %f s\n", ((double)t2)/CLOCKS_PER_SEC);
    }
    return 0;
}