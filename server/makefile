all: dataserver updserver monitor

CC = gcc

INCLUDE = -I/usr/include/mysql

LIBS = -lpthread -L/usr/lib/x86_64-linux-gnu/mysql -lmysqlclient
LIBSD = -lpthread

#debug
#CFLAGS = -Wall -O1 -g

#release
CFLAGS = -O1 -w

dataserver: dataserver.c
	$(CC) $(CFLAGS) dataserver.c -o bin/dataserver $(INCLUDE) $(LIBS)

updserver: updserver.c
	$(CC) $(CFLAGS) updserver.c -o bin/updserver $(LIBSD)

monitor: monitor.c
	$(CC) $(CFLAGS) monitor.c -o bin/monitor $(LIBSD)

clean:
	rm bin/*
