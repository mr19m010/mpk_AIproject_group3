#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <cmath>		/* for CubeSolver */
#include <time.h>		/* for randomizer in scramble();*/
#include <iostream>
#include <vector>



#define RCVBUFSIZE 32   /* Size of receive buffer */
#define MAXPENDING 5

using namespace std;


class Cube
{

public:


// Client

	void FillQuestion();
	void AdjustQuestion();
	void TopCrossQuestion();
	void TopCornerQuestion();
	void MiddleQuestion();
	void BottomQuestion();

	void ConnectToServer();

	int LoopArray(int * array);

	void ResetQuestion();

	void GenerateTransmissionString();

	void SendQuestion();

	void ReceiveAnswer();

	void CloseConnection();
	
	int getN();

	void clearCube();

void PrintArray();

bool DetectChange(int number);


// Server
	void StartServer();

	void HandleTCPClient();

	void GiveFeedback();


// CubeSolver-Stuff
	void r();
	void ri();

	void l();
	void li();

	void u();
	void ui();

	void d();
	void di();

	void f();
	void fi();

	void b();
	void bi();

	void copyCube(); // copys cube into old
	void print();      // prints cube

	void solveTopCross();
	void findNextTopEdge(int & s, int & p);
	void moveTopEdges();

	void solveTopCorners();
	void findNextTopCorner(int & s, int & p);
	void moveTopCorners();

	void scramble();
	void solveMiddleLayer();
	int findBottomMatch(int color);
	void placeMiddleEdge(int color, int edgeSide);
	void middleAlgorithm(int color, int direction);

	string last();      //gets the last move performed
	void removeLast(); //removes the last move performed
	void clearMoves(); //clears all moves


	void solveBottomLayer();
	void positionBottomCorners();
	int numCorrectCorners();
	void getCorner(int num, int corners[]);
	void swapCorners(int face);
	void positionBottomEdges();
	int numEdgesInCorrectPos();
	void rotateEdges(int face, bool rotClockwise);
	void correctBottomCorners();
	int cornerOrientation();
	int findBestFace(int cornerNum);
	void twoCornerRotate(int face, bool goForward);
	void correctBottomEdges();
	void twoEdgeRotate(int face, bool isNextTo);

void ChangeArray();

private:
	int n;
	int question[6][3][3];
	int sock;
 	char echoBuffer[6*3*3*sizeof(int)];
  int bytesRcvd, totalBytesRcvd;
  int auxNumber=0;
  vector <int> Pos;
  vector <int> Col;


  // Client
  int clientArray[6][3][3];
  vector<int> positionVectorClient; 
  vector<int> colorVectorClient; 
  vector<int> feedbackVector;
  int testClient=34;
  int n=10; // wird vom user gesetzt
  
  

  // Server
  int serverArray[6][3][3];
  vector<int> positionVectorServer; 
  vector<int> colorVectorServer; 
  int testServer=3;
  

  int servSock;                    /* Socket descriptor for server */
  int clntSock;                    /* Socket descriptor for client */
  struct sockaddr_in echoServAddr; /* Local address */
  struct sockaddr_in echoClntAddr; /* Client address */
  unsigned short echoServPort=10000;     /* Server port */
  unsigned int clntLen;            /* Length of client address data structure */
  int receivedArray[6][3][3];
  int number;

  	int ecken[8][3][3] = {
						{{0,0,0},{1,0,0},{4,0,2}},
						{{0,0,2},{3,0,2},{4,0,0}},
						{{0,2,0},{1,0,2},{2,0,0}},
						{{0,2,2},{2,0,2},{3,0,0}},
						{{5,0,0},{1,2,2},{2,2,0}},
						{{5,0,2},{2,2,2},{3,2,0}},
						{{5,2,0},{1,2,0},{4,2,2}},
						{{5,2,2},{3,2,2},{4,2,0}}
						};

	int kanten[12][2][3] = {{{0,0,1},{4,0,1}},
							{{0,1,0},{1,0,1}},
							{{0,1,2},{3,0,1}},
							{{0,2,1},{2,0,1}},
							{{5,0,1},{2,2,1}},
							{{5,1,0},{1,2,1}},
							{{5,1,2},{3,2,1}},
							{{5,2,1},{4,2,1}},
							{{1,1,0},{4,2,1}},
							{{1,1,2},{2,1,0}},
							{{2,1,2},{3,1,0}},
							{{3,1,2},{4,1,0}}
							};

	
	//CubeSolver-Stuff

	string moves = "";	// saves a list of performed moves
	
	int old[6][3][3] =
	{
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, //yellow side
    { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } }, //orange side
    { { 2, 2, 2 }, { 2, 2, 2 }, { 2, 2, 2 } }, //blue side
    { { 3, 3, 3 }, { 3, 3, 3 }, { 3, 3, 3 } }, //red side
    { { 4, 4, 4 }, { 4, 4, 4 }, { 4, 4, 4 } }, //green side
	};

	int cube[6][3][3] =
	{
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, //yellow side
    { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } }, //orange side
    { { 2, 2, 2 }, { 2, 2, 2 }, { 2, 2, 2 } }, //blue side
    { { 3, 3, 3 }, { 3, 3, 3 }, { 3, 3, 3 } }, //red side
    { { 4, 4, 4 }, { 4, 4, 4 }, { 4, 4, 4 } }, //green side
    { { 5, 5, 5 }, { 5, 5, 5 }, { 5, 5, 5 } }  //white side
	}; 

	int solved_cube[6][3][3] =
	{
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, //yellow side
    { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } }, //orange side
    { { 2, 2, 2 }, { 2, 2, 2 }, { 2, 2, 2 } }, //blue side
    { { 3, 3, 3 }, { 3, 3, 3 }, { 3, 3, 3 } }, //red side
    { { 4, 4, 4 }, { 4, 4, 4 }, { 4, 4, 4 } }, //green side
    { { 5, 5, 5 }, { 5, 5, 5 }, { 5, 5, 5 } }  //white side
	}; 

 
};
