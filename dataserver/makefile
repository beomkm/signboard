all: dataserver

CC = gcc

INCLUDE = -I/usr/include/mysql

LIBS = -lpthread -L/usr/lib/x86_64-linux-gnu/mysql -lmysqlclient

#debug
#CFLAGS = -Wall -O1 -g

#release
CFLAGS = -O1

dataserver: dataserver.c
	$(CC) $(CFLAGS) dataserver.c -o bin/dataserver $(INCLUDE) $(LIBS)

clean:
	rm dataserver
