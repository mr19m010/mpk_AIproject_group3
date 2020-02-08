
#include "MasterCube.h"
#include <iostream>

using namespace std;

void Cube::SetServer()
{
    bServerActive=true;
}

void Cube::SetClient()
{
    bClientActive=true;
}

void Cube::getN()
{
	cout <<	"Please enter Questionlength between 2 and 53: " << endl;
	cin >> n;
    if(n<2 || n>53){
        cout << "Input was not correct, please try again."<<endl;
        getN();
    }
}

void Cube::ReadFeedback(){ // 1 == color ok; 0 == color && position OK; 2 == nothing OK 
    //cout << "IM IN ReadFeedback"<< endl;
    feedcnt=0;
    for(int i = 0;i<n;i++){
        if(feedbackVector[i]==0 || feedbackVector[i]==1){
            feedcnt++;
        }else if(feedbackVector[i]==2){
            // nothing happens with 2
        }else {
            cout << "ERROR:ReadFeedback():unknown feedbackVector content: "<< feedbackVector[i] << " (9 means no new feedbackVector awailaible)"<<endl;
            feedcnt=-1;
            break;
        }
    }
    //FAKEfeedbackVector.assign(n,9); // writes n times 9 into vector to make sure we dont read the same feedbackvector twice
    //cout << "Feedcnt= "<< feedcnt<< endl;
    //cout << "feedbackVector: ";
    //PrintVector(feedbackVector);
}

void Cube::FillQuestion(){
    //cout << "IM IN FillQuestion"<< endl;
    int TmpPrioCnt=0;
    int k=0;   
    Qcnt=0;
    Pos.clear();
    Col.clear();
    for(int i=PrioCnt;i<n+PrioCnt;i++){        
             

            if(i>=54){ // this is for filling the Question, when there is no more stuff in the Priolist -> we fill from start again
                Pos.push_back(Prio[k++]);           // Fill Question with Prio List from start
                Col.push_back(5);                   // Write Color = 5 ("nicht yellow")    
            }else {
                Pos.push_back(Prio[i]);             // Fill Question with Prio List
                Col.push_back(5);                   // Write Color = 5 ("nicht yellow")    
                TmpPrioCnt++; 
            }


             
            //cout << "Pos: " <<Pos[i] << " Col: " <<Col[i]<<endl;    
            //cout << "Prio: " << Prio[i] << endl;
        }

   // cout << "Pos after FillQuestion: ";
    //PrintVector(Pos);

    PrioCnt+=TmpPrioCnt;
    // We need this to generate feedcntOld and to start with a meaningful Question in AdjustQuestion (where a Pos gets asked "0"?)    
    
    transmitData(true,false); // Parameter 1 -> Question+Feedback; Parameter 2 -> MoveCommand
    ReadFeedback();
    feedcntOld=feedcnt;
    Col[Qcnt]=0; // Write Color = "Yellow"
    transmitData(true,false);

    //PrintVector(Pos);
    //PrintVector(Col);

    } 

void Cube::AdjustQuestion(){
    //cout << "IM IN AdjustQuestion"<< endl;
    ReadFeedback();
    if(feedcnt!=-1){
        if(feedcntOld<feedcnt){ // we hit something good
            cube[X(Pos[Qcnt])][Y(Pos[Qcnt])][Z(Pos[Qcnt])]=0;
            //cout << "Feedback groesser, Qcnt: " << Qcnt << " Cube"<<X(Pos[Qcnt])<<Y(Pos[Qcnt])<<Z(Pos[Qcnt])<<"="<<cube[X(Pos[Qcnt])][Y(Pos[Qcnt])][Z(Pos[Qcnt])]<< " - Hit!"<<endl;
            Qcnt++;
            Col[Qcnt]=0; // Write Color = "Yellow"
            HitCnt++;
        }else if(feedcntOld>feedcnt){
            cube[X(Pos[Qcnt])][Y(Pos[Qcnt])][Z(Pos[Qcnt])]=5; // Stuff is white - we save that for later
            //cout << "Feedback kleiner, Qcnt: " << Qcnt << " Cube"<<X(Pos[Qcnt])<<Y(Pos[Qcnt])<<Z(Pos[Qcnt])<<"="<<cube[X(Pos[Qcnt])][Y(Pos[Qcnt])][Z(Pos[Qcnt])] << endl;
            Qcnt++;
            Col[Qcnt]=0; // Write Color = "Yellow"
        } else if(feedcntOld==feedcnt){ // no hit, not yellow or white
            //cout << "Feedback gleich, Qcnt: " << Qcnt << " Cube"<<X(Pos[Qcnt])<<Y(Pos[Qcnt])<<Z(Pos[Qcnt])<<"="<<cube[X(Pos[Qcnt])][Y(Pos[Qcnt])][Z(Pos[Qcnt])]<< endl;
            Qcnt++;
            Col[Qcnt]=0; // Write Color = "Yellow"            
        }
        feedcntOld=feedcnt;
        if(Qcnt>=n){         // if the Question runs out of new Positions to ask, we fill it again with new stuff from the Priolist
            FillQuestion();
        }
    }else {
        //do we need to do anything, if we didnt get a new feedbackvector? Send Question again?
        transmitData(true,false);
    }
}
void Cube::FindPosInPrio(int facePos){
    for(int i=0; i<sizeof(Prio)/sizeof(Prio[0]); ++i){
        if(Prio[i]==facePos){
            PrioCnt=i;
            //cout << "PrioCnt: "<<PrioCnt<<endl;
            break;
        } 
    }   
}

void Cube::FindSingleColor(int facePos){
    if(cube[X(facePos)][Y(facePos)][Z(facePos)]!=9) {
        //cout<<"Farbe bereits bekannt."<<endl;
        return; // return if we know color already
    }

    FindPosInPrio(facePos);
    FillQuestion();
    //cout << "im in FindSingleColor and have filled my Pos, which is: ";
    //PrintVector(Pos);

    while(1){
        transmitData(true,false);
        ReadFeedback();
        //cout << "ANFANGS IST DIE FARBE: " << Col[Qcnt] << endl;
        // We dont care about Qcnt in this Function at all - we only want to know Col of Qcnt == 0

        if(feedcnt!=-1){
            if(feedcntOld<feedcnt){ // we hit something good
                //cout<<"feedback groesser - Col[Qcnt] = "<<Col[Qcnt]<<endl;
                cube[X(Pos[Qcnt])][Y(Pos[Qcnt])][Z(Pos[Qcnt])]=Col[Qcnt];
                //cout << "Feedback groesser, Qcnt: " << Qcnt << " Cube"<<X(Pos[Qcnt])<<Y(Pos[Qcnt])<<Z(Pos[Qcnt])<<"="<<cube[X(Pos[Qcnt])][Y(Pos[Qcnt])][Z(Pos[Qcnt])]<<endl;
                //PrintVector(feedbackVector);

                return;
            }else if(feedcntOld>feedcnt){
               // cout<<"feedback kleiner - Col[Qcnt] = "<<Col[Qcnt]<< " should be 0, but we wrote 5 because we know its white"<<endl;
                cube[X(Pos[Qcnt])][Y(Pos[Qcnt])][Z(Pos[Qcnt])]=5; // Stuff is white - we save that for later
                //cout << "Feedback groesser, Qcnt: " << Qcnt << " Cube"<<X(Pos[Qcnt])<<Y(Pos[Qcnt])<<Z(Pos[Qcnt])<<"="<<cube[X(Pos[Qcnt])][Y(Pos[Qcnt])][Z(Pos[Qcnt])]<<endl;
                //PrintVector(feedbackVector);


                return;
            } else if(feedcntOld==feedcnt){ // no hit, not yellow or white
                //cout<<"feedback gleich - Col[Qcnt] = "<< Col[Qcnt]<< " - Die Farbe wars net."<<endl;
                //cout << "Feedback gleich, Qcnt: " << Qcnt << endl;
                if(Col[Qcnt]<5){
                    Col[Qcnt]++; // Write next Color"
                    //cout << "DIE JETZIGE FARBE IST: " << Col[Qcnt] << endl;
                }else {
                    cout << "Error - FindSingleColor: We looped through all colors and found nothing."<<endl;
                }    
            }
            feedcntOld=feedcnt;
            /*if(Qcnt>n){         // if the Question runs out of new Positions to ask, we fill it again with new stuff from the Priolist
                FillQuestion();
            }*/
        }else { // this is in case there is an error
            //do we need to do anything, if we didnt get a new feedbackvector? Send Question again?
            transmitData(true,false);
        }
    }
}


void Cube::TopCrossQuestion(){
    //cout << "IM IN TopCrossQuestion"<< endl;
    FillQuestion();
    while(HitCnt<4){
        AdjustQuestion();
        transmitData(true,false);
    }
   // cout << "Found 4 Yellow edges"<<endl;
    HitCnt=0; // Reset HitCnt for next Question-Set

}

void Cube::TopCornersQuestion(){
    



    PrioCnt=24; // So our FillQuestion knows to start in the Priolist at the Corners
    FillQuestion();
    while(HitCnt<4){
        AdjustQuestion();
        transmitData(true,false);
    }
   // cout << "Found 4 Yellow corners"<<endl;
    HitCnt=0; // Reset HitCnt for next Question-Set

};


