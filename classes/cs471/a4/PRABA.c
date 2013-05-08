#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define MAX_SEQUENCE_SIZE 9999;

int             op[9], ff[9], lr[9], mr[9], lf[9], mf[9], rn[9];		

void           *optimal(void *param);
void           *fifo(void *param);
void           *leastrecent(void *param);
void           *mostrecent(void *param);
void           *lestfreq(void *param);
void           *mostfreq(void *param);
void           *random(void *param);

int 		seq[MAX_SEQUENCE_SIZE];

int 
main(int argc, char *argv[])
{
	pthread_t       tid,tid2,tid3,tid4,tid5,tid6,tid7,tid8;
	pthread_attr_t  attr;

	int *fd, n; 
	char *mode = "r";
	int count = 0;

	if (argc <= 2) {
		fprintf(stderr, "Syntax: PRABA -f <sfile> | -b <slength> <tnumber>\n");
		return -1;
	}

	if (strncmp (argv[1], "-f") == 0) {
		//use sequence file
		if (argc <3) {
			fprintf(stderr, "Syntax: PRABA -f <sfile> | -b <slength> <tnumber>\n");
			return -1;
		}
		if(ifp = fopen(argv[2]),mode) {
			while (fscanf(fd, "%d", n) != EOF) {
				seq[count] = n;
				count++;
			}
			//begin creating threads, etc
			pthread_attr_init(&attr);
		        pthread_create(&tid, &attr, optimal, count);
			//...
		        pthread_join(tid, NULL);
		        pthread_join(tid2, NULL);
			// print out summary 
		}		
		else {
			fprintf(stderr, "Please enter a valid Sequence File");
			return -1;
		}
	}
	else if (strncmp (argv[1], "-b") == 0) {
		if (argc <4) {
                        fprintf(stderr, "Syntax: PRABA -f <sfile> | -b <slength> <tnumber>\n");
                        return -1;
                }
		int tries = argv[3];
		int slength = argv[2];
		//generate random sequence of slength
		srand(time(NULL));
		for (count;count < slength; count++) {
	        	seq[count] = (rand() % 10);
		}
	 	pthread_attr_init(&attr);

		//while loop of tests
                pthread_create(&tid, &attr, optimal, count);
                //...
                pthread_join(tid, NULL);
                pthread_join(tid2, NULL);
                // print out summary

	}
	else {
		fprintf(stderr, "Syntax: PRABA -f <sfile> | -b <slength> <tnumber>\n");
                return -1;
	}
}

/**
 * The thread will begin control in this function
 */
void           *
runner(void *param)
{
	int             i, upper = atoi(param);
	sum = 0;

	if (upper >= 0) {
		for (i = 0; i <= upper; i++) {
			sum += i;
//                printf("sum is %d\n", sum);
                //usleep(1);
                }
	}
	pthread_exit(0);
}

void           *
runner2(void *param)
{
	int             i, upper = atoi(param);
	sum2 = 0;

	if (upper >= 0) {
		for (i = 0; i <= upper; i++) {
			sum2 += pow(2, i);
 //               printf("sum2 is %d\n", sum2);
                // usleep(1);
                }
	}
	pthread_exit(0);
}
void 
printseq() 
{

}
#include <stdio.h>
#include <string.h>

int
main(int argc, char *argv[])
{
	int	seq[100] = {6,3,6,4,7,9,1,4,3,6,2,6,3,9,2,6,4,9,2,4,2,2,6,5,2,5,5,5,1,5,9,7,6,8,3,9,3,8,2,8,3,3,6,7,1,3,2,4,4,6,8,1,5,7,5,6,3,5,5,6,1,5,8,9,6,3,7,4,6,5,3,8,8,6,5,8,9,7,5,1,4,2,4,1,1,8,4,8,6,3,8,6,2,1,8,5,5,3,1,7};	
	int 	working_set[9] = { 0 };
	int 	fault_count[10] = { 0 };
	int 	dist[9] = { 0 };
	int 	n,m,o,p,tmp,index,max,bool1,bool2,bool3,slength=100;
	struct belady {
		char**  algr;
		int	bcount;
	};
	char** msg;

	for (m=1;m<10;m++) {				
		for(o=0;o<m;o++) {
			working_set[o]=0;
		}
		for (n = 0; n<slength; n++){
			bool1 = 0;
			bool2 = 0;
		 	for (o=0;o<m;o++) {
				if (seq[n] == working_set[o]) {
					working_set[o] = seq[n];
					bool1 = 1;	
					bool2 = 1;
					break;
				}
			}
			if (bool1 == 0) {
				for(o=0;o<m;o++) {
					if (working_set[o] == 0) {
	               		                working_set[o] = seq[n];
						fault_count[m] = fault_count[m]+=1;
						bool2 = 1;
						break;
					}
				}
			}
			if (bool2 == 0) {
				for(o=0;o<m;o++) {
					dist[o]=0;
				}
				for(o=0;o<m;o++) {
					for (p=n+1;p<slength;p++) {
						if (working_set[o] == seq[p]) {
							dist[o] = p-n;
							break;
						}
					}
				}
				bool3 = 0;
				for (o=0;o<m;o++) {
					if (dist[o] == 0) {
						index = o;
						bool3 = 1;
						break;
					}
				}
				if (bool3 == 0) {
					max = dist[0];
					index = 0;
					for (o=0;o<m;o++) {
						if (max < dist[o]) {
							max=dist[o];
							index = o;
						}
					}
				}
				working_set[index] = seq[n];
				fault_count[m] = fault_count[m]+=1;
				
			}
		}
	}
        for (m=1;m<10;m++) {
		printf("\n%d\t%d\n",m,fault_count[m]);			
	}
}
				

