#include "masterCube.h"
#include "client.h"
#include "server.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    //cout << "Hello world!" << endl;
	if (argc>=2){
		if (!strcmp(argv[1],"server")){
			SetupServer();
		}
		else if (!strcmp(argv[1],"client")){
			SetupClient();
		}
        else {
            cout << "Wrong parameter. Choose 'server' or 'client' as parameter." << endl;
        }
	}	
    else
    {
        cout << "No parameter. Choose 'server' or 'client' as parameter." << endl;
        cout << "Let's try to start the client..." << endl;
        if(SetupClient()==-1){
            cout << endl;
            cout << "There is no server to connect to. Let's start it now..." << endl;
            cout << "Please open a second terminal and run the following command:" << endl;
            cout << ".\\main client or only .\\main if you are lazy" << endl;
            cout << endl;
            SetupServer();
        }
    }
    return 0;
}

