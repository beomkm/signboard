all: bin/lcd bin/lcd-arm bin/updater

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

bin/updater: updater.c
	$(CC) $(CFLAGS) updater.c -o bin/updater $(LIBS)

clean:
	rm bin/lcd bin/lcd-arm bin/updater