/*void Cube::MiddleLayerQuestion(){


FindSingleColor(110);
FindSingleColor(112);
FindSingleColor(210);
FindSingleColor(212);
FindSingleColor(310);
FindSingleColor(312);
FindSingleColor(410);
FindSingleColor(412);
FindSingleColor(121);
FindSingleColor(221);
FindSingleColor(321);
FindSingleColor(421);
FindSingleColor(501);
FindSingleColor(510);
FindSingleColor(512);
FindSingleColor(521);

};*/

/*void Cube::BottomLayerQuestion(){

};*/

void Cube::clearCube(){ // writes 9 into every unknown face of the cube
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 3; j++){
            for (int k = 0; k < 3; k++){
                if(j!= 1 || k!=1)
                {
                    cube[i][j][k]=9;
                }
            }
        }
    }

    // THIS IS FOR TESTING ONLY
    /*cube[0][1][0]=0;
    cube[0][1][2]=0;
    cube[5][0][1]=0;
    cube[5][2][1]=0;
    cube[2][1][0]=0;
    cube[2][1][2]=0;
    cube[2][0][1]=0;
    cube[2][2][1]=0;*/
};

int Cube::X(int Pos){ // Hier wird die Positionsinformation auf x y z aufgeteilt
    int x = (Pos/100)%10;
    return x;
}
int Cube::Y(int Pos){
    int y = (Pos/10)%10;
    return y;
}
int Cube::Z(int Pos){
    int z = Pos%10;
    return z;
}
void Cube::PrintVector(vector <int> &v){
    cout << "PrintVector(): ";
    for(int i=0; i<v.size(); ++i){
        cout << v[i] << " ";   
    }   
    cout << endl;
}

void Cube::ConnectToServer()
{
	struct sockaddr_in echoServAddr; /* Echo server address */

	//cout << "Connecting to server"  << endl;


    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        cout << "Error - Socket" << endl;


    //cout << "Socket = " << sock << endl;
    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr("127.0.0.1");   /* Server IP address */
    echoServAddr.sin_port        = htons(10000); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        cout << "Error - Verbindung mit Server konnte nicht hergestellt werden." << endl;

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

bool Cube::DetectChange(int number)
{
    bool changed=false;

    if (number!=auxNumber){
        auxNumber=number;
        changed=true;
    }
    else changed=false;

    return changed;
}

void Cube::PrintArray()
{
    int z=0;
    int y=0;
    int x=0;
    for (int rows = 0; rows <= 8; rows++)
    {
        for (int cols = 0; cols <=11; cols++)
        {
            if (((rows<3 || rows>5) && (cols<3 || cols>5)))
            {
                z=0;
                cout << "# ";
            }
            else if (rows>=3 && rows<=5)
            {
                x=((cols+3)/3);
                if (Cube::DetectChange(x)==true) z=0;
                cout << clientArray[x][y][z] << " ";
                //cout << z << " ";
                z++;
                
            }
            else if (cols>=3 && cols<=5)
            {   
                x=((rows)/3)*5/2;
                cout << clientArray[x][y][z] << " ";
                //cout << z << " ";
                z++;
            }
            
        }
        cout << endl;
        y++;
        if (y>2) y=0;
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
        }
    }
}

void Cube::ResetQuestion() // Diese Funktion ist ALT und kann gelöscht werden
{
    int cntNumber=9;
	//cout << number << endl;
    for(int x=5; x>=0; x--)
    {
      for(int y=2; y>=0; y--)
      {
            for(int z=2; z>=0; z--)
            {
				//(question+(x*6)+(y*3)+z)=0;
            question[x][y][z]=cntNumber--;
            if (cntNumber<1) cntNumber=9;
				//cout << question[x][y][z] << " ";
            }
			//cout << endl;
        }
		//cout << "-----" << endl;
    }
}

void Cube::GenerateTransmissionString() // Diese Funktion dient zum Testen des Feedbacks
{
    Pos.resize(n);
    Pos[0]=11;
    Pos[1]=110;
    Pos[2]=222;
    Pos[3]=300;
    Pos[4]=401;
    Pos[5]=511;
    Pos[6]=20;
    Pos[7]=21;
    Pos[8]=11;
    Pos[9]=0;
    Pos[10]=12;
    Pos[11]=21;
    Pos[12]=112;
    Pos[13]=521;
    Pos[14]=510;
    Pos[15]=520;
    Pos[16]=400;
    Pos[17]=401;
    Pos[18]=402;
    Pos[19]=410;
    Pos[20]=412;
    Pos[21]=420;
    Pos[22]=421;
    Pos[23]=422;
    Pos[24]=300;
    Pos[25]=301;
    Pos[26]=302;
    Pos[27]=310;
    Pos[28]=312;
    Pos[29]=320;
    Pos[30]=321;
    
    Col.resize(n);


    //cout << "Vector size = " << Pos.size()*sizeof(int) << endl;
    //cout << "Vector capazity = " << Pos.capacity()*sizeof(int) << endl;
}

void Cube::transmitData(bool bSendQuestion, bool bSendMoveCommand)
{
    if((bClientActive==true) && (bServerActive==true))
    {
        cout << "ERROR -> You can't be server and client at the same time" << endl;
    }
    else if(bClientActive==true)
    {
        SendQuestion(bSendQuestion);
        SendMoveCommand(bSendMoveCommand);
        ReceiveAnswer(bSendQuestion);
    }
    else if (bServerActive==true)
    {
        //cout << "INFO -> transmitData can only be executed by the client" << endl;
    }
    else
    {
        cout << "ERROR -> this should not happen" << endl;
    }
    
    
}

void Cube::SendQuestion(bool bSendQuestion)
{
    //cout << "IM IN SendQuestion"<< endl;
    // Die Vectorelemente werden einzeln übertragen
    // Zuerst wird die Anzahl der Elemente übertragen,
    // damit der Server weiß wieviele Elemente noch folgen
    int transmissionSize=0;

    
    if (bSendQuestion==true)
    {
        transmissionSize=Pos.size();

        if (send(sock, &transmissionSize, sizeof(int), 0) < 0)
            cout << "error - Paketlaenge konnte nicht gesendet werden." << endl;

        if (send(sock, &Pos[0], Pos.size()*sizeof(int), 0) < 0)
            cout << "error - Vector konnte nicht uebertragen werden." << endl;

        if (send(sock, &Col[0], Col.size()*sizeof(int), 0) < 0)
            cout << "error - Vector konnte nicht uebertragen werden." << endl;

    }
    else
    {
        transmissionSize=0;

        if (send(sock, &transmissionSize, sizeof(int), 0) < 0)
            cout << "error - Paketlaenge konnte nicht gesendet werden." << endl;
    }
    
}

void Cube::SendMoveCommand(bool bSendMoveCommand)
{
    int transmissionSize =0;
    vector<char> moveCommandsChar;

    //moveSingle.resize(10);
    /*moveCommandsString[0]="r";
    moveCommandsString[1]="ri";
    moveCommandsString[2]="l";
    moveCommandsString[3]="li";
    moveCommandsString[4]="d";
    moveCommandsString[5]="di";
    moveCommandsString[6]="ui";
    moveCommandsString[7]="u";*/


    for(int i=0; i<moveSingle.size();i++)
    {
        //cout << endl << "MoveSingle = " << moveSingle[i] << endl;
        copy(moveSingle[i].begin(), moveSingle[i].end(), back_inserter(moveCommandsChar));
    }

    //printVector(moveSingle);
    /*for(int i=0; i<moveCommandsChar.size();i++)exe
    {
        cout << moveCommandsChar[i] << endl;
    }*/
    
    
    if (bSendMoveCommand==true)
    {
        transmissionSize=moveCommandsChar.size();
       // cout << endl << "Paketgröße von MoveCommand = " << moveCommandsChar.size()*sizeof(char) << endl;

        if (send(sock, &transmissionSize, sizeof(int), 0) < 0)
            cout << "error - Paketlaenge konnte nicht gesendet werden." << endl;

        if (send(sock, &moveCommandsChar[0], moveCommandsChar.size()*sizeof(char), 0) < 0)
            cout << "error - Vector konnte nicht uebertragen werden." << endl;

        //GetAcknowledge();
    }
    else
    {
        transmissionSize=0;
        
        if (send(sock, &transmissionSize, sizeof(int), 0) < 0)
            cout << "error - Paketlaenge konnte nicht gesendet werden." << endl;
    }
    

    
}

