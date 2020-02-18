#g++ -g -o server Server.cpp MasterCube.cpp
#g++ -g -o client Client.cpp MasterCube.cpp


#g++ -g -o main src/main.cpp src/masterCube.cpp src/client.cpp src/server.cpp
echo "compiling..."
g++ -pg -g -o main src/main.cpp src/masterCube.cpp src/client.cpp src/server.cpp
mkdir -v -p server_exe
mkdir -v -p client_exe
cp main server_exe/
cp main client_exe/


# Makefile muss noch geschrieben werden
# ./server
# ./client
