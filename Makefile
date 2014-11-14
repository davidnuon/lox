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
	install ./lox /usr/bin/lox

uninstall:
	rm /usr/bin/lox

clean:
	rm ./lox