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