void Cube::GiveFeedback()
{
    /*
    Schritt 1:  Positionen aus Vektor abrufen
    Schritt 2:  Farbe der Position überprüfen
    Schritt 3:  Prüfen ob die Position eine Ecke, Kante oder Mitte ist
    Schritt 4:  Wenn Nachbarelemente vorhanden sind, wird auch deren Farbinformation überprüft
    Schritt 5:  Nachdem die Positionen überprüft wurden, wird das Feedback randomisiert in einen Vektor
                geschrieben welcher an den Client übertragen wird
    */
    int countQuestions=0;
    vector<int> answerArray;
    vector<int> randArray;

    //cout << "Paketlänge der Anwort  = " << messageSize << endl;

    answerArray.resize(messageSize);
    randArray.resize(messageSize); 

    srand (time(NULL)); 

    int idX[3]={0,0,0};
    int idY[3]={0,0,0};
    int idZ[3]={0,0,0};
    
    // Schritt 1   
    for (int i=0; i<positionVectorServer.size(); i++)
    {
        answerArray[i]=0;
        //cout << "-----------------------------"<< endl;
        
        // Hier wird die Positionsinformation auf x y z aufgeteilt
        int x = positionVectorServer[i]/100;
        int y = (positionVectorServer[i]-(x*100))/10;
        int z = positionVectorServer[i]-(x*100)-(y*10);

        // Schritt 2
        if (colorVectorServer[i] == cube[x][y][z])
        {
            //cout << "OK -> Die Farben stimmen überein." << endl 
            //<< "Jetzt muss nur noch die Position auf ihre Richtigkeit überprüft werden." << endl;

            // Schritt 3
            // Prüfen ob Ecke
            if ((y!=1) & (z!=1)) 
            {
                // suche ID in ecken array
                // Hier werden die zugehörigen Nachbarseiten der Ecke gesucht
                //cout << "ECKE -> " << colorVectorServer[i] << " -> " << positionVectorServer[i] << endl;

                // Schleife 2/3 -> Nachbarseiten der Ecke überprüfen
                for (int a=7;a>=0;a--){         // a -> Es gibt 8 Ecken
                    for (int b=2;b>=0;b--){     // b -> Eine Ecke besteht aus drei Elementen (Nachbarseiten)
                        
                        // Hier wird überprüft welche Ecke gefragt wurde
                        if ( (ecken[a][b][0]==x) & (ecken[a][b][1]==y) & (ecken[a][b][2]==z)) 
                        {
                            //cout << "Ecken ID -> " << a << "/" << b << endl;

                            // Hier werden die Koordinaten von den 3 Eckseiten abgerufen
                            idX[0]=ecken[a][0][0];
                            idX[1]=ecken[a][1][0];
                            idX[2]=ecken[a][2][0];
                            
                            idY[0]=ecken[a][0][1];
                            idY[1]=ecken[a][1][1];
                            idY[2]=ecken[a][2][1];
                            
                            idZ[0]=ecken[a][0][2];
                            idZ[1]=ecken[a][1][2];
                            idZ[2]=ecken[a][2][2];
                            
                            //cout << "Nachbar elemente -> "  << idX[0] << idY[0] << idZ[0] << " ; "
                            ///<< idX[1] << idY[1] << idZ[1] <<  " ; "
                            //<< idX[2] << idY[2] << idZ[2]  << endl;
                            
                            // An dieser Stelle werden alle Farben der Ecke auf ihre Richtigkeit überprüft
                            // Schritt 4
                            bool eckeRichtig=true;
                            for (int j=2; j>=0; j--)
                            {
                                if ((cube[idX[j]][idY[j]][idZ[j]] == cube[idX[j]][1][1]))
                                {
                                    ;
                                }
                                else
                                {
                                    eckeRichtig=false;
                                }
                                if (eckeRichtig==true){
                                    //cout << "ECKE RICHTIG"<< endl;
                                    answerArray[i]=1;
                                    randArray[i]=rand();
                                } 
                                else {
                                    //cout << "ECKE FALSCH"<< endl;
                                    randArray[i]=rand();
                                }
                            }
                        }
                    }
                }
            }
            // Prüfe ob Kante
            if (((y==1) | (z==1)) & !(y==1 & z==1))
            {
                //cout << "KANTE -> " << colorVectorServer[i] << " -> " << positionVectorServer[i] << endl;
                // suche ID in kanten array
                for (int a=11;a>=0;a--){
                    for (int b=1;b>=0;b--){
                        if ( (kanten[a][b][0]==x) & (kanten[a][b][1]==y) & (kanten[a][b][2]==z) ) 
                        {
                            idX[0]=kanten[a][0][0];
                            idX[1]=kanten[a][1][0];

                            idY[0]=kanten[a][0][1];
                            idY[1]=kanten[a][1][1];

                            idZ[0]=kanten[a][0][2];
                            idZ[1]=kanten[a][1][2];

                            //cout << "Nachbar elemente -> "  << idX[0] << idY[0] << idZ[0] << " ; "
                            //<< idX[1] << idY[1] << idZ[1]  << endl;


                            // Prüfen ob sich die Kantenelemente auf der richtigen Seite befinden
                            // Schritt 4
                            bool kanteRichtig=true;
                            for (int j=1; j>=0; j--)
                            {
                                if ((cube[idX[j]][idY[j]][idZ[j]] == cube[idX[j]][1][1])) 
                                {
                                    ;
                                }
                                else {
                                    kanteRichtig=false;
                                }
                                if (kanteRichtig==true){
                                    //cout << "KANTE RICHTIG"<< endl;
                                    answerArray[i]=1;
                                    randArray[i]=rand();
                                } 
                                else {
                                    //cout << "KANTE FALSCH"<< endl;
                                    randArray[i]=rand();
                                }
                            }
                        }
                    }
                }
            }
            // Prüfen ob Mitte
            if (y==1 & z==1)
            {
                //cout << "MITTE RICHTIG" << endl;
                answerArray[i]=1;
                randArray[i]=rand();
            }
            else{
                ;
            }
        }
        else {
            //cout << "Die gefragte Farbe ist falsch." << endl;
            answerArray[i]=2;
            randArray[i]=rand();
        }
    }

    /*
    // Schritt 5
    FEEDBACK GENERIERT PRÜFEN OB RICHTIG
    das feedback muss an dieser stelle noch randomisiert werden
    */
    //cout << "Paketlänge der Anwort  = " << answerArray.size() << endl;

    bool switched=false;

    int testVar;
    //cin >> testVar;
    
    do{
        switched=false;
        for (int j=(messageSize-1); j>0; j--)
        {
            int merkerR=0;
            int merkerA=0;

            // Der größe nach ordnen
            if (randArray[j]<=randArray[j-1]){
                //cout << "OK -> " << randArray[j] << "/" << randArray[j-1]<<endl;
                //cout << "OK -> " << answerArray[j] << "/" << answerArray[j-1]<<endl;
            }
            else{
                merkerR=randArray[j];
                randArray[j]=randArray[j-1];
                randArray[j-1]=merkerR;

                merkerA=answerArray[j];
                answerArray[j]=answerArray[j-1];
                answerArray[j-1]=merkerA;

                switched=true;
                //cout << "switched -> " << randArray[j] << "/" << randArray[j-1]<<endl;
                //cout << "switched -> " << answerArray[j] << "/" << answerArray[j-1]<<endl;
            }
            //cout << endl << "Counter = " << j << endl;
        }
        //cout << " | " << endl;
        //cin >> testVar;
    }while(switched==true);
    //cout << endl;

    //cout << "AnswerArray = " << answerArray.size() << endl;

    if (send(clntSock, &answerArray[0], answerArray.size()*sizeof(int), 0) < 0)
        cout << "ERROR - Feedback konnte nicht gesendet werden." << endl;


}

void Cube::GetAcknowledge()
{   int recvMsgSize=0;
    if ((recvMsgSize = recv(clntSock, &bServerReady, sizeof(bool), 0)) < 0)
            cout << "ERROR -> bServerReady konnte nicht empfangen werden" << endl;
}

void Cube::SendAcknowledge()
{
    bool ready=true;
    if (send(clntSock, &ready, sizeof(bool), 0) < 0)
        cout << "ERROR - ready konnte nicht gesendet werden." << endl;
}

