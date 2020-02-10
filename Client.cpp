#include "MasterCube.h"
#include <iostream>

using namespace std;

int main()
{
	
	


	Cube cCube; // Client-Cube
	cCube.clkStartAll = clock();
	cCube.clkStart = clock();
	cCube.SetClient();


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
	cCube.getN();
	while(1){
		if(cCube.restart==1){
			cCube.restart=0;
			cCube.scramble();
			cCube.clkStart = clock();
			cout << " ##################### RESTART #############"<<endl;
			cCube.restartCnt++;
		}	
		cCube.clearCube();
		cCube.print();
		//cout << " ##################### TopCrossQuestion #############"<<endl;
		cCube.TopCrossQuestion();
		//cCube.print();
		cout << " ##################### solveTopCross #############"<<endl;
		cCube.solveTopCross();
		cCube.print();
		//cout << " ##################### TopCornersQuestion #############"<<endl;
		cCube.TopCornersQuestion();
		//cCube.print();
		cout << " ##################### solveTopCorner #############"<<endl;
		cCube.solveTopCorners();
		cCube.print();
		//cout << " ##################### MiddleLayerQuestion #############"<<endl;
		//cCube.MiddleLayerQuestion();
		//cCube.print();
		cout << " ##################### solveMiddleLayer #############"<<endl;
		cCube.solveMiddleLayer();
		cCube.print();
		cout << " ##################### solveBottomLayer #############"<<endl;
		cCube.solveBottomLayer();
		cCube.print();
		cCube.clkFinish = clock();
		if(cCube.restart==0){
			cout <<"Time: " << (double)(cCube.clkFinish-cCube.clkStart)/CLOCKS_PER_SEC << endl;
			break;
		}
	}

  	//cout << "Verbindung mit Server hergestellt" << endl;
	
	cCube.CloseConnection();
	cCube.clkFinish = clock();
	cout << "RestartCnt: " << cCube.restartCnt << endl;
	cout <<"TimeAll: " << (double)(cCube.clkFinish-cCube.clkStartAll)/CLOCKS_PER_SEC << endl;
    exit(0);

}
