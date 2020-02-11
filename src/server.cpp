#include "masterCube.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc>=2){
		if (!strcmp(argv[1],"server")){
			cout << "start server >>" << endl;
		}
		else if (!strcmp(argv[1],"client")){
			cout << "start client >>" << endl;
		}
	}

	//cout<< "main from server" << endl;
	Cube myCube;
	myCube.SetServer();

	myCube.scramble();
	//cout<< "done mycube erstellen" << endl;
	myCube.print();

	myCube.StartServer();		// Server starten
	//cout<< "done startserver" << endl;



	//while(myCube.StopServer()==false){
	while(myCube.CheckCubeState()==false){

		myCube.HandleTCPClient();	// Client abfragen
		//cout<< "done handle tcp client" << endl;
		//myCube.GiveFeedback();

		//myCube.print();

		/*if(myCube.CheckCubeState()==true)
		{
			cout << endl << "Cube solved!" << endl;
		}
		else
		{
			cout << endl << "Cube NOT solved!" << endl;
		}*/

	    //myCube.print();

		// Die Funktion GiveFeedback wird in der Funktion HandleTCPClient aufgerufen
		//myCube.GiveFeedback();
		//cout<< "done give GiveFeedback" << endl;
	}


	myCube.print();

		myCube.CloseSocket();
		//cout << "socket closed " << endl;
}
