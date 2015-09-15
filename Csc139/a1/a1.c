/* Name: David Saechao
   Course: Csc139
   Section: 2
   Programming Assignment 1
   
   This is a multiprocessing program to calculate matrix multiplication for 12 inputs
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>

/* Middle used for splitting the array up */
#define MIDDLE 6

/* Arrays 'a' and 'b' are for storing the converted integer inputs each holding up to 6 indexes */
int a[MIDDLE];	
int b[MIDDLE];

/* Array 'c' is the result of the matrix multiplication and 'cBuff' is buffer for pipes to make sure the data is passed through pipes */
int c[4];
int cBuff[4];

/* 'p0' to 'p3' is used for pipes read and write. 'mc' is for the child processes that calculate the matrix */
int p0[2],p1[2],p2[2],p3[2];
int mc[4];

/* 'display' is child process of displaying the result and 'status' for checking */
int display, status;

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
			
				/* Convert the inputs to integers to store in array */
				copyNumbers(i,argv);
			}
			
		}
		
		/* Create pipes */
		pipe(p0);
		pipe(p1);
		pipe(p2);
		pipe(p3);
		
		/* Create child process mc0 */
		printf( "Child process MC0 is being created\n" );
		mc[0] = fork();
		switch (mc[0]) {
			case -1:
				
				/* Error checking when creating process */
				perror("fork");
				exit(0);	
			case 0:
			
				/* If child process, then print child pid and parent pid */ 
				printf( "MC0 Child PID: %d\n", getpid() );
				printf( "MC0 Parent PID: %d\n", getppid() );
				
				/* Close read pipe and calculate c0 */
				close(p0[0]);
				c[0] = a[0]*b[0]+a[1]*b[2]+a[2]*b[4];
				
				/* Write c0 to pipe p0 and then close the write pipe and print result then exit process */
				write( p0[1], &c[0], sizeof(c[0]) );
				close(p0[1]);
				printf( "MC0 finished result: c0 = %d\n", c[0] );
				exit(0);
			default:

				/* If parent process, wait for child process MC0 to finish */
				printf( "Waiting for MC0 to finish\n" );
				waitpid(mc[0], &status, 0);
				printf( "Finished MC0\n\n" ); 
		}
		
		/* Create child process mc1 */
		printf( "Child process MC1 is being created\n" );
		mc[1] = fork();
		switch (mc[1]) {
			case -1:
				
				/* Error checking when creating process */
				perror("fork");
				exit(0);	
			case 0:
			
				/* If child process, then print child pid and parent pid */
				printf( "MC1 Child PID: %d\n", getpid() );
				printf( "MC1 Parent PID: %d\n", getppid() );
				
				/* Close read pipe and calculate c1 */
				close(p1[0]);
				c[1] = a[0]*b[1]+a[1]*b[3]+a[2]*b[5];
				
				/* Write c1 to pipe p1 and then close the write pipe and print result then exit process */
				write( p1[1], &c[1], sizeof(c[1]) );
				close(p1[1]);
				printf( "MC1 finished result: c1 = %d\n", c[1] );
				exit(0);
			default:
				
				/* If parent process, wait for child process MC1 to finish */
				printf( "Waiting for MC1 to finish\n" );
				waitpid(mc[1], &status, 0);
				printf( "Finished MC1\n\n" ); 
		}
		
		/* Create child process mc2 */
		printf( "Child process MC2 is being created\n" );
		mc[2] = fork();
		switch (mc[2]) {
			case -1:
				
				/* Error checking when creating process */
				perror("fork");
				exit(0);	
			case 0:
			
				/* If child process, then print child pid and parent pid */
				printf( "MC2 Child PID: %d\n", getpid() );
				printf( "MC2 Parent PID: %d\n", getppid() );
				
				/* Close read pipe and calculate c2 */
				close(p2[0]);
				c[2] = a[3]*b[0]+a[4]*b[2]+a[5]*b[4];
				
				/* Write c2 to pipe p2 and then close the write pipe and print result then exit process */
				write( p2[1], &c[2], sizeof(c[2]) );
				close(p2[1]);
				printf( "MC2 finished result: c2 = %d\n", c[2] );
				exit(0);				
			default:
				
				/* If parent process, wait for child process MC2 to finish */
				printf( "Waiting for MC2 to finish\n" );
				waitpid(mc[2], &status, 0);
				printf( "Finished MC2\n\n" );
		}
		
		/* Create child process mc3 */
		printf( "Child process MC3 is being created\n" );
		mc[3] = fork();
		switch (mc[3]) {
			case -1:
				
				/* Error checking when creating process */
				perror("fork");
				exit(0);	
			case 0:
				
				/* If child process, then print child pid and parent pid */
				printf( "MC3 Child PID: %d\n", getpid() );
				printf( "MC3 Parent PID: %d\n", getppid() );
				
				/* Close write pipe and then calculate c3 */
				close( p3[0] );
				c[3] = a[3]*b[1]+a[4]*b[3]+a[5]*b[5];
				
				/* Write c3 to pipe p3 and then close the write pipe and print result then exit process */
				write( p3[1], &c[3], sizeof(c[3]) );
				close( p3[1] );
				printf( "MC3 finished result: c3 = %d\n", c[3] );
				exit(0);
			default:
			
				/* If parent process, wait for child process MC3 to finish */
				printf( "Waiting for MC3 to finish\n" );
				waitpid(mc[3], &status, 0);
				printf( "Finished MC3\n\n" ); 
		}
		
		/* Create display process */
		printf( "Child process Display is being created\n" );
		display = fork();
		switch (display) {
			case -1:
				
				/* Error checking when creating child process */
				perror("fork");
				exit(0);	
			case 0:
			
				/* If child process, then print child pid and parent pid */
				printf( "Display Child PID: %d\n", getpid() );
				printf( "Display Parent PID: %d\n", getppid() );
				
				/* Close write pipes of p0, p1, p2, p3 */
				close( p0[1] );
				close( p1[1] );
				close( p2[1] );
				close( p3[1] );
				
				/* Read from pipes p0, p1, p2, p3 and store into buffer for printing matrix result*/
				read( p0[0], &cBuff[0], sizeof(cBuff[0]) );
				read( p1[0], &cBuff[1], sizeof(cBuff[1]) );
				read( p2[0], &cBuff[2], sizeof(cBuff[2]) );
				read( p3[0], &cBuff[3], sizeof(cBuff[3]) );
				printf( "The first row of the resulting matrix is:%6d  %4d\n", cBuff[0], cBuff[1] );
				printf( "The second row of the resulting matrix is:%5d  %4d\n", cBuff[2], cBuff[3] );
				
				/* Close read pipes of p0, p1, p2, p3 and exit process*/
				close( p0[0] );
				close( p1[0] );
				close( p2[0] );
				close( p3[0] );
				exit(0);
			default:
				
				/* If parent process, wait for child process Display to finish */
				printf( "Waiting for Display to finish\n" );
				waitpid(display, &status ,0);
				printf( "Finished Display\n\n");
				printf( "Finished Program Assignment 1\n");
 
		}
		
		
	}
}