void Cube::ReceiveAnswer(bool bReceiceFeedback)
{
    //cout << "IM IN ReceiveAnswer"<< endl;
    int recvMsgSize;
    feedbackVector.resize(n);

    if(bReceiceFeedback==true)
    {
        if ((recvMsgSize = recv(sock, &feedbackVector[0], feedbackVector.size()*sizeof(int), 0)) < 0)
        {
            cout << "ERROR -> Feedback konnte nicht empfangen werden." << endl;
        }
        else{
            /*for(int i=0; i<feedbackVector.size(); i++){
                cout << feedbackVector[i] << " " ;
            }
            cout << endl;*/
        }
    }
    else
    {
        //cout << endl << "INFO -> Feedback wurde nicht abgerufen da keine Frage gestellt wurde." << endl;
        //cout << "Wenn diese Meldung auf der Serverseite erscheint haben wir ein Problem" << endl;
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
            cout << "ERROR -> Socket konnte nicht erstellt werden" << endl;

    int enable = 1;
    if (setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        cout << "ERROR -> setsockopt(SO_REUSEADDR) failed" << endl;

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
        if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
            cout << "ERROR -> Bind to local address failed" << endl;

    /* Mark the socket so it will listen for incoming connections */
        if (listen(servSock, MAXPENDING) < 0)
            cout << "ERROR -> Listening for incoming connections failed" << endl;

    /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

    /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
            &clntLen)) < 0)
            cout << "ERROR -> Connection with client failed" << endl;

    /* clntSock is connected to a client! */
        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

    }

    void Cube::HandleTCPClient()
    {
    int recvMsgSize;                    /* Size of received message */
    int packageSizeQuestion=0;
    int packageSizeMoveCommand=0;
    int messageCounter=0;

    stopServer=true;

    

    // Empfangen der Länge des Fragepakets
        if ((recvMsgSize = recv(clntSock, &packageSizeQuestion, sizeof(int), 0)) < 0)
            cout << "ERROR -> Paketlänge der Frage konnte nicht empfangen werden" << endl;

        messageSize=packageSizeQuestion; // auf "globale Variable schreiben damit die Feedback funktion die richte Größe kennt"
        // CM -> kann sein, dass ich das noch anders löse

       // cout << "Paketlänge der Frage  = " << packageSizeQuestion << endl;

        if (packageSizeQuestion>0){
            stopServer=false;
            positionVectorServer.resize(packageSizeQuestion);
            colorVectorServer.resize(packageSizeQuestion);

            // Empfangen der Frage
            if ((recvMsgSize = recv(clntSock, &positionVectorServer[0], positionVectorServer.size()*sizeof(int), 0)) < 0)
                cout << "ERROR -> Positionsvektor konnte nicht empfangen werden" << endl;

            if ((recvMsgSize = recv(clntSock, &colorVectorServer[0], colorVectorServer.size()*sizeof(int), 0)) < 0)
                cout << "ERROR -> Farbvektor konnte nicht empfangen werden" << endl;
            
            //cout << "recvMsgSize" << recvMsgSize << endl;
            //cout << "clntSock" << clntSock << endl;
            //cout << "&colorVectorServer[0]" << &colorVectorServer[0] << endl;

            //cout << "Vektorgröße pos -> " << positionVectorServer.size() << endl; 
            //cout << "Vektorgröße color -> " << colorVectorServer.size() << endl;
        }

        //Empfangen der Länge des MoveCommandpakets
        if ((recvMsgSize = recv(clntSock, &packageSizeMoveCommand, sizeof(int), 0)) < 0)
            cout << "ERROR -> Paketlänge des MoveCommands konnte nicht empfangen werden" << endl;

        if (packageSizeMoveCommand>0)
        {
            stopServer=false;
            vector<char> moveCommandsChar;
            moveCommandsChar.resize(packageSizeMoveCommand);

            //cout << "Paketlänge des MoveCommands  = " << packageSizeMoveCommand << endl;
            if ((recvMsgSize = recv(clntSock, &moveCommandsChar[0], moveCommandsChar.size()*sizeof(char), 0)) < 0)
                cout << "ERROR -> MoveCommandVektor konnte nicht empfangen werden" << endl;

            int i=0;
            int j=-1; //-1 so we can do j++ always at the start
            moveSingle.clear();        

           // cout << "moveCommandsChar= "; // printing the transmitted Chars
            /*for(int i=0; i<moveCommandsChar.size();i++)
            {
                cout << moveCommandsChar[i];
            }
            cout << endl;*/

            while(i<moveCommandsChar.size()){ 
                if(moveCommandsChar[i]=='i' && i!=0){
                    moveSingle[j] += moveCommandsChar[i];
                    i++;
                }else {
                    j++;
                    moveSingle.push_back("");               //expanding the vector
                    moveSingle[j] += moveCommandsChar[i];
                    i++;
                }
            }

            //cout << "moveSingle[i]: "; //printing the rebuild commands from the vector
            /*for(int i=0; i<moveSingle.size();i++)
            {
                cout << moveSingle[i] << endl;
            }*/

            ExecuteMoveCommands();
            //SendAcknowledge();
            
        }

        if (packageSizeQuestion>0)
        {
            GiveFeedback();
        }

        


    //close(clntSock);    /* Close client socket */

    }

    bool Cube::StopServer()
    {
        return stopServer;
    }

  
    void Cube::ExecuteMoveCommands()
    {
        //cout << endl << "Execute MoveCommands..." << endl;
        for(int i=0; i<(moveSingle.size());i++)
        {
            if (moveSingle[i]=="r")         r();
            else if (moveSingle[i]=="ri")   ri();
            else if (moveSingle[i]=="r2")   {r();r();}
            else if (moveSingle[i]=="l")    l();
            else if (moveSingle[i]=="li")   li();
            else if (moveSingle[i]=="l2")   {l();l();}
            else if (moveSingle[i]=="u")    u();
            else if (moveSingle[i]=="ui")   ui();
            else if (moveSingle[i]=="u2")   {u();u();}
            else if (moveSingle[i]=="d")    d();
            else if (moveSingle[i]=="di")   di();
            else if (moveSingle[i]=="d2")   {d();d();}
            else if (moveSingle[i]=="f")    f();
            else if (moveSingle[i]=="fi")   fi();
            else if (moveSingle[i]=="f2")   {f();f();}
            else if (moveSingle[i]=="b")    b();
            else if (moveSingle[i]=="bi")   bi();
            else if (moveSingle[i]=="b2")   {b();b();}
            else cout << "ERROR -> MoveCommand konnte nicht ausgeführt werden" << endl;

           // cout << endl << "MoveCommand = " << moveSingle[i] << endl;
            
        }
    }

    void Cube::CloseSocket()
    {
        close(clntSock);    /* Close client socket */
        //close(servSock);
    }


