
#include <iostream>
#include "natural.h"


using namespace std;



Natural factorial (const Natural& j)
{
  Natural k = 1;
  for (Natural i = 2; i <= j; ++i)
    {
      k = k * i;
    }
  return k;
}


int main (int argc, char** argv) 
{
  unsigned k, n;
  cout << "What is the largest number in the lottery? " << flush;
  cin >> n;
  cout << "\nHow many numbers in each drawing? " << flush;
  cin >> k;
  cout << "\n";

  // Additional output for debugging purposes
  //  (and to help in providing partial credit when grading)
  //  is activated by running the program with a -debug flag:
  //    lottery.exe -debug
  string debugFlag ((argc > 1) ? argv[1] : "");
  if (debugFlag == "-debug")
    {
      Natural x = n;
      
      // try doubling x 32 times to test addition:
      for (int i = 0; i < 32; i++)
	{
	  x = x + x;
	}
      cout << "Addition test: " << x << endl;

      // Subtraction
      Natural sub = x - n;
      cout << "Subtraction test: " << sub << endl;

      // Division
      Natural div = x / n;
      cout << "Division test: " << div << endl;


      // Multiplication test
      for (int i = 0; i < 8; i++)
	{
	  x = x * n;
	}
      cout << "Multiplication test: " << x << endl;


      // Check the equality and < operators
      Natural y = n;
      if (y == 5)
	cout << y << " == " << 5 << endl;
      else
	{
	  cout << y << " != " << 5 << endl;
	  
	  if (y < 5)
	    cout << y << " <  " << 5 << endl;
	  else
	    cout << y << " >  " << 5 << endl;
	}
    }

  Natural factn = factorial(n);


  if (debugFlag == "-debug")
    cout << "Factorial of " << n << " is " << factn << endl;
  
  Natural odds = factn / ((factorial(k) * factorial(n-k)));

  cout << "Odds of winning are 1 in " << odds << endl;

  return 0;
}

