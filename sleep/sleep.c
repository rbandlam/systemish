//Non-blocking named pipes
#include <stdio.h>
#include <fcntl.h>
#include <time.h>

void micro_sleep(double microseconds)
{
	struct timespec start, end;
	double us = (double) microseconds / 1200000;
	clock_gettime(CLOCK_REALTIME, &start);
	while(1) {
		clock_gettime(CLOCK_REALTIME, &end);
		double seconds = (end.tv_sec - start.tv_sec) + 
			(double) (end.tv_nsec - start.tv_nsec) / 1000000000;
		if(seconds > us) {
			return;
		}
	}
	return;
}

int main(int argc, char *argv[]) 
{
	int i, ans;
	struct timespec tim1, tim2;
	tim1.tv_sec = 0;
	tim1.tv_nsec = 100;
	for(i = 0; i < 1000000; i++) {
		micro_sleep(1);
	}
}
