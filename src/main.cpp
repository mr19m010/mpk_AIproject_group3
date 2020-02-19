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
    }
    return 0;
}

