//Non-blocking named pipes
#include <stdio.h>
#include <time.h>

static inline long long get_cycles()
{
	unsigned low, high;
	unsigned long long val;
	asm volatile ("rdtsc" : "=a" (low), "=d" (high));
	val = high;
	val = (val << 32) | low;
	return val;
}

void nano_sleep(int ns)
{
	long long start = get_cycles();
	long long end = start;
	while(end - start < 2.1 * ns) {
		end = get_cycles();
	}
}

int main(int argc, char *argv[]) 
{
	int i = 0;
	for(i = 0; i < 500000; i++) {
		nano_sleep(2000);
	}
}
