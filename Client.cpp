#include "MasterCube.h"
#include <iostream>

using namespace std;

int main()
{
	
	Cube cCube; // Client-Cube

	/*
	cCube.print();

	cCube.clearCube(); // delete teststuff from this function!

	cCube.print();

	

	cCube.print();

	cout << "Emptymoves: " << cCube.moves << endl;

	cCube.solveTopCross();


	cout << "hopefully full moves" << cCube.moves << endl;

	cCube.print();

	
	cCube.getN();
	cCube.TopCrossQuestion();
	*/

	cCube.getN();
	cCube.GenerateTransmissionString();
	
	//cout << "Client Cube wurde erstellt" << endl;
	cCube.ResetQuestion();
	//cout << "Test Vektor erstellt" << endl;
	cCube.ConnectToServer(); // Hier wird die Verbindung zum Server hergestellt
  	//cout << "Verbindung mit Server hergestellt" << endl;
	
	cCube.SendQuestion();
	
	cCube.SendMoveCommand(true);
	//cout << "Positions und Farbvektor gesendet" << endl;


	cCube.ReceiveAnswer();
	//cout << "Feedback empfangen" << endl;
	cCube.CloseConnection();
    exit(0);

}
