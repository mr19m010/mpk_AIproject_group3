
#include "MasterCube.h"
#include <iostream>

using namespace std;

void Cube::l()
{

}

void Cube::ConnectToServer()
{
	struct sockaddr_in echoServAddr; /* Echo server address */

	cout << "Connecting to server"  << endl;


    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        printf("1");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr("127.0.0.1");   /* Server IP address */
    echoServAddr.sin_port        = htons(10000); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        printf("2");

}

int Cube::LoopArray(int * array)
{
	for(int x=5; x>=0; x--)
    {
        for(int y=2; y>=0; y--)
        {
            for(int z=2; z>=0; z--)
            {
                *(array+(x*6)+(y*3)+z)=0;
                cout << *(array+(x*6)+(y*3)+z) << " ";
            }
            cout << endl;
        }
        cout << "-----" << endl;
    }
}

void Cube::PrintArray()
{
    int counter=0;
    int x=5;
    int y=2;
    int z=2;

    for(int x=5; x>=0; x--)
	{
        
		for(int y=2; y>=0; y--)
		{   
             
			while(z>=0)
			{
                if (counter<3) {
                    cout << "x ";
                }
                else if (counter==3) cout << "|";
                else{
				    cout << receivedArray[x][y][z] << " ";
                    z--;
                }
                counter++;
			}
			cout << endl;
            counter=0;
            z=2;
		}
        
		cout << "-----" << endl;
	}
}


void Cube::ChangeArray()
{
    for(int x=5; x>=0; x--)
	{
		for(int y=2; y>=0; y--)
		{
			for(int z=2; z>=0; z--)
			{
				receivedArray[x][y][z]*=2;
			}
			cout << endl;
		}
		cout << "-----" << endl;
	}
}

void Cube::ResetQuestion()
{
	//cout << number << endl;
	for(int x=5; x>=0; x--)
	{
		for(int y=2; y>=0; y--)
		{
			for(int z=2; z>=0; z--)
			{
				//(question+(x*6)+(y*3)+z)=0;
				question[x][y][z]=2;
				cout << question[x][y][z] << " ";
			}
			cout << endl;
		}
		cout << "-----" << endl;
	}
}

void Cube::GenerateTransmissionString(int *quest)
{
	Cube::ResetQuestion();
}


void Cube::SendQuestion()
{
	if (send(sock, &question, 6*3*3*sizeof(int), 0) < 0)
		cout << "3" << endl;

}

void Cube::ReceiveAnswer()
{
    /* Receive the same string back from the server */
    totalBytesRcvd = 0;
    cout << "Received: " << endl;                /* Setup to print the echoed string */
    while (totalBytesRcvd < (6*3*3*sizeof(int)))
    {
        cout << "OK?" << endl;
        /* Receive up to the buffer size (minus 1 to leave space for
           a null terminator) bytes from the sender */
        if ((bytesRcvd = recv(sock, receivedArray, (6*3*3*sizeof(int)), 0)) <= 0)
			cout << "4" << endl;

        cout << "Bytes received = " << bytesRcvd << endl;
        totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
        cout << "Total bytes received = " << totalBytesRcvd << endl;
        echoBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
        Cube::PrintArray();
	}
}

void Cube::CloseConnection()
{
	close(sock);
}


// Server

void Cube::StartServer()
{
	/* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        cout << "1" << endl;

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        cout << "2" << endl;

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        cout << "3" << endl;

    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
                               &clntLen)) < 0)
            cout << "4" << endl;

        /* clntSock is connected to a client! */

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        Cube::HandleTCPClient();
        //Cube::GenerateFeedback();
        //Cube::SendFeedback();
    }
}

void Cube::HandleTCPClient()
{
    //char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int recvMsgSize;                    /* Size of received message */
	
    /* Receive message from client */
    if ((recvMsgSize = recv(clntSock, receivedArray, 6*3*3*sizeof(int), 0)) < 0)
        cout << "1" << endl;

    cout << "Receive Size = " << recvMsgSize << endl;
    cout << "Size of int = " << sizeof(int) << " -> 6*3*3 sizeof(int) = " 
    << 6*3*3*sizeof(int) << endl;

    Cube::ChangeArray();

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        cout << "Received message size = " << recvMsgSize << endl; 
        /* Echo message back to client */
        if (send(clntSock, &receivedArray, recvMsgSize, 0) != recvMsgSize)
            cout << "1" << endl;

        /* See if there is more data to receive */
        if ((recvMsgSize = recv(clntSock, receivedArray, 6*3*3*sizeof(int), 0)) < 0)
            cout << "1" << endl;
    }

    close(clntSock);    /* Close client socket */
}
