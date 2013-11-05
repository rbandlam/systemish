//Non-blocking named pipes
#include <stdio.h>
#include <fcntl.h>

#define MAX 100

int main(int argc, char *argv[]) 
{
	if(argc == 1) {	//Reader
		int fd = open("/tmp/testpipe", O_RDONLY | O_NONBLOCK);
		int flags = fcntl(fd, F_GETFL, 0);
		int num, i;
		if(fd == -1)
			printf("Open failed\n");
		for(i = 0; i < MAX; i++) {
		    int res = read(fd, &num, sizeof(int));
			printf("%d\n", res);
			sleep(1);
		}
		close(fd);
	} else { 	//Writer
		int fd = open("/tmp/testpipe", O_WRONLY);
		int num, i;
		if(fd == -1)
			printf("open failed\n");
		for(i = 0; i < MAX; i++) {
		    write(fd, &i, sizeof(int));
		}
		close(fd);	
	}
}
