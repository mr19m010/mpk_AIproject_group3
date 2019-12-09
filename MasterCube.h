#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#include <iostream>

#define RCVBUFSIZE 32   /* Size of receive buffer */
#define MAXPENDING 5

using namespace std;


class Cube
{

public:

// Client
void ConnectToServer();

int LoopArray(int * array);

void ResetQuestion();

void GenerateTransmissionString(int *quest);

void SendQuestion();

void ReceiveAnswer();

void CloseConnection();

// Server
void StartServer();

void HandleTCPClient();


private:
	int question[6][3][3];
	int sock;
	char echoBuffer[RCVBUFSIZE];    
    int bytesRcvd, totalBytesRcvd;  
	
	// Server
	int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned short echoServPort=10000;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */

};
