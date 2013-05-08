//Nathan Lutz CS417 Assignment 2 Gaussian Elimination

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
	double sum;
	int nx,ny,n;
	double** a;
	double** l;
	double** u;
	double* x;
	double* y;
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
                        infile >> nx;
                        infile >> ny;
                        n = ny * nx;

			a = createMatrix(n, n);
			l = createMatrix(n, n);
			u = createMatrix(n, n);

			b = createVector(n);
			x = createVector(n);
			y = createVector(n);

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

	// LU Decomp
	for(int t=0;t<n;t++)
    	{
        	u[t][t]=1;
	        for(int i=t;i<n;i++)
        	{
		        sum=0;
		        for(int p=0;p<t-1;p++)
                		sum+=l[i][p]*u[p][t];
	            	l[i][t]=a[i][t]-sum;
        	}

        	for(int j=t+1;j<n;j++)
	        {
                	sum=0;
	                for(int p=0;p<t-1;p++)
            			sum+=l[t][p]*u[p][j];
		        u[t][j]=(a[t][j]-sum)/l[t][t];
        	}
    	}
	
	cout << "L Matrix: " << endl;
	printMatrix(l, n);
	cout << "U Matrix: " << endl;
	printMatrix(u, n);

	// Forward solve for y
	for(int i=0;i<n;i++)
    	{                                       
        	sum=0;
	        for(int p=1;p<i;p++)
        		sum+=l[i][p]*y[p];
	        y[i]=(b[i]-sum)/l[i][i];
    	}
	// Back solve for x
	for(int i=n-1;i>0;i--)
    	{
        	sum=0;
	        for(int p=n;p>i;p--)
        		sum+=u[i][p]*x[p];
	        x[i]=(y[i]-sum)/u[i][i];
	}

	cout << "Solution:" << endl;
	
	printVector(x, n);
	
	deleteMatrix(a, n);
	deleteMatrix(l, n);
	deleteMatrix(u, n);
	deleteVector(b);
	deleteVector(y);
	deleteVector(x);
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
