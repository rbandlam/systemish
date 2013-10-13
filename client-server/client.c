#include "common.h"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

//Usage: ./client <server_ip>
int main(int argc, char *argv[])
{
    int sockfd;

    struct sockaddr_in serv_addr;
    struct hostent *server;

	struct my_struct *send_struct;
	send_struct = malloc(sizeof(*send_struct));	
	memset(send_struct, 0, sizeof(*send_struct));
	send_struct->a = 3185;
	send_struct->b = 3187;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL)
		error("No such host");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
	serv_addr.sin_port = htons(STRUCT_PORT);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

	if (write(sockfd,send_struct,sizeof(*send_struct)) < 0)
		error("ERROR writing to socket");
    return 0;
}
