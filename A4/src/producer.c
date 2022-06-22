#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define BYTE_COUNT 8
#define WRITER_SYSCALL_NO 449
int main(void){
    int cnt = 1;
    int fd = open("/dev/urandom", O_RDONLY);
    if(fd== -1){
        perror("open");
        exit(1);
    }
    char random_str[8];
    while(1){
        ssize_t res = read(fd, random_str, 8);
        if(res < 0){
            perror("read");
            exit(1);    
        }
        int result = syscall(WRITER_SYSCALL_NO, random_str, BYTE_COUNT);
        if(result < 0){
            perror("syscall");
            exit(1);
        }
        if(result >= 0){
            printf("******************************************\n\nstring sent:%d %s\n\n", cnt,random_str);
            cnt++;
        }
        sleep(2);
    }
    close(fd);
    return 0;
}
