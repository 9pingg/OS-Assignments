#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#define SOCKET_NAME "/tmp/asjkhdkja9342.socket"
#define BUFFER_SIZE 20

struct mesg_buffer {
	
    int index;
	char mesg_text[100];
} dataset[50], data, message;

int main(int argc, char *argv[])
{
    struct sockaddr_un name;
    int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    int result;
    char buffer[BUFFER_SIZE];
    unlink(SOCKET_NAME);
    // creating local socket
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (connection_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&name, 0, sizeof(name));
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    ret = bind(connection_socket, (const struct sockaddr *) &name,
                sizeof(name));
    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Prepare for accepting connections 

    ret = listen(connection_socket, 20);
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* handling connections. */

    for (;;) {

        /* Wait for incoming connection. */

        data_socket = accept(connection_socket, NULL, NULL);
        if (data_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        result = 0;
        for (;;) {

            ret = recv(data_socket, &data, sizeof(buffer), 0);
            if (ret == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            printf("index: %d, string: %s\n", data.index, data.mesg_text);

            if((data.index + 1) % 5 == 0){    
            ret = send(data_socket, &data, sizeof(buffer), 0);
            if (ret == -1) {
                goto label;
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        }


        close(data_socket);

        if (down_flag) {
            break;
        }
    }
    label:
    close(connection_socket);

    /* Unlink the socket. */

    unlink(SOCKET_NAME);

    exit(EXIT_SUCCESS);
}