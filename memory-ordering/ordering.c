#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define SIZE 2
void *client_function(void *ptr);
void *server_function(void *ptr);

int a, b;

main()
{
	pthread_t client_thread, server_thread;

	pthread_create(&client_thread, NULL, client_function, NULL);
	pthread_create(&server_thread, NULL, server_function, NULL);
	
	pthread_join(client_thread, NULL);
	pthread_join(server_thread, NULL);
	
	exit(0);
}

void *server_function(void *ptr)
{
	int iter = 0, i;
	while(1) {
		a = iter;
		b = iter;
		iter ++;
		usleep(200 + rand()%200);
	}
}

void *client_function(void *ptr)
{
	int iter = 0, i;
	int poll_index = SIZE - 1, check_index = 0;
	while(1) {
		while(b == iter) {
			//do nothing
		}
		if(a != b) {
			printf("Failed\n");
			exit(0);
		}
		iter ++;
	}
}
