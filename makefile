#für Make Test!
#$make -n for testing (shows actions without performing)

#extends search path to src/
VPATH = /src

### Variables: ###########

#bin = program name "Zieldatei"-> target
BIN = masterCubeSolver

#srcs = all necessary files -> relative path to .cpp file
SRCS = src/main.cpp\
src/masterCube.cpp\
src/client.cpp\
src/server.cpp\

#objs = source file -> depending obj.
#alt:objs = client.o server.o masterCube.o main.o
OBJS = $(SRCS:.cpp=.o)

HEADER =$(SRCS:.h)

#compiler commands
CC = g++

#compiler flags
#-g = adds debugging information to the executable file
CFLAGS = -g -O2 -DHAVE_CONFIG_H

#linker flags
LDFLAGS = -g -O2 -pg

#cpp preprocessor flags
#-wall = turns on most, but not all, compiler warnings
#CPPFLAGS = #-Wall

############################
#### First Target: #########
$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $(BIN) $(OBJS)
#masterCube.h server.h client.h
main.o: main.cpp $(HEADER)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o main.cpp -c masterCube.cpp server.cpp client.cpp

masterCube.o: masterCube.cpp masterCube.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -o masterCube.o -c masterCube.cpp

server.o: server.cpp server.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -o server.o server.cpp

client.o: client.cpp client.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -o client.o client.cpp

#### Clean Target: #########
clean:
	rm -f $(BIN) $(OBJS)
