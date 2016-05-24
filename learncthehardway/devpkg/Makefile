PREFIX?=/usr/local
CFLAGS=-g -Wall -I${PREFIX}/apr/include/apr-1 -I${PREFIX}/apr/include/apr-util-1 -D_LARGEFILE64_SOURCE=1

all: devpkg

devpkg: bstrlib.o db.o shell.o commands.o

LDFLAGS=-L${PREFIX}/apr/lib 
LDLIBS=-lapr-1 -pthread -laprutil-1

install: all
	install -d ${DESTDIR}/${PREFIX}/bin/
	install devpkg ${DESTDIR}/${PREFIX}/bin/

clean:
	rm -f *.o
	rm -f devpkg
	rm -rf *.dSYM