/*
#############################################################################
#############################################################################
################# SOLVER-STUFF ##############################################
#############################################################################
#############################################################################
*/


    void Cube::r()
    {
        if (last() == "r  ")
        {
            removeLast();
            moves += "r2 ";
        }
        else if (last() == "r2 ")
        {
            removeLast();
            moves += "ri ";
        }
        else if (last() == "ri ")
        {
            removeLast();
        }
        else
        {
            moves += "r  ";
        }

        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[2][i][2] = old[5][i][2];
            cube[0][i][2] = old[2][i][2];
            cube[4][i][0] = old[0][2 - i][2];
            cube[5][i][2] = old[4][2 - i][0];
        }
        cube[3][0][2] = old[3][0][0];
        cube[3][2][2] = old[3][0][2];
        cube[3][2][0] = old[3][2][2];
        cube[3][0][0] = old[3][2][0];

        cube[3][1][2] = old[3][0][1];
        cube[3][2][1] = old[3][1][2];
        cube[3][1][0] = old[3][2][1];
        cube[3][0][1] = old[3][1][0];

        moveSingle.clear();
        moveSingle.assign(1,"r");
        transmitData(false,true);
       
    }

    void Cube::ri()
    {
        if (last() == "ri ")
        {
            removeLast();
            moves += "r2 ";
        }
        else if (last() == "r2 ")
        {
            removeLast();
            moves += "r  ";
        }
        else if (last() == "r  ")
        {
            removeLast();
        }
        else
        {
            moves += "ri ";
        }
        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[5][i][2] = old[2][i][2];
            cube[2][i][2] = old[0][i][2];
            cube[0][2 - i][2] = old[4][i][0];
            cube[4][2 - i][0] = old[5][i][2];
        }
        cube[3][0][0] = old[3][0][2];
        cube[3][0][2] = old[3][2][2];
        cube[3][2][2] = old[3][2][0];
        cube[3][2][0] = old[3][0][0];

        cube[3][0][1] = old[3][1][2];
        cube[3][1][2] = old[3][2][1];
        cube[3][2][1] = old[3][1][0];
        cube[3][1][0] = old[3][0][1];
        
        moveSingle.clear();
        moveSingle.assign(1,"ri");
        transmitData(false,true);
        
    }

    void Cube::l()
    {
        if (last() == "l  ")
        {
            removeLast();
            moves += "l2 ";
        }
        else if (last() == "l2 ")
        {
            removeLast();
            moves += "li ";
        }
        else if (last() == "li ")
        {
            removeLast();
        }
        else
        {
            moves += "l  ";
        }
        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[5][i][0] = old[2][i][0];
            cube[2][i][0] = old[0][i][0];
            cube[0][i][0] = old[4][2 - i][2];
            cube[4][2 - i][2] = old[5][i][0];
        }
        cube[1][0][2] = old[1][0][0];
        cube[1][2][2] = old[1][0][2];
        cube[1][2][0] = old[1][2][2];
        cube[1][0][0] = old[1][2][0];

        cube[1][1][2] = old[1][0][1];
        cube[1][2][1] = old[1][1][2];
        cube[1][1][0] = old[1][2][1];
        cube[1][0][1] = old[1][1][0];
        
        moveSingle.clear();
        moveSingle.assign(1,"l");
        transmitData(false,true);
        
    }

    void Cube::li()
    {
        if (last() == "li ")
        {
            removeLast();
            moves += "l2 ";
        }
        else if (last() == "l2 ")
        {
            removeLast();
            moves += "l  ";
        }
        else if (last() == "l  ")
        {
            removeLast();
        }
        else
        {
            moves += "li ";
        }
        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[2][i][0] = old[5][i][0];
            cube[0][i][0] = old[2][i][0];
            cube[4][i][2] = old[0][2 - i][0];
            cube[5][i][0] = old[4][2 - i][2];
        }
        cube[1][0][0] = old[1][0][2];
        cube[1][0][2] = old[1][2][2];
        cube[1][2][2] = old[1][2][0];
        cube[1][2][0] = old[1][0][0];

        cube[1][0][1] = old[1][1][2];
        cube[1][1][2] = old[1][2][1];
        cube[1][2][1] = old[1][1][0];
        cube[1][1][0] = old[1][0][1];
        
        moveSingle.clear();
        moveSingle.assign(1,"li");
        transmitData(false,true);
        
    }

    void Cube::u()
    {
        if (last() == "u  ")
        {
            removeLast();
            moves += "u2 ";
        }
        else if (last() == "u2 ")
        {
            removeLast();
            moves += "ui ";
        }
        else if (last() == "ui ")
        {
            removeLast();
        }
        else
        {
            moves += "u  ";
        }

        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[1][0][i] = old[2][0][i];
            cube[4][0][i] = old[1][0][i];
            cube[3][0][i] = old[4][0][i];
            cube[2][0][i] = old[3][0][i];
        }
        cube[0][0][0] = old[0][2][0];
        cube[0][0][2] = old[0][0][0];
        cube[0][2][2] = old[0][0][2];
        cube[0][2][0] = old[0][2][2];

        cube[0][0][1] = old[0][1][0];
        cube[0][1][2] = old[0][0][1];
        cube[0][2][1] = old[0][1][2];
        cube[0][1][0] = old[0][2][1];
        
        moveSingle.clear();
        moveSingle.assign(1,"u");
        transmitData(false,true);
        
    }

    void Cube::ui()
    {
        if (last() == "ui ")
        {
            removeLast();
            moves += "u2 ";
        }
        else if (last() == "u2 ")
        {
            removeLast();
            moves += "u  ";
        }
        else if (last() == "u  ")
        {
            removeLast();
        }
        else
        {
            moves += "ui ";
        }
        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[2][0][i] = old[1][0][i];
            cube[1][0][i] = old[4][0][i];
            cube[4][0][i] = old[3][0][i];
            cube[3][0][i] = old[2][0][i];
        }
        cube[0][2][0] = old[0][0][0];
        cube[0][0][0] = old[0][0][2];
        cube[0][0][2] = old[0][2][2];
        cube[0][2][2] = old[0][2][0];

        cube[0][1][0] = old[0][0][1];
        cube[0][0][1] = old[0][1][2];
        cube[0][1][2] = old[0][2][1];
        cube[0][2][1] = old[0][1][0];

        moveSingle.clear();
        moveSingle.assign(1,"ui");
        transmitData(false,true);
    }

    void Cube::d()
    {
        if (last() == "d  ")
        {
            removeLast();
            moves += "d2 ";
        }
        else if (last() == "d2 ")
        {
            removeLast();
            moves += "di ";
        }
        else if (last() == "di ")
        {
            removeLast();
        }
        else
        {
            moves += "d  ";
        }
        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[2][2][i] = old[1][2][i];
            cube[1][2][i] = old[4][2][i];
            cube[4][2][i] = old[3][2][i];
            cube[3][2][i] = old[2][2][i];
        }
        cube[5][0][0] = old[5][2][0];
        cube[5][0][2] = old[5][0][0];
        cube[5][2][2] = old[5][0][2];
        cube[5][2][0] = old[5][2][2];

        cube[5][0][1] = old[5][1][0];
        cube[5][1][2] = old[5][0][1];
        cube[5][2][1] = old[5][1][2];
        cube[5][1][0] = old[5][2][1];
        
        moveSingle.clear();
        moveSingle.assign(1,"d");
        transmitData(false,true);
    }

    void Cube::di()
    {
        if (last() == "di ")
        {
            removeLast();
            moves += "d2 ";
        }
        else if (last() == "d2 ")
        {
            removeLast();
            moves += "d  ";
        }
        else if (last() == "d  ")
        {
            removeLast();
        }
        else
        {
            moves += "di ";
        }
        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[1][2][i] = old[2][2][i];
            cube[4][2][i] = old[1][2][i];
            cube[3][2][i] = old[4][2][i];
            cube[2][2][i] = old[3][2][i];
        }
        cube[5][2][0] = old[5][0][0];
        cube[5][0][0] = old[5][0][2];
        cube[5][0][2] = old[5][2][2];
        cube[5][2][2] = old[5][2][0];

        cube[5][1][0] = old[5][0][1];
        cube[5][0][1] = old[5][1][2];
        cube[5][1][2] = old[5][2][1];
        cube[5][2][1] = old[5][1][0];
        
        moveSingle.clear();
        moveSingle.assign(1,"di");
        transmitData(false,true);
    }

    void Cube::f()
    {
        if (last() == "f  ")
        {
            removeLast();
            moves += "f2 ";
        }
        else if (last() == "f2 ")
        {
            removeLast();
            moves += "fi ";
        }
        else if (last() == "fi ")
        {
            removeLast();
        }
        else
        {
            moves += "f  ";
        }
        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[0][2][i] = old[1][2 - i][2];
            cube[3][i][0] = old[0][2][i];
            cube[5][0][2 - i] = old[3][i][0];
            cube[1][2 - i][2] = old[5][0][2 - i];
        }
        cube[2][0][2] = old[2][0][0];
        cube[2][2][2] = old[2][0][2];
        cube[2][2][0] = old[2][2][2];
        cube[2][0][0] = old[2][2][0];

        cube[2][1][2] = old[2][0][1];
        cube[2][2][1] = old[2][1][2];
        cube[2][1][0] = old[2][2][1];
        cube[2][0][1] = old[2][1][0];
        
        moveSingle.clear();
        moveSingle.assign(1,"f");
        transmitData(false,true);
        }

    void Cube::fi()
    {
        if (last() == "fi ")
        {
            removeLast();
            moves += "f2 ";
        }
        else if (last() == "f2 ")
        {
            removeLast();
            moves += "f  ";
        }
        else if (last() == "f  ")
        {
            removeLast();
        }
        else
        {
            moves += "fi ";
        }
        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[1][i][2] = old[0][2][2 - i];
            cube[0][2][2 - i] = old[3][2 - i][0];
            cube[3][2 - i][0] = old[5][0][i];
            cube[5][0][i] = old[1][i][2];
        }
        cube[2][0][0] = old[2][0][2];
        cube[2][0][2] = old[2][2][2];
        cube[2][2][2] = old[2][2][0];
        cube[2][2][0] = old[2][0][0];

        cube[2][0][1] = old[2][1][2];
        cube[2][1][2] = old[2][2][1];
        cube[2][2][1] = old[2][1][0];
        cube[2][1][0] = old[2][0][1];
        
        moveSingle.clear();
        moveSingle.assign(1,"fi");
        transmitData(false,true);
    }

    void Cube::b()
    {
        if (last() == "b  ")
        {
            removeLast();
            moves += "b2 ";
        }
        else if (last() == "b2 ")
        {
            removeLast();
            moves += "bi ";
        }
        else if (last() == "bi ")
        {
            removeLast();
        }
        else
        {
            moves += "b  ";
        }
        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[1][2 - i][0] = old[0][0][i];
            cube[0][0][i] = old[3][i][2];
            cube[3][i][2] = old[5][2][2 - i];
            cube[5][2][2 - i] = old[1][2 - i][0];
        }
        cube[4][0][2] = old[4][0][0];
        cube[4][2][2] = old[4][0][2];
        cube[4][2][0] = old[4][2][2];
        cube[4][0][0] = old[4][2][0];

        cube[4][1][2] = old[4][0][1];
        cube[4][2][1] = old[4][1][2];
        cube[4][1][0] = old[4][2][1];
        cube[4][0][1] = old[4][1][0];
        
        moveSingle.clear();
        moveSingle.assign(1,"b");
        transmitData(false,true);
    }

    void Cube::bi()
    {
        if (last() == "bi ")
        {
            removeLast();
            moves += "b2 ";
        }
        else if (last() == "b2 ")
        {
            removeLast();
            moves += "b  ";
        }
        else if (last() == "b  ")
        {
            removeLast();
        }
        else
        {
            moves += "bi ";
        }
        copyCube();
        for (int i = 0; i < 3; i++)
        {
            cube[0][0][i] = old[1][2 - i][0];
            cube[3][i][2] = old[0][0][i];
            cube[5][2][2 - i] = old[3][i][2];
            cube[1][2 - i][0] = old[5][2][2 - i];
        }
        cube[4][0][0] = old[4][0][2];
        cube[4][0][2] = old[4][2][2];
        cube[4][2][2] = old[4][2][0];
        cube[4][2][0] = old[4][0][0];

        cube[4][0][1] = old[4][1][2];
        cube[4][1][2] = old[4][2][1];
        cube[4][2][1] = old[4][1][0];
        cube[4][1][0] = old[4][0][1];
        
        moveSingle.clear();
        moveSingle.assign(1,"bi");
        transmitData(false,true);
    }

    void Cube::solveTopCross()
    {
        moveTopEdges();
    //look for the 4 yellow edges
        for (int i = 1; i < 5; i++)
        {
        int side = 0; //side that the yellow sticker is on
        int pos = 0; //position (1-4) of the yellow sticker
        findNextTopEdge(side, pos);
        int numTurns = 0; //keeps track of how many times we turn the cube to put pieces on the bottom layer so we can undo the turns
        if (side == 1)
        {
            while (cube[5][1][0] == 0)
            {
                d();
            }
            while (cube[0][1][0] == 0)
            {
                u();
                numTurns++;
            }
            if (pos == 1)
            {
                l();
                l();
            }
            else if (pos == 2)
            {
                l();
            }
            else if (pos == 4)
            {
                li();
            }
        }
        else if (side == 2)
        {
            while (cube[5][0][1] == 0)
            {
                d();
            }
            while (cube[0][2][1] == 0)
            {
                u();
                numTurns++;
            }
            if (pos == 1)
            {
                f();
                f();
            }
            else if (pos == 2)
            {
                f();
            }
            else if (pos == 4)
            {
                fi();
            }
            di();
        }
        else if (side == 3)
        {
            while (cube[5][1][2] == 0)
            {
                d();
            }
            while (cube[0][1][2] == 0)
            {
                u();
                numTurns++;
            }
            if (pos == 1)
            {
                r();
                r();
            }
            else if (pos == 2)
            {
                r();
            }
            else if (pos == 4)
            {
                ri();
            }
            d();
            d();
        }
        else if (side == 4)
        {
            while (cube[5][2][1] == 0)
            {
                d();
            }
            while (cube[0][0][1] == 0)
            {
                u();
                numTurns++;
            }
            if (pos == 1)
            {
                b();
                b();
            }
            else if (pos == 2)
            {
                b();
            }
            else if (pos == 4)
            {
                bi();
            }
            d();
        }
        else if (side == 5)
        {
            for (int j = 0; j < 4 - pos; j++)
            {
                d();
            }
        }
        for (int j = 0; j < numTurns; j++)
        {
            ui();
        }
        if (side == 5) //algorithm for when the sticker is on the bottom
        {
            // WE NEED CODE HERE TO FIND OUT THE COLOR OF 121
            FindSingleColor(121);

            int color = cube[1][2][1];            
            for (int a = 1; a < color; a++) //set-up so that the edge goes into the right spot
            {
                u();
            }
            l();
            l();
            for (int a = 1; a < color; a++) //undo set-up
            {
                ui();
            }
        }
        else //algorithm for when th esticker is not on the bottom
        {
            // WE NEED CODE HERE TO FIND OUT THE COLOR OF 510
            FindSingleColor(510);

            int color = cube[5][1][0];
            if (color < 4) //set-up so that the sticker goes into the right spot
            {
                for (int a = 1; a < color + 1; a++)
                {
                    u();
                }
            }
            l();
            bi();
            li();
            if (color < 4) //undo set-up
            {
                for (int a = 1; a < color + 1; a++)
                {
                    ui();
                }
            }
        }
    }
}

