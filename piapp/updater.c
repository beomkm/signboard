#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define F_BUF 100 //file read buffer
#define IP "0.0.0.0"
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
	char buf[F_BUF];
	int file_size;
	int read_size;
	int nread;
	int success = 0;
	FILE *fp;

	while(read(sock, &msg, 1)) {
		if(msg == 0x01) {
			//printf("ACK\n");
		}
		else if(msg == 0x02) {
			read(sock, &file_size, sizeof(int));
			printf("size : %d\n", file_size);
			if(file_size > 0) {
				fp = fopen("up", "w");
				if(fp != NULL) {
					while(1) {
						nread = read(sock, buf, F_BUF);
						read_size += nread;
						fwrite(buf, 1, nread, fp);
						if(read_size == file_size) break;
					}
					fclose(fp);
					success = 1;
					system("sudo pkill -9 lcd-arm");
					system("rm lcd-arm");
					system("sudo chmod 755 up");
					system("mv up lcd-arm");
					system("sudo ./lcd-arm &");
					printf("Update complete\n");
				}
				else {
					printf("Failed to write file\n");
				}
			}
			else {
				printf("Failed to receive file\n");
			}

			if(success == 1) {
				msg = 0x02;
				write(sock, &msg, 1);
			}
			else {
				msg = 0x03;
				write(sock, &msg, 1);
			}
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
	while(write(sock, &msg, 1) != -1) {
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
