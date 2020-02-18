#include "masterCube.h"
#include "server.h"
#include <iostream>

using namespace std;

void SetupServer(){
    Cube myCube;
    myCube.SetServer();
    myCube.scramble();
    myCube.print();
    myCube.StartServer();		// Server starten
    while(myCube.CheckCubeState()==false){
        myCube.HandleTCPClient();	// Client abfragen
    }
    myCube.print();
    myCube.CloseSocket();
}