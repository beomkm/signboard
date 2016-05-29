all: dataserver

CC = gcc
LIBS = -lpthread

#debug
#CFLAGS = -Wall -O1 -g

#release
CFLAGS = -Wall -O1

dataserver: dataserver.c
	$(CC) $(CFLAGS) dataserver.c -o bin/dataserver $(LIBS)

clean:
	rm dataserver

