#g++ -g -o server Server.cpp MasterCube.cpp
#g++ -g -o client Client.cpp MasterCube.cpp


#g++ -g -o main src/main.cpp src/masterCube.cpp src/client.cpp src/server.cpp
g++ -pg -g -o main src/main.cpp src/masterCube.cpp src/client.cpp src/server.cpp

# Makefile muss noch geschrieben werden
# ./server
# ./client