void Cube::moveTopEdges() //move all yellow edge stickers on top to the bottom
{
    for (int i = 0; i < 4; i++)
    {
        if (cube[0][2][1] == 0)
        {
            while (cube[5][0][1] == 0)
            {
                d();
            }
            f();
            f();
        }
        u();
    }
}

void Cube::findNextTopEdge(int & s, int & p)
{
    for (s = 1; s < 6; s++)
    {
        if (cube[s][0][1] == 0)
        {
            p = 1;
            break;
        }
        else if (cube[s][1][2] == 0)
        {
            p = 2;
            break;
        }
        else if (cube[s][2][1] == 0)
        {
            p = 3;
            break;
        }
        else if (cube[s][1][0] == 0)
        {
            p = 4;
            break;
        }
    }
}

void Cube::solveTopCorners()
{
    moveTopCorners();
    for (int i = 1; i < 5; i++)
    {
        bool bottom = false; //holds whether the yellow sticker is on the bottom face
        bool rig = false; //holds whether the yellow sticker is on the right face
        int side = 0; //what face the yellow sticker is on
        int pos = 0; //which corner the piece is in
        findNextTopCorner(side, pos);
        if (side < 5) //puts the corner in the back bottom right corner
        {
            if (pos == 2)
            {
                rig = true;
            }
            for (int t = 0; t < side + pos - 1; t++)
            {
                di();
            }
        }
        else //puts the corner in the back bottom right corner
        {
            bottom = true;
            for (int t = 0; t < pos; t++)
            {
                di();
            }
        }
        if (rig) //algorithm for when yellow is on the right
        {
            FindSingleColor(420);
            int color = cube[4][2][0];
            if (color < 4) //move the top so that the corner goes into the correct position
            {
                for (int t = 0; t < color; t++)
                {
                    u();
                }
            }
            r();
            d();
            ri();
            if (color < 4) //undo the setup moves to put the corner in the right position
            {
                for (int t = 0; t < color; t++)
                {
                    ui();
                }
            }
        }
        else if (bottom) //algorithm for when yellow is on bottom
        {
            FindSingleColor(322);
            int color = cube[3][2][2];
            if (color < 4) //move the top so that the corner goes into the correct position
            {
                for (int t = 0; t < color; t++)
                {
                    u();
                }
            }
            r();
            d();
            d();
            ri();
            di();
            r();
            d();
            ri();
            if (color < 4) //undo the setup moves to put the corner in the right position
            {
                for (int t = 0; t < color; t++)
                {
                    ui();
                }
            }
        }
        else //algorithm for when yellow is on back
        {
            FindSingleColor(522);
            int color = cube[5][2][2];
            if (color < 4) //move the top so that the corner goes into the correct position
            {
                for (int t = 0; t < color; t++)
                {
                    u();
                }
            }
            bi();
            di();
            b();
            if (color < 4) //undo the setup moves to put the corner in the right position
            {
                for (int t = 0; t < color; t++)
                {
                    ui();
                }
            }
        }
    }
}

void Cube::moveTopCorners() //moves all yellow corners on the top to the bottom face
{
    for (int i = 0; i < 4; i++)
    {
        if (cube[0][2][2] == 0 || cube[2][0][2] == 0 || cube[3][0][0] == 0)
        {
            while (cube[1][2][2] == 0 || cube[2][2][0] == 0 || cube[5][0][0] == 0) //make sure not to push a different yellow corner back to the top
            {
                d();
            }
            ri();
            d();
            r();
        }
        u();
    }
}

void Cube::findNextTopCorner(int & s, int & p) //locates the next corner that is not in its correct spot (guaranteed to be one of the bottom four corners)
//updates pos and side in solveTopCorners() to hold the side and position of the yellow sticker
{
    for (s = 1; s < 5; s++)
    {
        if (cube[s][2][0] == 0)
        {
            p = 1;
            return;
        }
        else if (cube[s][2][2] == 0)
        {
            p = 2;
            return;
        }
    }
    if (cube[5][0][0] == 0)
    {
        p = 2;
    }
    else if (cube[5][0][2] == 0)
    {
        p = 3;
    }
    else if (cube[5][2][2] == 0)
    {
        p = 0;
    }
    else
    {
        p = 1;
    }
}

void Cube::solveMiddleLayer()
{
    int side = -1;
    int color = -1;
    int count = 0;

    beginning:
    if (side != -1)
    {
        placeMiddleEdge(color, side);
        //print();
        count++;
        side = -1;
        color = -1;
    }

    for (int i = 1; i <= 4; i++)
    {
        side = findBottomMatch(i);

        if (side != -1)
        {
            color = i;
            goto beginning;
        }
    }

    for (int i = 1; i <= 4; i++)
    {
        if (cube[i][1][0] != (i))
        {
            middleAlgorithm(i, 1);
            if(count >= 15)
            {
                solveTopCross();
                solveTopCorners();
            }

            goto beginning;
        }
        else if (cube[i][1][2] != i)
        {
            middleAlgorithm(i, 2);

            if(count >= 10)
            {
                solveTopCross();
                solveTopCorners();
            }

            goto beginning;
        }
    }

    int done = 0;
}

int Cube::findBottomMatch(int color)
{   
    for (int i = 1; i <= 4; i++)
    {   int temp=i*100+21;
        FindSingleColor(temp);
        if (cube[i][2][1] == color)
        {
            if (i % 2 == 0)
            {
                if (cube[5][abs(2 - i)][1] != 5)
                {
                    return i;
                }
            }
            else
            {
                if (cube[5][abs(2 - i)][i - 1] != 5)
                {
                    return i;
                }
            }
        }
    }

    return -1;
}

void Cube::placeMiddleEdge(int color, int edgeSide)
{
    if (edgeSide - color == 0)
    {
        middleAlgorithm(color, -1);
    }
    else if (edgeSide - color == 1)
    {
        di();
        middleAlgorithm(color, -1);
    }
    else if (edgeSide - color == 2)
    {
        di();
        di();
        middleAlgorithm(color, -1);
    }
    else if (edgeSide - color == 3)
    {
        d();
        middleAlgorithm(color, -1);
    }
    else if (edgeSide - color == -1)
    {
        d();
        middleAlgorithm(color, -1);
    }
    else if (edgeSide - color == -2)
    {
        d();
        d();
        middleAlgorithm(color, -1);
    }
    else if (edgeSide - color == -3)
    {
        di();
        //print();
        middleAlgorithm(color, -1);
    }
}

void Cube::middleAlgorithm(int color, int direction) //1 - L 2 - R -1 for nothing
{
    if (color == 1)
    {   
        FindSingleColor(510);
        if (cube[5][1][0] == 4 || direction == 1)
        {
            d();
            b();
            di();
            bi();
            di();
            li();
            d();
            l();
        }
        else if (cube[5][1][0] == 2 || direction == 2)
        {
            di();
            fi();
            d();
            f();
            d();
            l();
            di();
            li();
        }
    }
    else if (color == 2)
    {   
        FindSingleColor(501);
        if (cube[5][0][1] == 1 || direction == 1)
        {
            d();
            l();
            di();
            li();
            di();
            fi();
            d();
            f();
        }
        else if (cube[5][0][1] == 3 || direction == 2)
        {
            di();
            ri();
            d();
            r();
            d();
            f();
            di();
            fi();
        }
    }
    else if (color == 3)
    {   
        FindSingleColor(512);
        if (cube[5][1][2] == 2 || direction == 1)
        {
            d();
            f();
            di();
            fi();
            di();
            ri();
            d();
            r();
        }
        else if (cube[5][1][2] == 4 || direction == 2)
        {
            di();
            bi();
            d();
            b();
            d();
            r();
            di();
            ri();
        }
    }
    else if (color == 4)
    {   
        FindSingleColor(521);
        if (cube[5][2][1] == 3 || direction == 1)
        {
            d();
            r();
            di();
            ri();
            di();
            bi();
            d();
            b();
        }
        else if (cube[5][2][1] == 1 || direction == 2)
        {
            di();
            li();
            d();
            l();
            d();
            b();
            di();
            bi();
        }
    }
}

void Cube::solveBottomLayer()
{
    positionBottomCorners();
    //print();
    positionBottomEdges();
    //print();
    correctBottomCorners();
    //print();
    correctBottomEdges();
    //  print();
}

