#makefile.am -> .am wird für aufruf $make weggelassen!
#$make -n for testing (shows actions without performing)

#extends search path to src/
VPATH = /src

### Variables: ###########

#bin = program name "Zieldatei"-> target
bin = masterCubeSolver

#srcs = all necessary files -> relative path to .cpp file
srcs = src/main.cpp\
			 src/masterCube.cpp\

#objs = source file -> depending obj.
#alt:objs = client.o server.o masterCube.o main.o
objs = $(srcs:.cpp=.o)

#compiler commands
cc = g++

#compiler flags
#-g = adds debugging information to the executable file
cflags = -o2 -g

#linker flags
ldflags = -s

#cpp preprocessor flags
#-wall = turns on most, but not all, compiler warnings
cppflags = -Wall

############################
#### First Target: #########
$(bin): $(objs)
	$(cc) $(ldflags) -o $(bin) $(objs)

main.o: main.cpp masterCube.h
	$(cc) $(cppflags) -o main.cpp -c masterCube.cpp

masterCube.o: masterCube.cpp masterCube.h
	$(cc) $(cppflags) -o masterCube.o -c masterCube.cpp

#### Clean Target: #########
clean:
	rm -f $(bin) $(objs)
