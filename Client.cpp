#include "MasterCube.h"
#include <iostream>

using namespace std;

int main()
{
	Cube cCube; // Client-Cube

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
	
	

	cCube.ResetQuestion();
	cCube.ConnectToServer(); // Hier wird die Verbindung zum Server hergestellt
  	cCube.SendQuestion();
	cCube.ReceiveAnswer();
	cCube.CloseConnection();
	

    exit(0);
}
