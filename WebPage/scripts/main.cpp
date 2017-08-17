#include <iostream>
using namespace std;

int main(int argc, char *argv[])
 {
 	/*--------------File I/O-----------------*/
    if(argc == 1)
    {
      cout << "No arguments passed in." << endl;
    }

    cout << "Cups: " << argv[1] << endl;
    cout << "Strength: " << argv[2] << endl;
    return 0;
   
 }
