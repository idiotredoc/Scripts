//Nathan Lutz CS417 Assignment 2 LU Decomposition

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <new>
using namespace std;

double** createMatrix(int, int);
double* createVector(int);
void deleteMatrix(double**, int);
void deleteVector(double*);
void printMatrix(double**, int);
void printVector(double*, int);

int main(int argc, char *argv[])
{
	double x, sum;
	int n;
	double** a;
	double* b;
        if (argc != 2)
	{
                cout << "usage: "<< argv[0] <<" <filename>\n";
                return 1;
        }
        else
	{
                ifstream infile;
                infile.open(argv[1]);
                if (infile.is_open())
		{
                        infile >> n;
			a = createMatrix(n, n);
			b = createVector(n);

                        cout << "n: "<< n <<".\n";
                        for (int i=0; i<n; i++)
			{
                                infile >> b[i];
                        }
                        for (int j=0; j<n; j++)	
			{
                                for (int i=0; i<n; i++)
				{
                                        infile >> a[j][i];
                                }
                        }
                        infile.close();
                }
                else cout << "Unable to open file " << argv[1] <<".\n";
	}
	cout << "Matrix A:" << endl;
	printMatrix(a, n);
	cout << "RHS vector b:" << endl;
	printVector(b, n);

	// upper triangular 
	for (int k=0;k<n-1;k++)
	{
		if ( fabs(a[k][k])>=1.e-6)
	  	{
	    		for (int i=k+1;i<n;i++)
	    		{
	      			x = a[i][k]/a[k][k];
	      			for (int j=k+1;j<n;j++) a[i][j] = a[i][j] -a[k][j]*x;
	        			b[i] = b[i] - b[k]*x;
	    		}
	  	}
	  	else
	  	{
	    		cout << "zero pivot found in line:" << k << endl;
	    		return 1;
	  	}
	}

	// back substitution
	b[n-1]=b[n-1]/a[n-1][n-1];
	for ( int i = n-2; i >= 0; i--)
	{
		sum = b[i];
	    	for (int j = i+1; j < n; j++)
	      		sum = sum - a[i][j]*b[j];
	  	b[i] = sum/a[i][i];
	}

	cout << "Solution:" << endl;
	
	printVector(b, n);
	
	deleteMatrix(a, n);
	deleteVector(b);

	return 0;
}

double** createMatrix(int n, int m)
{
	double** p; 
	
	try 
	{
		p = new double* [n];
	  	for ( int i = 0; i < n; i++ )
	      		p[i] = new double[m];
	}
	catch (bad_alloc e) 
	{
		cout << "Exception occurred: "<< e.what() << endl;
	}

	return p;
} 

double* createVector(int n)
{
	double* p;

	try 
	{
		p = new double[n];
	}
	catch (bad_alloc e) 
	{
		cout << "Exception occurred: "<< e.what() << endl;
	}

	return p;
}

void deleteMatrix(double** p, int n)
{
	for ( int i = 0; i < n; i++)
		delete[] p[i];
	delete[] p;
	p = 0;
}

void deleteVector(double* p)
{
	delete[] p;
	p = 0;
}

void printMatrix(double** p, int n)
{
	for ( int i = 0; i < n; i++)
	{
		for ( int j = 0; j < n; j++)
	    		cout << setiosflags(ios::showpoint | ios::fixed | ios::right) 
	         	<< setprecision(4) 
	         	<< setw(12) << p[i][j];
	  	cout << endl;
	}
}

void printVector(double* p, int n)
{
	for ( int i = 0; i < n; i++)
		cout << setiosflags(ios::showpoint | ios::fixed | ios::right) 
	       	<< setprecision(4)
	       	<< setw(12) << p[i];
	cout << endl << endl;
}
