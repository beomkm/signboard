#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENT 127
#define BACKLOG 10
#define PORT 12300

void *handler(void *arg);

pthread_mutex_t mutex;
int count = 0; //client count
int list[MAX_CLIENT]; //connecting client list

int main()
{
	pthread_t thread;
	int server, client; //socket
	struct sockaddr_in s_adr, c_adr;
	socklen_t size; //client address size

	pthread_mutex_init(&mutex, NULL);

	server = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_adr, 0, sizeof(s_adr));
	s_adr.sin_family = AF_INET;
	s_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_adr.sin_port = htons(PORT);

	if(bind(server, (struct sockaddr*)&s_adr, sizeof(s_adr))==-1) {
		fprintf(stderr, "bind error\n");
		exit(1);
	}
	if(listen(server, BACKLOG)==-1) {
		fprintf(stderr, "listen error\n");
		exit(1);
	}

	printf("wating to connect.. port : %d\n", PORT);

	while(1) {
		size = (socklen_t)sizeof(c_adr);
		client = accept(server, (struct sockaddr*)&c_adr, &size);

		pthread_mutex_lock(&mutex);
		++count;
		list[count] = client;
		pthread_mutex_unlock(&mutex);
		printf("Connected %s \n", inet_ntoa(c_adr.sin_addr));

		if(pthread_create(&thread, NULL, handler, (void*)&client)!=0) {
			fprintf(stderr, "thread creating error\n");
			exit(1);
		}
		pthread_detach(thread);
	}
	close(server);
	exit(0);
}

void *handler(void *arg)
{
	int sock = *((int*)arg);
	int i;

	//disconnecting
	pthread_mutex_lock(&mutex);
	for(i=0; i<count; i++) {
		if(list[i] == sock) {
			for(; i<count-1; i++) {
				list[i] = list[i+1];
			}
			break;
		}
	}
	--count;
	pthread_mutex_unlock(&mutex);
	close(sock);
	return NULL;
}	
