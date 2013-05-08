Jacobi : Jacobi.o 
	g++ -o Jacobi Jacobi.o 
Jacobi.o : Jacobi.cpp
	g++ -c Jacobi.cpp  
generator : generator.o
        g++ -o generator generator.o 
generator.o : generator.cpp
        g++ -c generator.cpp  
GaussSeidel : GaussSeidel.o
	g++ -o GaussSeidel GaussSeidel.o
GaussSeidel.o : GaussSeidel.cpp
	g++ -c GaussSeidel.cpp
SSOR : SSOR.o
        g++ -o SSOR SSOR.o 
SSOR.o : SSOR.cpp
        g++ -c SSOR.cpp  
clean: 
	rm *.o Jacobi GaussSeidel SSOR generator
