#include "masterCube.h"
#include <iostream>

using namespace std;

void SetupServer(){
    Cube myCube;
    myCube.SetServer();
    myCube.scramble();
    myCube.print();
    myCube.StartServer();		// Server starten
    while(myCube.CheckCubeState()==false){
        myCube.HandleTCPClient();	// Client abfragen
    }
    myCube.print();
    myCube.CloseSocket();
}

int SetupClient(){
    Cube cCube; // Client-Cube
    cCube.clkStartAll = clock();
    cCube.clkStart = clock();
    cCube.SetClient();

    if (cCube.ConnectToServer()==false) // Hier wird die Verbindung zum Server hergestellt
    {
        cout << "Connection to server failed" << endl;
        return -1;
    }
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
        
        if(cCube.IsSolved()==true){
            cout << "Cube is solved" << endl;
        }
        else{
            cout << "Something went wrong. Cube is not solved." << endl;
        }

        cCube.clkFinish = clock();
        if(cCube.restart==0){
            cout <<"Time: " << (double)(cCube.clkFinish-cCube.clkStart)/CLOCKS_PER_SEC << endl;
            break;
        }
    }
    cCube.CloseConnection();
    cCube.clkFinish = clock();
    cout << "RestartCnt: " << cCube.restartCnt << endl;
    cout <<"TimeAll: " << (double)(cCube.clkFinish-cCube.clkStartAll)/CLOCKS_PER_SEC << endl;
    exit(0);
}

int main()
{
	SetupClient();
    return 0;
    
}

