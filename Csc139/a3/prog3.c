/* Name: David Saechao
   Course: Csc139
   Assignment 3
   
   This is a multi-thread program using semaphores to simulate producer/consumer problem.
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

/* Define buffer size */
#define SLOTSIZE 14
#define SLOTCNT 7

/* Buffer */
char buffer[SLOTCNT][SLOTSIZE];

/* Store file name */ 
char infileName[20];
char outfileName[20];

/* Producer index */
int pIndex;

/* Consumer index */
int cIndex;

/* Number of item in buffer */
int no_of_item;

/* Semaphore variables */
sem_t buf_lock, slot_avail, item_avail;

/* File pointers for infile and outfile */
FILE *infile, *outfile;

/* Producer thread */
void *producer() {
	
	/* Temporary string to store from infile */
	char string[SLOTSIZE];
	strncpy(string, "12345678901234", SLOTSIZE);
	
	/* Flag to enter and exit loop */
	int flag = 1;
	
	/* Read string of 14 byte from file */
	while(flag) {
		
		/* Get first 14 byte of string from file */
		if (fgets(string, SLOTSIZE, infile) != NULL) {

			/* Wait on semaphores for slot available and buffer lock */
			sem_wait(&slot_avail);
			sem_wait(&buf_lock);
			
			/* Place into buffer */
			strncpy(buffer[pIndex],string, SLOTSIZE);
			
			/* Produced item, added one item to buffer */
			no_of_item++;
			
			/* Update producer index */
			pIndex = (pIndex + 1) % SLOTCNT;
			
			/* Increment semaphore and signal */
			sem_post(&buf_lock);
			sem_post(&item_avail);
		
		/* End of infile so update flag */
		} else {
			flag = 0;
		}
	}
	
	/* Producer thread exiting */
	printf("Producer thread finished\n"); 
	pthread_exit(NULL);

}

/* Consumer thread */
void *consumer() {
	
	/* Temporary string for future copy */
	char string[SLOTSIZE];
	strncpy(string, "12345678901234", SLOTSIZE);
	
	/* Flag to enter and exit loop */ 	
	int flag = 1;
	
	/* Write string of 14 byte from file */
	while(flag) {
		
		/* Wait on semaphores for item available and buffer lock */
		sem_wait(&item_avail);
		sem_wait(&buf_lock);
		
		/* Read from buffer and store item in string */
		strncpy(string,buffer[cIndex],SLOTSIZE);
		
		/* Consumed item, one less item in buffer */
		no_of_item--;
		
		/* Update consumer index */
		cIndex = (cIndex + 1) % SLOTCNT;

		/* Increment semaphore and signal */
		sem_post(&buf_lock);
		sem_post(&slot_avail);
		
		/* Write to outfile */
		fputs(string,outfile);
		
		/* If infile reached end of file and there are no more items in buffer, update flag */
		if ( (feof(infile) != 0 ) && (no_of_item == 0) ) {
			flag = 0;	
		}	
	}
	
	/* Consumer thread exiting */
	printf("Consumer thread finished\n");
	fclose(outfile); 
	pthread_exit(NULL);
	
}

int main (int argc, char *argv[]) {

	/* Check if correct number of inputs */
	if (argc != 3) {
		printf("Correct Usage: prog3 infile outfile\n");
		exit(1);
	} else {
		
		/* Get infile and outfile name */
		strcpy(infileName, argv[1]);
		strcpy(outfileName, argv[2]);
		
		/* Read file pointer */
		infile = fopen(infileName, "r");
		
		/* Check if infile name exists in same directory */
		if ((infile=fopen(infileName,"r"))==NULL){
			printf("No such infile name exists\n");
			exit(1);    
		}
		
		/* Write file pointer */
		outfile = fopen(outfileName, "w");
		
		/* Initialize semaphores */
		sem_init(&buf_lock,0,1);
		sem_init(&slot_avail,0,7);
		sem_init(&item_avail,0,0);
		
		/* Create threads variables */
		pthread_t consumer_thread, producer_thread;
		
		/* Create producer thread */
		printf("Producer thread started\n");
		pthread_create(&producer_thread, NULL, producer, NULL);
		
		/* Create consumer thread */
		printf("Consumer thread started\n");
		pthread_create(&consumer_thread, NULL, consumer, NULL);
		
		/* Main thread wait for all threads to finish */
		pthread_join(producer_thread, NULL);
		pthread_join(consumer_thread, NULL);
		
		printf("Main thread finished\n");
	}
}
