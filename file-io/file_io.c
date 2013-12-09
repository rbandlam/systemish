#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<fcntl.h>

#define NUM_OPS 1000000
int main()
{
	// Open the file. O_RDWR means read/write mode
	int i;
	for(i = 0; i < NUM_OPS; i++) {
		char filename[15];
		sprintf(filename, "%d", i);
		int fd = open(filename, O_RDWR | O_CREAT);
		write(fd, "Anuj", 4);
	}
	
	for(i = 0; i < NUM_OPS; i++) {
		char filename[15];
		sprintf(filename, "%d", i);
		unlink(filename);
	}
}

