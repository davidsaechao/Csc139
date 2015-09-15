/* Name: David Saechao
   Course: Csc139
   Assignment 5
   
   server.c
   
   Receives messages from client and converts them to all uppercase letters before sending back to client
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#define SERVER 1L

typedef struct {
 long msg_to;
 long msg_fm;
 char buffer[BUFSIZ];
} MESSAGE;

int mid;
key_t key;
struct msqid_ds buf;
MESSAGE msg;

int main (int argc, char *argv[]) {
	/* Assign key to message queue */
	key = ftok(".", 'z');
	mid = msgget(key, IPC_CREAT|0660);
	
	/* Flag to enter loop */
	int flag = 1;
	
	/* Waiting for signal to exit which is empty body */
	while (flag) {
		
		/* Receive message from client */
		if(msgrcv(mid, &msg, sizeof(msg.buffer), SERVER, 0) < 0){
			perror("msgrcv");
			exit(-1);
		} 
		
		/* Get client PID from message */
		long client_pid = msg.msg_fm;
		
		/* Check if empty message */
		if (sizeof(msg.buffer) == 0) {
			flag = 0;
			/* Remove message queue */
			msgctl(mid, IPC_RMID, (struct msqid_ds *) 0);
			exit(EXIT_SUCCESS);
		}
			
		/* Convert all lowercase characters to uppercase characters */
		int i;
		for (i = 0; i < sizeof(msg.buffer); i++) {
			msg.buffer[i] = toupper(msg.buffer[i]);
		}
			
		/* From SERVER to client */
		msg.msg_fm = SERVER;
		msg.msg_to = client_pid;
			
		/* Send converted message to client */
		if(msgsnd(mid, (struct MESSAGE*)&msg, sizeof(msg.buffer), 0) == -1){
			perror("msgsnd");
			exit(-1);
		}
		
		/* Exit loop if message empty */
		msgctl(mid, IPC_STAT, &buf);
		if ( (buf.msg_cbytes == 0) && (buf.msg_qnum >= 1) ) {
			flag = 0;
			/* Remove message queue */
			msgctl(mid, IPC_RMID, (struct msqid_ds *) 0);
			exit(EXIT_SUCCESS);
		}
		
	}

	/* Remove message queue */
	msgctl(mid, IPC_RMID, (struct msqid_ds *) 0);
	exit(EXIT_SUCCESS);
	
}