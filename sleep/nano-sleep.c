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
	int upp = (int) (2.1 * ns);
	while(end - start < upp) {
		end = get_cycles();
	}
}

int main(int argc, char *argv[]) 
{
	int i = 0, sum = 5, mod = 7;
	long long start = get_cycles();
	for(i = 0; i < 1000; i++) {
		sum = sum % mod;
	}
	long long end = get_cycles();
	printf("%d %lld\n", sum, end - start);
}
