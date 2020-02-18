#g++ -g -o server Server.cpp MasterCube.cpp
#g++ -g -o client Client.cpp MasterCube.cpp


#g++ -g -o main src/main.cpp src/masterCube.cpp src/client.cpp src/server.cpp
echo "compiling..."
g++ -pg -g -o main src/main.cpp src/masterCube.cpp src/client.cpp src/server.cpp
cp main server_exe/
cp main client_exe/


# Makefile muss noch geschrieben werden
# ./server
# ./client
