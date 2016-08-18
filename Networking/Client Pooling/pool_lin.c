#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MAX_CLIENTS 10

void *pool_sockets();
int *sock_pool;
int sock;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	pthread_t pool_thread;	
	int status, pool_retval;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	//NULL makes it localhost
	if((status = getaddrinfo(NULL, "1337", &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}
	
	//Make the socket
	if((sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
	{
		fprintf(stderr, "socket error: %d\n", errno);
		exit(1);
	}
	
	//Bind the socket 
	if (bind(sock, servinfo->ai_addr, servinfo->ai_addrlen)==-1)
	{
		fprintf(stderr, "bind error: %d\n", errno);
		exit(1);
	}
	
	//Listen for clients
	if(listen(sock, MAX_CLIENTS) == -1)
	{
		fprintf(stderr, "listen error: %d\n", errno);
		exit(1);
	}
	
	//Ready to accept connections  
	pool_retval = pthread_create(&pool_thread, NULL, pool_sockets, NULL);
	pthread_join(pool_thread, NULL);
	
	
	
	//Cleanup
	pthread_mutex_destroy(&mutex1);
	free(sock_pool);
	freeaddrinfo(servinfo);
}

void *pool_sockets()
{
	pthread_mutex_lock(&mutex1);
	printf("Thread: %u   started", (unsigned int)pthread_self());
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof(their_addr);
	int num_clients = 1;
	
	if((sock_pool = (int*) malloc(sizeof(int)))==NULL)
	{
		fprintf(stderr, "malloc error\n");
		exit(1);
	}
	
	printf("Thread: %u   starting to accept clients", (unsigned int) pthread_self());
	while(num_clients < 10)
	{
		sock_pool[num_clients-1] = accept(sock, (struct sockaddr *)&their_addr, &addr_size);
		printf("Yay, client!");
		if((sock_pool = (int*) realloc(sock_pool, sizeof(int)*num_clients)) == NULL)
		{
			fprintf(stderr,"realloc error\n");
			exit(1);
		}
	}
	printf("We haz contact to TENZ clients!\n");
	pthread_mutex_unlock(&mutex1);
}