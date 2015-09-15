/* Name: David Saechao
   Course: Csc139
   Assignment 5
   
   client1.c
   
   Sends two messages from two input files
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
FILE *infile1, *infile2;

/* Store file name */ 
char infileName1[20];
char infileName2[20];

int main (int argc, char *argv[]) {
	
	/* Assign key to message queue */
	key = ftok(".", 'z');
	mid = msgget(key, 0);
	
	/* Get infile names */
	strcpy(infileName1, argv[1]);
	strcpy(infileName2, argv[2]);
		
	/* Read file pointer */
	infile1 = fopen(infileName1, "r");
	infile2 = fopen(infileName2, "r");	
	
	/* Check if infile name exists in same directory */
	if ( ((infile1=fopen(infileName1,"r"))==NULL) || ((infile2=fopen(infileName2,"r"))==NULL) ){
		printf("No such infile name exists\n");
		exit(1);    
	}
	
	/* Read from file1 */
	while(1) {
		
		/* Copy string from file */
		if (fgets(msg.buffer, BUFSIZ, infile1) != NULL) {
		
		} else {
			break;
		}
	}
	
	/* Print message from file1 */
	printf("Message1 before: %s\n", msg.buffer);
	
	/* From client1 to SERVER */
    long client_pid = (long)getpid();
    msg.msg_fm = client_pid;
	msg.msg_to = SERVER;
	
	/* Send message to server */
    if(msgsnd(mid, &msg, sizeof(msg.buffer), 0) == -1){
        perror("msgsnd");
        exit(-1);
    }
	
	/* Receive converted message from server */
	if(msgrcv(mid, &msg, sizeof(msg), client_pid, 0) < 0){
        perror("msgrcv");
        exit(-1);
    }
	
	/* Print message from server */
	printf("Message1 after: %s\n", msg.buffer);
	
	/* Read from file2 */
	while(1) {
		
		/* Copy string from file */
		if (fgets(msg.buffer, BUFSIZ, infile2) != NULL) {
		
		} else {
			break;
		}
	}
	
	/* Print message from file2 */
	printf("Message2 before: %s\n", msg.buffer);
	
	/* From client1 to SERVER */
	msg.msg_fm = client_pid;
	msg.msg_to = SERVER;
	
	/* Send message to server */
    if(msgsnd(mid, &msg, sizeof(msg.buffer), 0) == -1){
        perror("msgsnd");
        exit(-1);
    }
	
	/* Receive converted message from server */
	if(msgrcv(mid, &msg, sizeof(msg), client_pid, 0) < 0){
        perror("msgrcv");
        exit(-1);
    }
	
	/* Print message from server */
	printf("Message2 after: %s\n", msg.buffer);
	exit(1);

}