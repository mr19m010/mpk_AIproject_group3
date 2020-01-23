#include "MasterCube.h"
#include <iostream>

using namespace std;

int main()
{
	Cube myCube;
	
	myCube.getN();
	myCube.ResetQuestion();
	myCube.ConnectToServer(); // Hier wird die Verbindung zum Server hergestellt
  	myCube.SendQuestion();
	myCube.ReceiveAnswer();
	myCube.CloseConnection();
	

    exit(0);
}
