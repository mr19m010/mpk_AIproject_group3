#include "MasterCube.h"
#include <iostream>

using namespace std;

int main()
{
	cout<< "main from server" << endl;
	Cube myCube;
	myCube.scramble();
	cout<< "done mycube erstellen" << endl;

	myCube.StartServer();		// Server starten
	cout<< "done startserver" << endl;

	while(1){

		myCube.HandleTCPClient();	// Client abfragen
		cout<< "done handle tcp client" << endl;
		//myCube.GiveFeedback();
		
		//myCube.print();
	    
	    //myCube.print();
		myCube.GiveFeedback();
		cout<< "done give GiveFeedback" << endl;
	}

		myCube.CloseSocket();
		cout << "socket closed " << endl;

}