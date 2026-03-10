all: start client server

start: start.c
	gcc  start.c  -o start

client: client.c header.h
	gcc  client.c  -o client

server: server.c header.h
	gcc  server.c  -o server

clean:
	rm -f start
	rm -f client
	rm -f server