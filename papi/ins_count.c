#include <stdio.h>
#include <stdlib.h>
#include <papi.h>

long long ins_count;

void papi_start(void) 
{
    int ins = PAPI_TOT_INS;
    if (PAPI_start_counters(&ins, 1) != PAPI_OK) {
        printf("Error starting instruction count\n");
		exit(-1);
    }
}

void papi_mark(void) 
{
    if (PAPI_read_counters(&ins_count, 1) != PAPI_OK) {
        printf("Error reading instruction count\n");
		exit(-1);
    } else {
        printf("Instructions = %lld\n", ins_count);
    }
}

int main(int c, char** args)
{
	papi_start();
	papi_mark();
}
