#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

// structure for message queue
struct mesg_buffer {
	long mesg_type;
    int index;
	char mesg_text[100];
} message, dataset[50], data_to_send;

int main()
{               
    for(int i = 0 ; i  < 50; i ++){
            dataset[i].mesg_type = 1;
    }	
	key_t key_1;
	key_t key_2;
	int msgid_1;
	int msgid_2;

	// ftok to generate unique key
	key_1 = ftok("a.txt", 65);
	key_2= ftok("b.txt", 66);

	msgid_1 = msgget(key_1, 0666 | IPC_CREAT);
	msgid_2 = msgget(key_2, 0666 | IPC_CREAT);
        time_t secs = 25; 
        time_t startTime = time(NULL);
        while (time(NULL) - startTime < secs)
        {
        	if(msgrcv(msgid_1, &message, sizeof(message), 1, 0) == -1){
				continue;
			}
			else{
				if((message.index + 1) % 5 == 0){
					data_to_send.mesg_type = 1;
					strcpy(data_to_send.mesg_text, message.mesg_text);
					data_to_send.index = message.index;
					if(msgsnd(msgid_2, &data_to_send, sizeof(data_to_send), 0) == -1){
						break;
					}
				}
				printf("Data Received is :%d %s \n",message.index ,message.mesg_text);
			}
		}

      	msgctl(msgid_1, IPC_RMID, NULL);
		msgctl(msgid_2, IPC_RMID, NULL);

	return 0;
}