#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 128
#define IP "127.0.0.1"
#define PORT 23432

void *h_read(void *arg);
void *h_ack(void *arg);
void *h_input(void *arg);

int main(void)
{


	void *vp; //for thread join
	pthread_t t_read;
	pthread_t t_ack;
	pthread_t t_input;

	int sock;
	struct sockaddr_in s_adr;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&s_adr, 0, sizeof(s_adr));
	s_adr.sin_family = AF_INET;
	s_adr.sin_addr.s_addr = inet_addr(IP);
	s_adr.sin_port = htons(PORT);

	if(connect(sock, (struct sockaddr*)&s_adr, sizeof(s_adr))==-1) {
		fprintf(stderr, "Failed to connect\n");
		exit(1);
	}

	if(pthread_create(&t_read, NULL, h_read, (void*)&sock) != 0) {
		fprintf(stderr, "Faild to create thread\n");
		exit(1);
	}
	pthread_detach(t_read);

	if(pthread_create(&t_ack, NULL, h_ack, (void*)&sock) != 0) {
		fprintf(stderr, "Faild to create thread\n");
		exit(1);
	}
	pthread_detach(t_ack);

	if(pthread_create(&t_input, NULL, h_input, NULL) != 0) {
		fprintf(stderr, "Faild to create thread\n");
		exit(1);
	}
	pthread_join(t_input, &vp);

	exit(0);
}

void* h_read(void *arg)
{
	int sock = *((int*)arg);
	char count; //data count
	char size; //size of each data
	int i;
	char msg;

	while(read(sock, &msg, 1)) {
		if(msg == 0x01) {
			printf("ACK\n");
		}
		else if(msg == 0x02) {
			data_count = 0;
			printf("recv!\n");
		}
	}
	printf("disconnected\n");
	close(sock);
	pthread_exit(NULL);
}

void* h_ack(void *arg)
{
	int sock = *((int*)arg);
	char msg = 0x01;
	while(1) {
		write(sock, &msg, 1);
		sleep(2);
	}
	pthread_exit(NULL);
}


void* h_input(void *arg)
{
	char c;
	while(1) {
		scanf("%c", &c);
		if(c == 'q')
			break;
	}
	pthread_exit(NULL);
}
