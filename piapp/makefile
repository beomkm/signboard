all: bin/lcd bin/lcd-arm

CCARM = arm-linux-gnueabi-gcc
CC = gcc

LIBS = -lpthread

#debug
#CFLAGS = -Wall -O1 -g

#release
CFLAGS = -O2 -w

bin/lcd-arm: lcd-arm.c
	$(CCARM) $(CFLAGS) lcd-arm.c -o bin/lcd-arm $(LIBS)

bin/lcd: lcd.c
	$(CC) $(CFLAGS) lcd.c -o bin/lcd $(LIBS)

clean:
	rm lcd lcd-arm
