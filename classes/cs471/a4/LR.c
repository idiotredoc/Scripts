#include <stdio.h>
#include <string.h>

int
main(int argc, char *argv[])
{
	int	seq[100] = {6,3,6,4,7,9,1,4,3,6,2,6,3,9,2,6,4,9,2,4,2,2,6,5,2,5,5,5,1,5,9,7,6,8,3,9,3,8,2,8,3,3,6,7,1,3,2,4,4,6,8,1,5,7,5,6,3,5,5,6,1,5,8,9,6,3,7,4,6,5,3,8,8,6,5,8,9,7,5,1,4,2,4,1,1,8,4,8,6,3,8,6,2,1,8,5,5,3,1,7};	
	struct frame {
		int data;
		int ref;
	};
	struct frame	working_set[9];
	int 	fault_count[10] = { 0 };
	int 	dist[9] = { 0 };
	int 	n,m,o,index,incr,min,bool1,bool2,bool3,slength=100;
	for (m=1;m<10;m++) {				
		incr=0;
		for(o=0;o<m;o++) {
			working_set[o].data=0;
			working_set[o].ref=0;
		}
		for (n = 0; n<slength; n++){
			incr++;
			bool1 = 0;
			bool2 = 0;
		 	for (o=0;o<m;o++) {
				if (seq[n] == working_set[o].data) {
					working_set[o].data = seq[n];
					working_set[o].ref = incr;
					bool1 = 1;	
					bool2 = 1;
					break;
				}
			}
			if (bool1 == 0) {
				for(o=0;o<m;o++) {
					if (working_set[o].data == 0) {
	               		                working_set[o].data = seq[n];
						working_set[o].ref = incr;
						fault_count[m] = fault_count[m]+=1;
						bool2 = 1;
						break;
					}
				}
			}
			if (bool2 == 0) {
				min=working_set[0].ref;
				for(o=0;o<m;o++) {
					if (working_set[o].ref <= min) {
						min = working_set[o].ref;
						index = o;
					}
				}
				working_set[index].data = seq[n];
				working_set[index].ref = incr;
				fault_count[m] = fault_count[m]+=1;
			}
		}
	}
        for (m=1;m<10;m++) {
		printf("\n%d\t%d\n",m,fault_count[m]);
	}
}
