//Non-blocking named pipes
#include <stdio.h>
#include <fcntl.h>

#define MAX 10000000

char pipename[] = "/tmp/testpipe";
int main(int argc, char *argv[]) 
{
	if(argc == 1) {	//Reader
		int fd = open(pipename, O_RDONLY | O_NONBLOCK);
		int flags = fcntl(fd, F_GETFL, 0);
		int num, i;
		if(fd == -1)
			printf("Open failed\n");
		for(i = 0; i < MAX; i++) {
		    while(read(fd, &num, sizeof(int)) <= 0) {
				//Do nothing
			}
			if(num != i) {
				printf("Fail\n");
				exit(0);
			}
		}
		close(fd);
	} else { 	//Writer
		int fd = open(pipename, O_WRONLY | O_NONBLOCK);
		int num, i;
		if(fd == -1)
			printf("open failed\n");
		for(i = 0; i < MAX; i++) {
		    write(fd, &i, sizeof(int));
		}
		close(fd);	
	}
}
