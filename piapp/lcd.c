#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 17

int main(void)
{
	FILE *fp;
	char msg[BUF_SIZE] = "Hello LCD!";
	int len = strlen(msg);

	fp = fopen("/dev/lcd_ascii", "wt");
	if(fp == NULL) {
		fprintf(stderr, "Failed to open device driver!\n");
		exit(1);
	}

	fwrite(msg, 1, len+1, fp); //including '\0'

	fclose(fp);


	exit(0);
}
