#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *handler(void *arg);

int main(int argc, char* argv[])
{
	char input;

	//thread
	int res;
	pthread_t a_thread;

	res = pthread_create(&a_thread, NULL, handler, NULL);
	if(res != 0) {
		perror("Thread creation failed");
		exit(1);
	}


	while(1) {
		scanf("%c", &input);
		switch(input) {
			case 'q':
				printf("bye\n");
				exit(0);
				break;
		}
		getchar();
	}

	exit(0);
}


void *handler(void *arg)
{

  int buf0;//PID input
  char buf1[8];//tty input
  char buf2[16];//time input
  char buf3[64];//name input
  FILE *fp;

  int p1; //dataserver
  int p2; //updserver

	while(1) {
   p1 = p2 = 0;
  	fp = popen("ps -e | grep \"dataserver\\|updserver\"", "r");
  	while(!feof(fp)) {
  		fscanf(fp, "%d%s%s%s", &buf0, buf1, buf2, buf3);
  		//printf("%d %s %s %s\n", buf0, buf1, buf2, buf3);
  		if(strcmp(buf3, "dataserver") == 0) {
        p1 = 1;
  		}
      else if(strcmp(buf3, "updserver") == 0) {
        p2 = 1;
      }
  	}
  	pclose(fp);
	if(p1 == 0) {
		printf("dataserver is off.\nauto start.\n");
		system("./dataserver &");
	}
	else {
		printf("dataserver is running\n");
	}

	if(p2 == 0) {
		printf("updserver is off.\nauto start.\n");
		system("./updserver &");
	}
	else {
		printf("updserver is running\n");
	}

	printf("Type 'q' to quit\n\n");
	sleep(5);
	}
	pthread_exit(NULL);
}
