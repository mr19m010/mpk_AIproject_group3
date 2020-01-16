#include <iostream>
using namespace std;

int getQuestionNumber(int a)
{
  int n = 0;
  cout << "Enter amount of question asked inbetween the range [2-53]:" << endl;
  cin >> n;
  if(n <= 2 || n >=53)
  {
    cout << "Error! Invalid number, try again!" << endl;
    getQuestionNumber(int a);
  }
  else
    cout << "Thank you! Question asked: " << n << endl;
  return n;
}
