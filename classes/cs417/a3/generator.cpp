#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[])
{
	int index = atoi(argv[1]);
	int B[index];
	int A[index][index];

	string file; 
	ostringstream oss;
	
	srand(33);
	for (int i=0;i<index;i++) {
		B[i] = rand() % 100 +1;
	}
	
	for (int i=0;i<index;i++) {
		for (int j=0;j<index;j++) {
			A[i][j] = rand() % 100 +1;
		}
	}		
	
	oss << "matrices/" << index << "x" << index << ".txt";
	file = oss.str();
	ofstream myfile(file.c_str());

	myfile << index <<" ";

	for (int i=0;i<index;i++) {
		myfile << B[i] << " ";
        }

        for (int i=0;i<index;i++) {
                for (int j=0;j<index;j++) {
                        myfile << A[i][j] << " ";
                }
        }

	myfile <<endl;	
	return 0;
}
