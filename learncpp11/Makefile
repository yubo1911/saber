CXX=g++-4.9
CPPFLAGS=-g -std=c++11 -O2 -Wall -Wextra -rdynamic -DNDEBUG $(OPTFLAGS)
LDLIBS=$(OPTLIBS)

SOURCES=$(wildcard *.cpp)
TARGETS=$(patsubst %.cpp,%,$(SOURCES))

all: $(TARGETS)

clean:
	rm -rf $(TARGETS)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

