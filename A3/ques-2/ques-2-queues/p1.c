#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#define MAX 10

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
// structure for message queue
struct mesg_buffer {
	long mesg_type;
        int index;
	char mesg_text[100];
} dataset[50], data, message;

int main()
{
	key_t key_1;
	key_t key_2;
	int msgid_1;
	int msgid_2;
	key_1 = ftok("a.txt", 65);
	key_2= ftok("b.txt", 66);
	msgid_1 = msgget(key_1, 0666 | IPC_CREAT);
	msgid_2 = msgget(key_2, 0666 | IPC_CREAT);
        for(int i = 0; i < 50 ; i++){
                strcpy(dataset[i].mesg_text, randstring(10));
                dataset[i].mesg_type = 1;
                dataset[i].index = i;
                printf("%d %s\n", dataset[i].index,dataset[i].mesg_text);
        }
        for(int i  = 0 ; i < 10 ; i ++){
            printf("Sending batch %d\n", i);
			for(int j = 0; j < 5 ; j ++){
                msgsnd(msgid_1, &dataset[j + 5 *i], sizeof(dataset[0]), 0);
                printf("Data sent is : %d %s \n", dataset[j + 5 *i].index, dataset[j + 5 *i].mesg_text);
                }
				sleep(1);
				msgrcv(msgid_2, &message, sizeof(message), 1, 0);
				printf("Data Received is : %d %s \n",message.index ,message.mesg_text);
				sleep(1);
        }
		msgctl(msgid_1, IPC_RMID, NULL);
		msgctl(msgid_2, IPC_RMID, NULL);
	return 0;
}