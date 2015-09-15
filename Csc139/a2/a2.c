/* Name: David Saechao
   Course: Csc139
   Assignment 2
   
   This is a multi-thread program to calculate matrix multiplication for 12 inputs
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <pthread.h>

/* Middle used for splitting the array up */
#define MIDDLE 6

/* Mutex variables */
pthread_mutex_t mc0_Lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mc1_Lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mc2_Lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mc3_Lock = PTHREAD_MUTEX_INITIALIZER;

/* Arrays 'a' and 'b' are for storing the converted integer inputs each holding up to 6 indexes. 'c' is the resulting array */
int a[MIDDLE];	
int b[MIDDLE];
int c[4];

/* This function converts the inputs to integers to store in array */
void copyNumbers(int i, char *argv[]) {
	if (i < MIDDLE) {
		a[i] = atoi(argv[i+1]);
	} else {
		b[i-MIDDLE] = atoi(argv[i+1]);
	}  	
}

/* This function checks if the string is number */
void isNumber(char inputString[]) {
	int j;
	for (j = 0; j < strlen(inputString); j++) {
		
		/* If not a digit, exit */
		if ( !isdigit(inputString[j]) ) {
			printf("Incorrect type of inputs\n");
			exit(0);
		}
	}
}

/* Calculate c0 */
void *mc0() {
	
	/* Calculate c0 and print results */
	printf( "mc0 tid = %u\n", pthread_self() );
	c[0] = a[0]*b[0]+a[1]*b[2]+a[2]*b[4];
	printf( "mc0 finished result: c0 = %d\n", c[0] );
	printf( "mc0 communication to display: %d\n", c[0] );
	printf( "mc0 finished\n" );
	
	/* Unlock mutex */
	pthread_mutex_unlock(&mc0_Lock);
	
	pthread_exit(NULL);
}

/* Calculate c1 */
void *mc1() {
	
	/* Calculate c1 and print results */
	printf( "mc1 tid = %u\n", pthread_self() );
	c[1] = a[0]*b[1]+a[1]*b[3]+a[2]*b[5];
	printf( "mc1 finished result: c1 = %d\n", c[1] );
	printf( "mc1 communication to display: %d\n", c[1] );
	printf( "mc1 finished\n" );
	
	/* Unlock mutexes */
	pthread_mutex_unlock(&mc1_Lock);
	pthread_exit(NULL);
}

/* Calculate c2 */
void *mc2() {

	/* Calculate c3 and print results */	
	printf( "mc2 tid = %u\n", pthread_self() );
	c[2] = a[3]*b[0]+a[4]*b[2]+a[5]*b[4];
	printf( "mc2 finished result: c2 = %d\n", c[2] );
	printf( "mc2 communication to display: %d\n", c[2] );
	printf( "mc2 finished\n" );
	
	/* Unlock mutex */	
	pthread_mutex_unlock(&mc2_Lock);
	pthread_exit(NULL);
}

/* Calculate c3 */
void *mc3() {

	/* Calculate c3 and print results */
	printf( "mc3 tid = %u\n", pthread_self() );
	c[3] = a[3]*b[1]+a[4]*b[3]+a[5]*b[5];
	printf( "mc3 finished result: c3 = %d\n", c[3] );
	printf( "mc3 communication to display: %d\n", c[3] );
	printf( "mc3 finished\n" );
		
	/* Unlock mutex */	
	pthread_mutex_unlock(&mc3_Lock);
	pthread_exit(NULL);
}

/* Display results */
void *display() {
	
	/* Lock mutexes wait for others to unlock */	
	pthread_mutex_lock(&mc0_Lock);
	pthread_mutex_lock(&mc1_Lock);
	pthread_mutex_lock(&mc2_Lock);
	pthread_mutex_lock(&mc3_Lock);
	
	/* Display results */
	printf( "Display tid = %u\n", pthread_self() );
	printf( "The first row of the resulting matrix is:%6d  %4d\n", c[0], c[1] );
	printf( "The second row of the resulting matrix is:%5d  %4d\n", c[2], c[3] );
	printf( "Display finished\n" );
		
	pthread_exit(NULL);
}


int main (int argc, char *argv[]) {

	/* Check if correct number of inputs */
	if (argc != 13) {
		printf("Incorrect number of inputs\n");
		exit(0);
	} else {
		int i;
		for (i = 0; i < argc-1; i++) {
		
			/* Copy each input to variable 'inputString' */
			char inputString[strlen(argv[i+1])];
			strcpy(inputString, argv[i+1]);
			
			/* Check if input has a leading sign of '+' or '-' */
			if ( inputString[0] == '+' || inputString[0] == '-' ) {
				
				/* Move to string after leading sign */
				argv[i+1]++;
				strcpy(inputString, argv[i+1]);
				
				/* Check if string is number */
				isNumber(inputString);
				
				/* Move back to leading sign and copy and convert */
				argv[i+1]--;
				strcpy(inputString, argv[i+1]);
				copyNumbers(i,argv);
				
				
			} else {
			
			/* Check if string is number */
			isNumber(inputString);
			
			copyNumbers(i,argv);
			
			}
		}
		
		/* Create threads variables */
		pthread_t mc0_thread, mc1_thread, mc2_thread, mc3_thread, display_thread;
		
		/* Lock mutexes */
		pthread_mutex_lock(&mc0_Lock);
		pthread_mutex_lock(&mc1_Lock);
		pthread_mutex_lock(&mc2_Lock);
		pthread_mutex_lock(&mc3_Lock);
		
		/* Create mc0 thread */
		printf( "Creating mc0 thread\n" );
		pthread_create(&mc0_thread, NULL, mc0, NULL);
		
		/* Create mc1 thread */
		printf( "Creating mc1 thread\n" );
		pthread_create(&mc1_thread, NULL, mc1, NULL);
		
		/* Create mc2 thread */
		printf( "Creating mc2 thread\n" );
		pthread_create(&mc2_thread, NULL, mc2, NULL);
		
		/* Create mc3 thread */
		printf( "Creating mc3 thread\n" );
		pthread_create(&mc3_thread, NULL, mc3, NULL);
		
		/* Create display thread */
		printf( "Creating display thread\n" );
		pthread_create(&display_thread, NULL, display, NULL);
		
		/* Main thread wait for all threads to finish */
		printf( "Main thread waiting for all 5 threads to finish\n" );
		pthread_join(mc0_thread, NULL);
		pthread_join(mc1_thread, NULL);
		pthread_join(mc2_thread, NULL);
		pthread_join(mc3_thread, NULL);
		pthread_join(display_thread, NULL);
		
		/* Destroy locks */
		pthread_mutex_destroy(&mc0_Lock);
		pthread_mutex_destroy(&mc1_Lock);
		pthread_mutex_destroy(&mc2_Lock);
		pthread_mutex_destroy(&mc3_Lock);

		
		/* Print main thread id when finished */
		printf( "Main thread tid = %u\n", pthread_self() );
		printf( "Main thread finished\n" );

	}
}
