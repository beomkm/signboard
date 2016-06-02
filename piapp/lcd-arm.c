#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 17
#define MSG_SIZE 68
#define DISP_SIZE 100
#define IP "127.0.0.1"
#define PORT 12300

void* h_read(void *arg);
void* h_ack(void *arg);
void* h_display(void *arg);
void* h_input(void *arg);

void display(char *msg)
{
	FILE *fp;
	int len = strlen(msg);
	char disp[DISP_SIZE];
	char blank[BUF_SIZE];
	int i;

	fp = fopen("/dev/lcd_ascii", "wt");
	if(fp == NULL) {
		fprintf(stderr, "Failed to open device driver!\n");
		exit(1);
	}

	for(i=0; i<BUF_SIZE; i++) {
		blank[i] = ' ';
	}
	for(i=0; i<15; i++)
		disp[i] = ' ';
	for(i=0; i<len; i++)
		disp[i+15] = msg[i];
	for(i=0; i<17; i++)
		disp[i+15+len] = ' ';



	for(i=0; i<len+16; i++) {
		printf("%d\n",i);
		fwrite(disp+i+1, 1, BUF_SIZE+1, fp); //including '\0'
		fflush(fp);
		usleep(500*1000);
		fwrite(blank, 1, BUF_SIZE+1, fp); //including '\0'
		fflush(fp);
		usleep(160*1000);
	}

	fclose(fp);


}

int main(void)
{
	FILE *fp;
	char msg[MSG_SIZE];

	int len;
	void *vp; //for thread join

	printf("input text : ");
	fgets(msg, MSG_SIZE, stdin);
	len = strlen(msg);

	display(msg);
	/*

	pthread_t t_read;
	pthread_t t_ack;
	pthread_t t_display;
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
	*/



	/*
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

	if(pthread_create(&t_display, NULL, h_display, NULL) != 0) {
		fprintf(stderr, "Faild to create thread\n");
		exit(1);
	}
	pthread_detach(t_display);

	if(pthread_create(&t_input, NULL, h_input, NULL) != 0) {
		fprintf(stderr, "Faild to create thread\n");
		exit(1);
	}
	pthread_join(t_input, &vp);
	*/

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
			read(sock, &count, 1);
			for(i=0; i<count; i++) {
				read(sock, &size, 1);
			}
		}
	}
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

void* h_display(void *arg)
{
	while(1) {
		printf("h_display\n");
		sleep(1);
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
