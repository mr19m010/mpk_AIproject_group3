#include "MasterCube.h"
#include <iostream>

using namespace std;

int main()
{
	Cube myCube;

	myCube.StartServer();		// Server starten
	myCube.HandleTCPClient();	// Client abfragen
	myCube.GiveFeedback();
	myCube.print();
    myCube.scramble();
    myCube.print();
	myCube.GiveFeedback();
    

}