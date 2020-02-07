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
//#include <bits/stdc++.h> 


#define RCVBUFSIZE 32   /* Size of receive buffer */
#define MAXPENDING 5


using namespace std;


class Cube
{

public:
		

	int X(int);
	int Y(int);
	int Z(int);

// Client
	void ReadFeedback();
	void FillQuestion();
	void AdjustQuestion();
	void FindSingleColor(int);
	void FindPosInPrio(int);
	void TopCrossQuestion();
	void TopCornerQuestion();
	void MiddleQuestion();
	void BottomQuestion();

	void ConnectToServer();

	int LoopArray(int * array);

	void ResetQuestion();

	void GenerateTransmissionString();

	void SendQuestion(bool bSendQuestion);

	void ReceiveAnswer(bool bGetFeedback);

	void CloseConnection();

	void CloseSocket();
	
	int getN();

	void clearCube();

void PrintArray();
void PrintVector(vector <int> &v);

bool DetectChange(int number);

void SendMoveCommand(bool sendVector);

void transmitData(bool bSendQuestion, bool bSendMoveCommand);

// Server
	void StartServer();

	void HandleTCPClient();

	void GiveFeedback();

	void ExecuteMoveCommands();
	bool StopServer();


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
	string moves = "";	// saves a list of performed moves
	vector <string> moveSingle;
	int n;
	int question[6][3][3];
	int sock;
 	char echoBuffer[6*3*3*sizeof(int)];
  int bytesRcvd, totalBytesRcvd;
  int auxNumber=0;
  
  int feedcnt=0;
  int feedcntOld=0;
  int Qcnt=0; // Question Position Counter
  int HitCnt=0;
  //vector <int> FAKEfeedbackVector;


  // Client
  int clientArray[6][3][3];
  vector<int> Pos; 
  vector<int> Col; 
  vector<int> feedbackVector;
  vector<char> moveCommandsChar;
  //vector<string> moveCommandsString;

  

  int testClient=34;
  
  

  // Server
  int serverArray[6][3][3];
  vector<int> positionVectorServer; 
  vector<int> colorVectorServer; 
  //int testServer=3;
  int messageSize=0;
  bool stopServer=false;
  

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
							{{1,1,0},{4,1,2}},
							{{1,1,2},{2,1,0}},
							{{2,1,2},{3,1,0}},
							{{3,1,2},{4,1,0}}
							};

	
	//CubeSolver-Stuff

	//string moves = "";	// saves a list of performed moves
	
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

	//Questions-Prioritys
 	int Prio[55] = {	
 					  1,  10,  12,  21, //  4 topcross edges 
					101, 110, 112, 121,
					201, 210, 212, 221,
					301, 310, 312, 321,
					401, 410, 412, 421,
					501, 510, 512, 521,	// 6sides x 4 edges = 24 Areas
					
					  0,   2,  20, 022, //  6sides x 4 corners = 24 Areas
					100, 102, 120, 122,
					200, 202, 220, 222,
					300, 302, 320, 322,
					400, 402, 420, 422,
					500, 502, 520, 522,

					 11, 111, 211, 311,	// useless middlepieces
					411, 511
					 
			};
	int PrioCnt=0; // saves our Position in the Priolist for filling the Question


 
};
