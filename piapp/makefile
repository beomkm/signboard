all: lcd

CC = arm-linux-gnueabi-gcc
LIBS = -lpthread

#debug
#CFLAGS = -Wall -O1 -g

#release
CFLAGS = -O2 -w

lcd: lcd.c
	$(CC) $(CFLAGS) lcd.c -o bin/lcd $(LIBS)

clean:
	rm lcd

