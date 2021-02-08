#include <iostream>
#include <string>
using namespace std;

int main ()
{
  string mystr;
  cout << "What's your name? ";
  getline (cin, mystr);
  cout << "Hello " << mystr << ".\n";
  cout << "What is your favorite color? ";
  getline (cin, mystr);
  cout << "I like " << mystr << " too!\n";
  return 0;
}