void Cube::positionBottomCorners()
{
    int dTurns[4];

    int bestNumDTurns = 0;
    int pos1 = -1;
    int pos2 = -1;

    for (int i = 0; i < 4; i++)
    {
        dTurns[i] = numCorrectCorners();

        if (dTurns[i] == 4)
        {
            pos1 = 4;
            bestNumDTurns = i;
            break;
        }
        else if (dTurns[i] < 4 && dTurns[i] > -1)
        {
            if (pos2 == -1)
            {
                pos1 = dTurns[i];
                bestNumDTurns = i;
            }
        }
        else if (dTurns[i] > 4)
        {
            int side1 = dTurns[i] % 10 + 1;
            int side2 = (int)(dTurns[i] / 10) + 1;

            //Corners are next to each other
            if (side2 == (side1 + 1) || side1 == (side2 + 1) || (side2 == 4 && side1 == 1) || (side2 == 1 && side1 == 4))
            {
                pos1 = side1;
                pos2 = side2;
                bestNumDTurns = i;
            }
            else if (pos2 == -1)
            {
                pos1 = side1;
                pos2 = side2;
                bestNumDTurns = i;
            }
        }

        d();
        //print();
    }

    for (int i = 0; i < bestNumDTurns && pos1 != 4; i++)
    {
        d();
    }


    //Finally Ready to start positioning corners with algorithm!!!!! OMG WHO KNEW A BRAIN COULD DO SO MUCH SO EASILY!!!!!

    if (pos1 == 4)
    {
        //One CRAZY lucky scenario, and is the only way to exit this nightmare :D
        return;
    }
    else if (pos2 != -1 && (pos2 == (pos1 + 1) || pos1 == (pos2 + 1) || (pos2 == 4 && pos1 == 1) || (pos2 == 1 && pos1 == 4)))
    {
        int oppFace = pos2 + 3;
        //int face = oppFace + 2;

        if (oppFace > 4)
        {
            oppFace = oppFace - 4;
        }

        swapCorners(oppFace);
        positionBottomCorners();
        return;
    }
    else if (pos2 != -1)
    {
        //Corners opp. corners correct
        int face = pos1 + 4;

        if (face > 4)
        {
            face = face - 4;
        }

        int face2 = face + 1;
        if (face2 > 4)
        {
            face2 = face2 - 4;
        }

        swapCorners(face);
        swapCorners(face2);
        swapCorners(face);
        positionBottomCorners();
        return;
    }
    else
    {
        //Only 1 correct, 3 corners wrong... If thats even posible (I'm 99% positive it isnt the way I set the cube up)?????
        //But just in case
        int face = pos1 + 2;

        if (face > 4)
        {
            face = face - 4;
        }

        swapCorners(face); //Eventually One of the previous situations above will occur :D
        positionBottomCorners();
        return;
    }

}

void Cube::swapCorners(int face)
{
    if (face == 1)
    {
        di();
        fi();
        d();
        b();
        di();
        f();
        d();
        bi();
        di();
    }
    else if (face == 2)
    {
        di();
        ri();
        d();
        l();
        di();
        r();
        d();
        li();
        di();
    }
    else if (face == 3)
    {
        di();
        bi();
        d();
        f();
        di();
        b();
        d();
        fi();
        di();
    }
    else if (face == 4)
    {
        di();
        li();
        d();
        r();
        di();
        l();
        d();
        ri();
        di();
    }
}

int Cube::numCorrectCorners()
{
    //If numCorrect ==  4 it will return 4
    //If numCorrect == 2 it will return 2nd Corner * 10 + 1st corner (i.e. 20)
    //Else if numCorrect == 1 it will return pos of corner
    //Else if 0 are correct return -1
    int cornerColors[3];
    int side1 = 0;
    int side2 = 0;

    int count = 0;

    int colorsMatch = 0; //if this equals 2 then there is a match per corner
    int position[4] = { -1, -1, -1, -1 }; //-1 means there is no match, 1 means match

    for (int i = 0; i < 4; i++)
    {
        getCorner(i, cornerColors);

        side1 = i + 1;
        side2 = i + 2;

        if (side2 == 5)
        {
            side2 = 1;
        }

        for (int j = 0; j < 3; j++)
        {
            if (cornerColors[j] == side1 || cornerColors[j] == side2)
            {
                colorsMatch++;
            }
        }

        if (colorsMatch == 2)
        {
            position[i] = 1;
            count++;
        }

        colorsMatch = 0;
    }

    int pos = 0;
    int multiplier = 1;

    if (count == 4)
    {
        return 4;
    }
    else if (count == 2 || count == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            if (position[i] == 1)
            {
                pos += i * multiplier;
                multiplier *= 10;
            }
        }
    }
    else
    {
        pos = -1;
    }

    return pos;
}

void Cube::getCorner(int num, int corners[])
{
    if (num == 0)
    {
        FindSingleColor(122);
        FindSingleColor(220);
        FindSingleColor(500);

        corners[0] = cube[1][2][2];
        corners[1] = cube[2][2][0];
        corners[2] = cube[5][0][0];
    }
    else if (num == 1)
    {
        FindSingleColor(222);
        FindSingleColor(320);
        FindSingleColor(502);

        corners[0] = cube[2][2][2];
        corners[1] = cube[3][2][0];
        corners[2] = cube[5][0][2];
    }
    else if (num == 2)
    {   
        FindSingleColor(322);
        FindSingleColor(420);
        FindSingleColor(522);

        corners[0] = cube[3][2][2];
        corners[1] = cube[4][2][0];
        corners[2] = cube[5][2][2];
    }
    else if (num == 3)
    {
        FindSingleColor(422);
        FindSingleColor(120);
        FindSingleColor(520);

        corners[0] = cube[4][2][2];
        corners[1] = cube[1][2][0];
        corners[2] = cube[5][2][0];
    }
}

void Cube::positionBottomEdges()
{
    int pos = numEdgesInCorrectPos();

    if (pos == 0)
    {
        rotateEdges(2, true);
        positionBottomEdges();
        return;
    }
    else if (pos == 5)
    {
        return;
    }
    else
    {
        bool clockwise = pos > 0;

        int face = abs(pos) + 1;

        if (face > 4)
        {
            face = face - 4;
        }

        rotateEdges(face, clockwise);
        return;
    }
}

int Cube::numEdgesInCorrectPos()
{
    //Note this will either be 1, 0 or 4...
    //It will return 0 if none are in correct position
    //If there is one in correct position it will return the corresponding position pos or neg determining rotation
    //Note positions are determined by what side they match with, 1 - 4
    //Return 5 if all are in position

    int edges[4][2] = { { cube[5][1][0], cube[1][2][1] }, { cube[5][0][1], cube[2][2][1] }, { cube[5][1][2], cube[3][2][1] }, { cube[5][2][1], cube[4][2][1] } };

    int correctPos = -1;
    int numCorrect = 0;
    bool isOneCorrect = false;
    int positions[4];

    for (int i = 1; i <= 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (edges[i - 1][j] == i)
            {
                isOneCorrect = true;
                correctPos = i - 1;
                numCorrect++;
            }

            if (edges[i - 1][j] != 5)
            {
                positions[i - 1] = edges[i - 1][j];
            }
        }
    }

    if (numCorrect == 4)
    {
        return 5;
    }

    if (isOneCorrect == false)
    {
        return 0;
    }
    else
    {
        bool clockwise = false;
        bool firstTime = true;
        int numTimesThrough = 0;
        numCorrect = 0;
        int temp;
        int temp2;

        while (numCorrect != 4)
        {
            firstTime = true;
            numCorrect = 0;
            for (int i = 0; i < 4; i++)
            {
                if (i != correctPos)
                {
                    if (firstTime)
                    {
                        firstTime = false;
                        temp = positions[i];

                        int j = i - 1;

                        if (j < 0)
                        {
                            j = 4 + j;
                        }

                        if (j == correctPos)
                        {
                            j--;
                            if (j < 0)
                            {
                                j = 4 + j;
                            }
                        }

                        positions[i] = positions[j];
                    }
                    else
                    {
                        temp2 = positions[i];
                        positions[i] = temp;
                        temp = temp2;
                    }
                }

                if (positions[i] == i + 1)
                {
                    numCorrect++;
                }
            }

            numTimesThrough++;
        }

        clockwise = numTimesThrough == 1;
        correctPos++;

        if (clockwise == false)
        {
            correctPos = -1 * (correctPos);
        }
    }

    return correctPos;
}

void Cube::rotateEdges(int face, bool rotClockwise)
{
    if (face == 1)
    {
        if (rotClockwise == true)
        {
            f();
            f();
            d();
            li();
            r();
            f();
            f();
            l();
            ri();
            d();
            f();
            f();
        }
        else
        {
            f();
            f();
            di();
            li();
            r();
            f();
            f();
            l();
            ri();
            di();
            f();
            f();
        }
    }
    else if (face == 2)
    {
        if (rotClockwise == true)
        {
            r();
            r();
            d();
            fi();
            b();
            r();
            r();
            f();
            bi();
            d();
            r();
            r();
        }
        else
        {
            r();
            r();
            di();
            fi();
            b();
            r();
            r();
            f();
            bi();
            di();
            r();
            r();
        }
    }
    else if (face == 3)
    {
        if (rotClockwise == true)
        {
            b();
            b();
            d();
            ri();
            l();
            b();
            b();
            r();
            li();
            d();
            b();
            b();
        }
        else
        {
            b();
            b();
            di();
            ri();
            l();
            b();
            b();
            r();
            li();
            di();
            b();
            b();
        }
    }
    else if (face == 4)
    {
        if (rotClockwise == true)
        {
            l();
            l();
            d();
            bi();
            f();
            l();
            l();
            b();
            fi();
            d();
            l();
            l();
        }
        else
        {
            l();
            l();
            di();
            bi();
            f();
            l();
            l();
            b();
            fi();
            di();
            l();
            l();
        }
    }
}

