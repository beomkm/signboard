#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENT 127
#define BACKLOG 10
#define PORT 12321

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "pass"
#define DB_NAME "mysql"

MYSQL mysql;
MYSQL *connection = NULL;

void *h_read(void *arg);
void *h_input(void *arg);

void read_db(void);

pthread_mutex_t mutex;
int count; //client count
int list[MAX_CLIENT]; //connecting client list

int main(void)
{
	//init socket
	void *vp; //for thread join
	pthread_t t_read;
	pthread_t t_input;

	int server, client; //socket
	struct sockaddr_in s_adr, c_adr;
	int size; //client address size

	if(pthread_create(&t_input, NULL, h_input, NULL) != 0) {
		fprintf(stderr, "Faild to create thread\n");
		exit(1);
	}

	count = 0;
	//init sql
	mysql_init(&mysql);
	connection = mysql_real_connect(&mysql,
									DB_HOST,
									DB_USER,
									DB_PASS,
									DB_NAME,
									3306,
									(char *)NULL,
									0);
	if(connection == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&mysql));
		exit(1);
	}



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
		size = sizeof(c_adr);
		client = accept(server, (struct sockaddr*)&c_adr, &size);

		pthread_mutex_lock(&mutex);
		printf("client fd : %d\n", client);
		list[count] = client;
		++count;
		pthread_mutex_unlock(&mutex);
		printf("Connected %s \n", inet_ntoa(c_adr.sin_addr));

		if(pthread_create(&t_read, NULL, h_read, (void*)&client)!=0) {
			fprintf(stderr, "thread creating error\n");
			exit(1);
		}
		pthread_detach(t_read);
	}


	pthread_join(t_input, &vp);

	mysql_close(connection);
	close(server);
	exit(0);
}

void *h_read(void *arg)
{
	int sock = *((int*)arg);
	char msg;
	int i;
	char len;

	MYSQL_ROW sql_row;
	MYSQL_RES *sql_result;
	//send db data when pi connect
	pthread_mutex_lock(&mutex);
	msg = 0x02;
	write(sock, &msg, 1);
	read_db();
	sql_result = mysql_store_result(connection);
	while((sql_row = mysql_fetch_row(sql_result)) != NULL) {
		len = (char)strlen(sql_row[2]);
		if(strcmp(sql_row[1], "1") == 0 && len != 0) { //when enable is true
			write(sock, &len, 1);
			write(sock, sql_row[2], (int)len);
		}
	}
	msg = 0x00; //end of data
	write(sock, &msg, 1);
	mysql_free_result(sql_result);
	pthread_mutex_unlock(&mutex);


	while(read(sock, &msg, sizeof(char))) {
		if(msg == 0x01) {
			write(sock, &msg, 1);
		}
	}


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
	printf("disconnected\n");
	close(sock);
	return NULL;
}

void* h_input(void *arg)
{
	MYSQL_ROW sql_row;
	MYSQL_RES *sql_result;
	struct sockaddr_in addr;
	int len; //
	char msg;
	char c;
	int i;

	while(1) {
		scanf("%c", &c);
		if(c == 'q')
			break;
		else if(c=='p') {
			//send db data
			for(i=0; i<count; i++) {
				pthread_mutex_lock(&mutex);
				msg = 0x02;
				write(list[i], &msg, 1);
				read_db();
				sql_result = mysql_store_result(connection);
				while((sql_row = mysql_fetch_row(sql_result)) != NULL) {
					len = (char)strlen(sql_row[2]);
					if(strcmp(sql_row[1], "1") == 0 && len != 0) { //when enable is true
						write(list[i], &len, 1);
						write(list[i], sql_row[2], (int)len);
					}
				}
				msg = 0x00; //end of data
				write(list[i], &msg, 1);
				mysql_free_result(sql_result);
				pthread_mutex_unlock(&mutex);
			}
		}
		else if(c=='i') {
			printf("current client count : %d\n", count);
			for(i=0; i<count; i++) {
				printf("client fd : %d\n", list[i]);
				getpeername(list[i], (struct sockaddr *)&addr, &len);
				printf("%s\n", inet_ntoa(addr.sin_addr));
			}
		}
	}
	exit(0);
}


void read_db(void)
{
	int status = mysql_query(connection, "select * from text");
	if(status != 0) {
		fprintf(stderr, "Mysql query error : %s\n", mysql_error(&mysql));
		exit(1);
	}
}
