/* Name: David Saechao
   Course: Csc139
   Assignment 6

   prog6.c
   
   This is a multi-processed program which works with memory-mapped file to improve performance.
*/

#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/mman.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <signal.h> 
#include <string.h> 
#include <errno.h>

struct stat buf;
char *mm_file;

int main(int argc, char *argv[]) {
	int flag = 0;
	int fd, status, length;

	char *find;
	char content[] = "virtual memory allows a disk file to be mapped into memory through the\nmmap call, thus making it possible for file i/o to be carried out via\nsimple memory accesses. in this assignment, the parent process memory maps\nthis disk file first, and then creates two child processes that each make\nsome changes to the file. when both child processes are done, changes made\nare synch’ed to the disk file.\n";
	
	/* Initialize file name, only mapped to one file */
	if (argc == 2) {
		length = strlen(content);
		fd = open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); 
		if (write(fd, content, length) != length) {
			perror("Error in writing");
			return 1;
		}
		flag = 1;
	} else {
		flag = 0;
		printf("Incorrect usage need to supply only 1 file name\n");
	}

	if (flag) {
		pid_t pid[2];

		/* Get status about the file */
		fstat(fd, &buf);
		
		/* Map the file into memory */
		if ((mm_file = mmap(0, (size_t) buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (void*) -1) {
			fprintf(stderr, "mmap call fails\n");
			return 1;
		}
		
		/* Child 1 process upper cases all characters */
		if ((pid[0] = fork()) == 0) {
			printf("Child 1 %d reads: \n%s\n", getpid(), mm_file);
			strncpy(content, mm_file, length);
			int i;
			for (i = 0; i < length; i++) {
				content[i] = toupper(content[i]);
			}
			strncpy(mm_file, content, length);
			msync(0, (size_t) buf.st_size, MS_SYNC);
			printf("Child 1 %d reads again: \n%s\n", getpid(), mm_file); 
			return 1;
		}
		
		/* Child 2 process replaces certain words */
		if ((pid[1] = fork()) == 0) {
		
			/* Reads and replaces from mapped file */
			sleep(1);
			printf("Child 2 %d reads: \n%s\n", getpid(), mm_file);
			strncpy(content, mm_file, length);
			
			/* Replace all occurrences of MEMORY MAPS with MEMORY-MAPS */
			while (strstr(content, "MEMORY MAPS")) {
				if ((find = strstr(content, "MEMORY MAPS")) == NULL) {
					perror("MEMORY MAPS");
				} else {
					strncpy(find, "MEMORY-MAPS", strlen("MEMORY-MAPS"));
				}
			}
			
			/* Replaces all occurrences of CHANGES with UPDATES */
			while (strstr(content, "CHANGES")) {
				if ((find = strstr(content, "CHANGES")) == NULL) {
					perror("CHANGES");
				} else {
					strncpy(find, "UPDATES", strlen("UPDATES"));
				}
			}
			/* Copy content back into the memory mapped location */
			strncpy(mm_file, content, length);
			printf("Child 2 %d reads again: \n%s\n", getpid(), mm_file);
			return 1;
		}
		
		/* Parent waits for child processes to finish */
		waitpid(pid[1], &status, 0);
		waitpid(pid[2], &status, 0);
		printf("Exiting: %d\n", getpid());
		close(fd);
		return 1;
	}
	return 1;
}
