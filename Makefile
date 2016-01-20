CC=g++
CFLAGS=-Wall
LDFLAGS=
SOURCES=lox.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=lox 

all: $(SOURCES) $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $(SOURCES) -o $@

install:
	install ./lox /usr/local/bin/lox

uninstall:
	rm /usr/local/bin/lox

clean:
	rm ./lox
