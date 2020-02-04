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

#define c000 1 // Seite 0
#define c001 2
#define c002 3
#define c010 4
#define c011 5
#define c012 6
#define c020 7
#define c021 8
#define c022 9
#define c100 10 // Seite 1
#define c101 11
#define c102 12
#define c110 13
#define c111 14
#define c112 15
#define c120 16
#define c121 17
#define c122 18
#define c200 19 // Seite 2
#define c201 20
#define c202 21
#define c210 22
#define c211 23
#define c212 24
#define c220 25
#define c221 26
#define c222 27
#define c300 28 // Seite 3
#define c301 29
#define c302 30
#define c310 31
#define c311 32
#define c312 33
#define c320 34
#define c321 35
#define c322 36
#define c400 37 // Seite 4
#define c401 38
#define c402 39
#define c410 40
#define c411 41
#define c412 42
#define c420 43
#define c421 44
#define c422 45
#define c500 46 // Seite 5
#define c501 47
#define c502 48
#define c510 49
#define c511 50
#define c512 51
#define c520 52
#define c521 53
#define c522 54

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
