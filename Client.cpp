#include "MasterCube.h"
#include <iostream>

using namespace std;

int main()
{
	
	Cube cCube; // Client-Cube

	/*
	cCube.print();

	//cCube.clearCube(); // delete teststuff from this function!

    cCube.scramble();
	cCube.print();

	

	cout << "Emptymoves: " << cCube.moves << endl;

	cCube.solveTopCross();


	cout << "hopefully full moves" << cCube.moves << endl;

	cCube.print();

	
	cCube.getN();
	cCube.TopCrossQuestion();
	*/

	//cCube.GenerateTransmissionString();
	
	//cout << "Client Cube wurde erstellt" << endl;
	//cCube.ResetQuestion();
	//cout << "Test Vektor erstellt" << endl;
	cCube.ConnectToServer(); // Hier wird die Verbindung zum Server hergestellt

	cCube.clearCube();
	cCube.getN();
	cCube.print();
	cout << " ##################### TopCrossQuestion #############"<<endl;
	cCube.TopCrossQuestion();
	cCube.print();
	cout << " ##################### solveTopCross #############"<<endl;
	cCube.solveTopCross();
	cCube.print();
	
	cCube.FindSingleColor(212);
	cCube.print();




	//cCube.solveTopCross();
	//cCube.print();
	//cout << "TopCross: " << moves;
	

  	//cout << "Verbindung mit Server hergestellt" << endl;
	
	//cCube.SendQuestion();
	
	//cCube.SendMoveCommand(true);
	//cout << "Positions und Farbvektor gesendet" << endl;


	
	//cout << "Feedback empfangen" << endl;
	cCube.CloseConnection();
    exit(0);

}
