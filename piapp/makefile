all: lcd

CC = arm-linux-gnueabi-gcc

#debug
CFLAGS = -Wall -O1 -g

#release
CFLAGS = -Wall -O1

lcd: lcd.c
	$(CC) $(CFLAGS) lcd.c -o lcd

clean:
	-rm lcd

