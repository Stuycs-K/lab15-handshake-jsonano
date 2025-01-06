.PHONY: all run client server clean

all: client server

run:
	@./server.out & ./client.out

client: basic_client.o pipe_networking.o
	@gcc -o client.out basic_client.o pipe_networking.o

server: basic_server.o pipe_networking.o
	@gcc -o server.out basic_server.o pipe_networking.o

basic_client.o: basic_client.c pipe_networking.h
	@gcc -c basic_client.c -Wall

basic_server.o: basic_server.c pipe_networking.h
	@gcc -c basic_server.c -Wall

pipe_networking.o: pipe_networking.c pipe_networking.h
	@gcc -c pipe_networking.c

clean:
	@rm *.o client server
