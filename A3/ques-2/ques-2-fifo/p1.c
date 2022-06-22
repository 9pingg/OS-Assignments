// C program to implement one side of FIFO
// This side writes first, then reads
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

char *randstring(size_t length) { // const size_t length, supra
    static char charset[] = "abcdefghijklmnopqrstuvwxyz"; // could be const
    char *randomString;

    if (length) {
        randomString = malloc(length + 1); // sizeof(char) == 1, cf. C99

        if (randomString) {
            int l = (int) (sizeof(charset) -1); // (static/global, could be const or #define SZ, would be even better)
            int key;  // one-time instantiation (static/global would be even better)
            for (int n = 0;n < length;n++) {        
                key = rand() % l;   // no instantiation, just assignment, no overhead from sizeof
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }
    return randomString;
}
struct mesg_buffer {
    int index;
	char mesg_text[100];
} dataset[50], data, message;

int main()
{   
    printf("Printing all random strings with their index:\n\n");
    for(int i = 0; i < 50 ; i++){
        strcpy(dataset[i].mesg_text, randstring(10));
        dataset[i].index = i;
        printf("%d %s\n", dataset[i].index,dataset[i].mesg_text);
    }
    int fd;
    // FIFO file path
    char * file_name = "file.txt";
    if(mkfifo(file_name, 0666) < 0){
        
    }

        for(int i  = 0 ; i < 10 ; i ++){
            printf("Sending batch %d\n\n", i);
		    for(int j = 0; j < 5 ; j ++){
                printf("index : %d, string: %s\n", dataset[j + 5 *i].index, dataset[j + 5 *i].mesg_text);
                fd = open(file_name, O_WRONLY);
                if (fd == -1) {
                    perror("open");
                    break;
                }
                write(fd, &dataset[j + 5 *i], sizeof(dataset[0]));
                close(fd);
                sleep(1);
            }
             fd = open(file_name, O_RDONLY);
             if(fd == -1){
                perror("open");
             }
            // Read from FIFO
            read(fd, &data, sizeof(data));
            printf("received index: %d, string: %s\n\n", data.index, data.mesg_text);
            close(fd);
        }
    
    return 0;
}