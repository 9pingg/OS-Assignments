#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_INDEX 49
struct mesg_buffer {
    int index;
	char mesg_text[100];
} dataset[50], data, message;
int main()
{
    int fd1;
    char * file_name = "file.txt";
    mkfifo(file_name, 0666);
    int x = -1;
    
    // used to clear file.txt, if it already has some data in it.
    fd1 = open(file_name, O_WRONLY | O_TRUNC);
    close(fd1);

    while (1)
    {
        // First open in read only and read
        fd1 = open(file_name,O_RDONLY);
        if(fd1 == -1){
            perror("file open");
        }
        int r = read(fd1, &data, sizeof(data));
        if(r == -1){
            close(fd1);
            perror("read");
            exit(1);
        }
        else{
        // Print the read string and close
            
            if(x == data.index){
                close(fd1);
                continue;
            }
            if(MAX_INDEX == data.index){
               printf("index : %d, string: %s\n", data.index, data.mesg_text);
                x = data.index;
                close(fd1);
                break;
            }
            if(strcmp(data.mesg_text, "") != 0){
                x = data.index;
                printf("index : %d, string: %s\n", data.index, data.mesg_text);
                close(fd1);
            }
            else{
                close(fd1);
                continue;
            }
        }
        // Now open in write mode and write
        // string taken from user.
        if(data.index + 1 % 5 == 0){
            fd1 = open(file_name,O_WRONLY);
            write(fd1, &data, sizeof(data));
            close(fd1);
        }
    }
    return 0;
}