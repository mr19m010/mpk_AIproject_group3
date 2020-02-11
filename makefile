#makefile.am -> .am wird für aufruf $make weggelassen!
#$make -n for testing (shows actions without performing)

VPATH = src

### Variables: ###########

#bin = program name "Zieldatei"-> target
bin = prog2

#srcs = all necessary files -> relative path to .cpp file
srcs = src/main.cpp\
			src/client.cpp\
			src/masterCube.cpp\
			src/server.cpp\

#objs = source file -> depending obj.
#alt:objs = client.o server.o masterCube.o main.o
objs = $(srcs:.cpp=.o)

#compiler commands
cc = g++

#compiler flags
#-g = adds debugging information to the executable file
cflags = -o

#linker flags
ldflags = -s

#cpp preprocessor flags
#-wall = turns on most, but not all, compiler warnings
cppflags=-Wall

############################
#### First Target: #########
$(bin): $(objs)
	$(cc) $(ldflags) -o $(bin) $(objs)

client.o: client.cpp masterCube.h
	g++ -o client.o -c masterCube.cpp

server.o: server.cpp masterCube.h
	g++ -o server.o -c masterCube.cpp

masterCube.o: masterCube.cpp masterCube.h
	g++ -o masterCube.o -c masterCube.cpp

main.o: main.cpp masterCube.h
		g++ -o main.cpp -c masterCube.cpp


#### Clean Target: #########
clean:
	rm -f $(bin) $(objs)
