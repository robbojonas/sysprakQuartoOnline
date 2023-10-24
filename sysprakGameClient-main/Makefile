CFLAGS = -g -Wall
CC = gcc

all: client serverConnect performConnection param 

client: client.c client.h serverConnect.o performConnection.o 
	$(CC) $(CFLAGS) -o sysprak-client client.c serverConnect.o performConnection.o

serverConnect: serverConnect.c serverConnect.h
	$(CC) $(CFLAGS) -c serverConnect.c

performConnection: performConnection.c performConnection.h 
	$(CC) $(CFLAGS) -c performConnection.c
	
param: param.c param.h 
	$(CC) $(CFLAGS) -c param.c

clean: 
	rm -f *.o ./sysprak-client ./serverConnect ./performConnection ./param
