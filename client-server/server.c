#include "common.h"

void error(char *msg)
{
    perror(msg);
    exit(1);
}

//Usage: ./server
int main(int argc, char *argv[])
{
     int sockfd, newsockfd;
     struct sockaddr_in serv_addr;

     struct my_struct *recv_struct;
	 recv_struct = malloc(sizeof(*recv_struct));
	 memset(recv_struct, 0 , sizeof(*recv_struct));

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
		error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(STRUCT_PORT);

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");

     listen(sockfd, 5);

     newsockfd = accept(sockfd, NULL, NULL);
     if (newsockfd < 0)
		error("ERROR on accept");

     if (read(newsockfd, recv_struct, sizeof(*recv_struct)) < 0)
		error("ERROR reading from socket");
     printf("Here is the struct: %d %d\n", recv_struct->a, recv_struct->b);
     return 0; 
}
