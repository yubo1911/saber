CFLAGS=-g -Wall 
PREFIX?=/usr/local

LDFLAGS=-L${PREFIX}/lib
LDLIBS=-luv -pthread

clean:
	rm -f *.o
	rm -f libuv-echo-client epoll-echo-server socket-echo-server select-echo-server libuv-echo-server libuv-helloworld
	rm -rf *.dSYM
