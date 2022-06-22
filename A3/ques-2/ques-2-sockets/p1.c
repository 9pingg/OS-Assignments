#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define SOCKET_NAME "/tmp/asjkhdkja9342.socket"
#define BUFFER_SIZE 20
char *randstring(size_t len_string) { 
    char character_set[] = "abcdefghijklmnopqrstuvwxyz"; 
    char *randomString;
    if (len_string) {
        randomString = malloc(len_string + 1);

        if (randomString) {
            int l = (int) (sizeof(character_set) -1); 
            int key;  
            for (int n = 0;n < len_string;n++) 
            {        
                key = rand() % l;   
                randomString[n] = character_set[key];
            }

            randomString[len_string] = '\0';
        }
    }
    return randomString;
}
struct mesg_buffer {
    int index;
	char mesg_text[100];
} dataset[50], data, message;

int main(int argc, char *argv[])
{
    for(int i = 0; i < 50 ; i++){
        strcpy(dataset[i].mesg_text, randstring(10));
        dataset[i].index = i;
        printf("%d %s\n", dataset[i].index,dataset[i].mesg_text);
    }
    struct sockaddr_un addr;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];

    data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));

    /* Connecting socket to socket address. */

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = connect(data_socket, (const struct sockaddr *) &addr,
                    sizeof(addr));
    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }
   for(int i  = 0 ; i < 10 ; i ++){
        printf("Sending batch %d\n\n", i);
		for(int j = 0; j < 5 ; j ++){
            printf("index : %d, string: %s\n", dataset[j + 5 *i].index, dataset[j + 5 *i].mesg_text);
            ret = send(data_socket,  &dataset[j + 5 *i], sizeof(dataset[0]), 0);
            if (ret == -1) {
                perror("write");
                break;
            }
            sleep(1);
        }
            ret = recv(data_socket, &data, sizeof(data), 0);
            if (ret == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            else{
                printf("received index: %d, string: %s\n\n", data.index, data.mesg_text);
            }
        }
    


    close(data_socket);

    exit(EXIT_SUCCESS);
}