void Cube::correctBottomCorners()
{
    int result = cornerOrientation();

    while (result != 5)
    {
        twoCornerRotate(abs(findBestFace(result)), findBestFace(result) > 0);
                // old: twoCornerRotate(abs(findBestFace(result)), findBestFace > 0);

        result = cornerOrientation();
    }
}

int Cube::cornerOrientation()
{
    int numCorrect = 0;
    int wrongPosition = 0;

    int corners[4] = { cube[5][0][0], cube[5][0][2], cube[5][2][2], cube[5][2][0] };

    for (int i = 0; i < 4; i++)
    {
        if (corners[i] == 5)
        {
            numCorrect++;
        }
        else
        {
            wrongPosition = i;
        }
    }

    if (numCorrect == 4)
    {
        return 5;
    }

    return wrongPosition;
}

int Cube::findBestFace(int cornerNum)
{
    int corners[4] = { cube[5][0][0], cube[5][0][2], cube[5][2][2], cube[5][2][0] };

    int corner1 = cornerNum + 1;
    int corner2 = cornerNum - 1;

    if (corner1 > 3)
    {
        corner1 = 4 - corner1;
    }

    if (corner2 < 0)
    {
        corner2 = 4 + corner2;
    }

    //First Determine which corner to go with
    int finalCorner = -1;
    int face = -1;

    if (corners[cornerNum] == corners[corner1])
    {
        finalCorner = corner1;

        face = corners[finalCorner] + 1;

        if (face > 4)
        {
            face = face - 4;
        }

        return face;
    }
    else if (corners[cornerNum] == corners[corner2])
    {
        finalCorner = corner2;

        face = corners[finalCorner] + 1;

        if (face > 4)
        {
            face = face - 4;
        }

        return face;
    }
    else if (abs(corners[cornerNum] - corners[corner1]) == 2 && corners[corner1] != 5)
    {
        finalCorner = corner1;
        face = corners[finalCorner];
        return -face;
    }
    else if (abs(corners[cornerNum] - corners[corner2]) == 2 && corners[corner2] != 5)
    {
        finalCorner = corner2;
        face = corners[cornerNum];
        return -face;
    }
    else
    {
        face = corners[cornerNum];
        return face;
    }
}

void Cube::twoCornerRotate(int face, bool goForward)
{
    if (face == 1)
    {
        if (goForward == true)
        {
            bi();
            u();
            b();
            l();
            u();
            li();
            d();
            l();
            ui();
            li();
            bi();
            ui();
            b();
            di();
        }
        else
        {
            d();
            bi();
            u();
            b();
            l();
            u();
            li();
            di();
            l();
            ui();
            li();
            bi();
            ui();
            b();
        }
    }
    else if (face == 2)
    {
        if (goForward == true)
        {
            li();
            u();
            l();
            f();
            u();
            fi();
            d();
            f();
            ui();
            fi();
            li();
            ui();
            l();
            di();
        }
        else
        {
            d();
            li();
            u();
            l();
            f();
            u();
            fi();
            di();
            f();
            ui();
            fi();
            li();
            ui();
            l();
        }
    }
    else if (face == 3)
    {
        if (goForward == true)
        {
            fi();
            u();
            f();
            r();
            u();
            ri();
            d();
            r();
            ui();
            ri();
            fi();
            ui();
            f();
            di();
        }
        else
        {
            d();
            fi();
            u();
            f();
            r();
            u();
            ri();
            di();
            r();
            ui();
            ri();
            fi();
            ui();
            f();
        }
    }
    else if (face == 4)
    {
        if (goForward == true)
        {
            ri();
            u();
            r();
            b();
            u();
            bi();
            d();
            b();
            ui();
            bi();
            ri();
            ui();
            r();
            di();
        }
        else
        {
            d();
            ri();
            u();
            r();
            b();
            u();
            bi();
            di();
            b();
            ui();
            bi();
            ri();
            ui();
            r();
        }
    }
}

void Cube::correctBottomEdges()
{
    int edges[4] = { cube[5][1][0], cube[5][0][1], cube[5][1][2], cube[5][2][1] };

    bool isTogether = false;
    bool isStart = false;
    int numWrong = 0;
    int pos1 = -1;
    int pos2 = -1;
    int j = 0;

    for (int i = 0; i < 5; i++)
    {
        j = i;

        if (i == 4)
        {
            j = 0;
        }

        if (edges[j] != 5)
        {
            numWrong++;
            if (isStart == false)
            {
                pos1 = j;
                isStart = true;
            }
            else
            {
                pos2 = j;
                isTogether = true;
            }
        }
        else
        {
            isStart = false;
        }
    }

    if (numWrong >= 4)
    {
        twoEdgeRotate(1, false);
        twoEdgeRotate(2, false);
        return;
    }
    else
    {
        if(pos2 != -1)
        {
            twoEdgeRotate(pos2 + 1, isTogether);
        }
        else
        {
            twoEdgeRotate(pos1 + 1, isTogether);
        }
        return;
    }
}

void Cube::twoEdgeRotate(int face, bool isNextTo)
{
    if (face == 1)
    {
        if (isNextTo == true)
        {
            l();
            ui();
            d();
            f();
            f();
            d();
            d();
            u();
            u();
            b();
            d();
            bi();
            u();
            u();
            d();
            d();
            f();
            f();
            di();
            u();
            li();
            di();
        }
        else
        {
            l();
            ui();
            d();
            f();
            f();
            d();
            d();
            u();
            u();
            b();
            d();
            d();
            bi();
            u();
            u();
            d();
            d();
            f();
            f();
            di();
            u();
            li();
            di();
            di();
        }
    }
    else if (face == 2)
    {
        if (isNextTo == true)
        {
            f();
            ui();
            d();
            r();
            r();
            d();
            d();
            u();
            u();
            l();
            d();
            li();
            u();
            u();
            d();
            d();
            r();
            r();
            di();
            u();
            fi();
            di();
        }
        else
        {
            f();
            ui();
            d();
            r();
            r();
            d();
            d();
            u();
            u();
            l();
            d();
            d();
            li();
            u();
            u();
            d();
            d();
            r();
            r();
            di();
            u();
            fi();
            di();
            di();
        }
    }
    else if (face == 3)
    {
        if (isNextTo == true)
        {
            r();
            ui();
            d();
            b();
            b();
            d();
            d();
            u();
            u();
            f();
            d();
            fi();
            u();
            u();
            d();
            d();
            b();
            b();
            di();
            u();
            ri();
            di();
        }
        else
        {
            r();
            ui();
            d();
            b();
            b();
            d();
            d();
            u();
            u();
            f();
            d();
            d();
            fi();
            u();
            u();
            d();
            d();
            b();
            b();
            di();
            u();
            ri();
            di();
            di();
        }
    }
    else if (face == 4)
    {
        if (isNextTo == true)
        {
            b();
            ui();
            d();
            l();
            l();
            d();
            d();
            u();
            u();
            r();
            d();
            ri();
            u();
            u();
            d();
            d();
            l();
            l();
            di();
            u();
            bi();
            di();
        }
        else
        {
            b();
            ui();
            d();
            l();
            l();
            d();
            d();
            u();
            u();
            r();
            d();
            d();
            ri();
            u();
            u();
            d();
            d();
            l();
            l();
            di();
            u();
            bi();
            di();
            di();
        }
    }
}

void Cube::copyCube()
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                old[i][j][k] = cube[i][j][k];
            }
        }
    }
}

void Cube::print()
{
    int i=0; // top
    for (int j = 0; j < 3; j++)
    {
        cout << "    |";
        for (int k = 0; k < 3; k++)
        {
            cout << cube[i][j][k];
        }
        cout <<"|"<< endl;
    }

    // left front right back
    for (int j = 0; j < 3; j++)
    {
        cout << "|";
        for (i = 1; i < 5; i++)
        {

            for (int k = 0; k < 3; k++)
            {
                cout << cube[i][j][k];
            }
            cout <<"|";

        }
        cout << endl;
    }
    i=5; // bottom
    for (int j = 0; j < 3; j++)
    {
        cout << "    |";
        for (int k = 0; k < 3; k++)
        {
            cout << cube[i][j][k];
        }
        cout <<"|"<< endl;
    }
    cout << endl;
}

string Cube::last()
{
    if (moves.size() < 3)
    {
        return "";
    }
    int s = moves.size();
    return moves.substr(s - 3, 3);
}

void Cube::removeLast()
{
    if (moves.size() < 3)
    {
        return;
    }
    int s = moves.size();
    moves = moves.substr(0, s - 3);
}

void Cube::clearMoves()
{
    moves = "";
}
void Cube::scramble()
{
    srand(time(NULL));
    for (int i = 0; i < 30; i++)
    {
        int num = rand() % 12 + 1;
        switch (num)
        {
            case 1:
            r();
            break;
            case 2:
            ri();
            break;
            case 3:
            u();
            break;
            case 4:
            ui();
            break;
            case 5:
            f();
            break;
            case 6:
            fi();
            break;
            case 7:
            l();
            break;
            case 8:
            li();
            break;
            case 9:
            b();
            break;
            case 10:
            bi();
            break;
            case 11:
            d();
            break;
            case 12:
            di();
            break;
        }
    }
}


