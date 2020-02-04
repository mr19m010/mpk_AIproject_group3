#include "MasterCube.h"
#include <iostream>

using namespace std;

int main()
{
	Cube cCube; // Client-Cube
	cCube.clearCube();

	cCube.print();

	
	cCube.getN();
	cCube.FillQuestion();
	for(int i=0;i<10;i++){
		cCube.TopCrossQuestion();
	}
	

	cCube.ResetQuestion();
	cCube.ConnectToServer(); // Hier wird die Verbindung zum Server hergestellt
  	cCube.SendQuestion();
	cCube.ReceiveAnswer();
	cCube.CloseConnection();
	

    exit(0);
}
