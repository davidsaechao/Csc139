/* Name: David Saechao
   Course: Csc139
   Assignment 5
   
   client2.c
   
   Sends message from an input file and then sends empty body message
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
FILE *infile3;
char infileName3[20];

int main (int argc, char *argv[]) {

	/* Assign key to message queue */
	key = ftok(".", 'z');
	mid = msgget(key, 0);
	
	/* Get infile name */
	strcpy(infileName3, argv[1]);
	
	/* Check if infile name exists in same directory */
	if ( ((infile3=fopen(infileName3,"r"))==NULL) ){
		printf("No such infile name exists\n");
		exit(1);    
	}
	
	/* Read from file3 */
	while(1) {
		
		/* Copy string from file */
		if (fgets(msg.buffer, BUFSIZ, infile3) != NULL) {
		
		} else {
			break;
		}
	}
	
	/* Print message from file3 */
	printf("Message3 before: %s\n", msg.buffer);
	
	/* From client2 to SERVER */
    long client_pid = (long)getpid();
    msg.msg_fm = client_pid;
	msg.msg_to = SERVER;
	
	/* Send message to server */
    if(msgsnd(mid, &msg, sizeof(msg.buffer), 0) == -1){
        perror("msgsnd");
        exit(-1);
    }
	
	/* Send empty body message signaling exit to server */
	if(msgsnd(mid, &msg, 0, 0) == -1){
        perror("msgsnd");
        exit(-1);
    }
	
	/* Receive converted message from server */
	if(msgrcv(mid, &msg, sizeof(msg), client_pid, 0) < 0){
        perror("msgrcv");
        exit(-1);
    }
	
	/* Print message from server */
	printf("Message3 after: %s\n", msg.buffer);
}	