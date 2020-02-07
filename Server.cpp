#include "MasterCube.h"
#include <iostream>

using namespace std;

int main()
{
	cout<< "main from server" << endl;
	Cube myCube;
	myCube.scramble();
	cout<< "done mycube erstellen" << endl;
	myCube.print();

	myCube.StartServer();		// Server starten
	cout<< "done startserver" << endl;



	while(1){

		myCube.HandleTCPClient();	// Client abfragen
		cout<< "done handle tcp client" << endl;
		//myCube.GiveFeedback();
		
		myCube.print();
	    
	    //myCube.print();
		
		// Die Funktion GiveFeedback wird in der Funktion HandleTCPClient aufgerufen
		//myCube.GiveFeedback();
		//cout<< "done give GiveFeedback" << endl;
	}

		myCube.CloseSocket();
		cout << "socket closed " << endl;

}