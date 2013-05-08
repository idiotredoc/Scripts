pgausselim : pgausselim.o 
	g++ -o pgausselim pgausselim.o 
pgausselim.o : pgausselim.cpp
	g++ -c pgausselim.cpp  
ludecomp : ludecomp.o
	g++ -o ludecomp ludecomp.o
ludecomp.o : ludecomp.cpp
	g++ -c ludecomp.cpp

clean: 
	rm *.o pgausselim ludecomp
