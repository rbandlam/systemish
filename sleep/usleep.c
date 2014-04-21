#include <stdio.h>
#include <time.h>

/*
 * Measurements on xia-router0: Xeon L5640 @2.27 GHz
 * 100000 usleep(1) = 5.6 sec ==> usleep(1) = 56 us
 * 
 * Measurements on xia-router2: Xeon E5-2680 @2.7 GHz
 * usleep(1) = 58 us
 */
int main(int argc, char *argv[]) 
{
	int i, ans;
	for(i = 0; i < 100000; i++) {
		usleep(1);
	}
}
