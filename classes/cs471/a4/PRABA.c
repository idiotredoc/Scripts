#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_SEQUENCE_SIZE 9999;

int             op_count[10], ff_count[10], lr_count[10], mr_count[10], lf_count[10], mf_count[10], rn_count[10];		

void           *optimal();
void           *fifo();
void           *lr();
void           *mr();
void           *lf();
void           *mf();
void           *rn();

int 		seq[999];
int 		count;

int 
main(int argc, char *argv[])
{
        srand(time(NULL));
	pthread_t       tid,tid2,tid3,tid4,tid5,tid6,tid7;
	pthread_attr_t  attr;
	FILE* file;
	int n,i; 
	char *mode = "r";
	count = 0;
	int tries, slength;
	struct bel_det {
		char *msg;
		int  flag;
	};
	struct bel_det op_bel_det[9];
        struct bel_det ff_bel_det[9];
        struct bel_det lr_bel_det[9];
        struct bel_det mr_bel_det[9];
        struct bel_det lf_bel_det[9];
        struct bel_det mf_bel_det[9];
        struct bel_det rn_bel_det[9];

	int op_bel_count =0;
	int ff_bel_count =0;
	int lr_bel_count=0;
	int mr_bel_count=0;
	int lf_bel_count=0;
        int mf_bel_count =0;
	int rn_bel_count =0;

	if (argc <= 2) {
		fprintf(stderr, "Syntax: PRABA -f <sfile> | -b <slength> <tnumber>\n");
		return -1;
	}

	if (strncmp (argv[1], "-f",2) == 0) {
		//use sequence file
		if (argc <3) {
			fprintf(stderr, "Syntax: PRABA -f <sfile> | -b <slength> <tnumber>\n");
			return -1;
		}
		
		file = fopen (argv[2], "r");
		if(file) {
			n = 0;
			fscanf (file, "%d", &n);    
			while (!feof (file)) {
			        seq[count] = n;
      		       		fscanf (file, "%d", &n);      
		       		count++;
    			}
			fclose (file);        
		
			//begin creating threads, etc
			pthread_attr_init(&attr);
		        pthread_create(&tid, &attr, optimal,NULL);
                        pthread_create(&tid2, &attr, fifo,NULL);
                        pthread_create(&tid3, &attr, lr,NULL);
                        pthread_create(&tid4, &attr, mr,NULL);
                        pthread_create(&tid5, &attr, lf,NULL);
                        pthread_create(&tid6, &attr, mf,NULL);
                        pthread_create(&tid7, &attr, rn,NULL);

		        pthread_join(tid, NULL);
		        pthread_join(tid2, NULL);
			pthread_join(tid3, NULL);
                        pthread_join(tid4, NULL);
                        pthread_join(tid5, NULL);
                        pthread_join(tid6, NULL);
                        pthread_join(tid7, NULL);

			// print out summary 
			printf("Using Sequence stored in File: %s\n",argv[2]);
			for (n=0;n<count;n++) {
				printf("%d ",seq[n]);
			}
			printf("\n\nPage Fault Counts:\n\n");
			printf("W\tOP\tFF\tLR\tMR\tLF\tMF\tRN\n");
			for (n=1;n<10;n++) {
				printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n",n,op_count[n],ff_count[n],lr_count[n],mr_count[n],lf_count[n],mf_count[n],rn_count[n]);
			}
		}		
		else {
			fprintf(stderr, "Please enter a valid Sequence File");
			return -1;
		}
	}
	else if (strncmp (argv[1], "-b",2) == 0) {
		if (argc <4) {
                        fprintf(stderr, "Syntax: PRABA -f <sfile> | -b <slength> <tnumber>\n");
                        return -1;
                }
		tries = atoi(argv[3]);
		slength = atoi(argv[2]);
		//generate random sequence of slength

		printf("Testing Belady's Anomaly using %d random sequences and Trying %d times\n",slength,tries);
		//while loop of tests
		for(i=0;i<tries;i++) {
			sleep(1);
			for (count=0;count < slength; count++) {
                        	n=(rand() % 9)+1;
	                        seq[count] = n;
        	        }

	                pthread_attr_init(&attr);

			pthread_create(&tid, &attr, optimal,NULL);
                        pthread_create(&tid2, &attr, fifo,NULL);
                        pthread_create(&tid3, &attr, lr,NULL);
                        pthread_create(&tid4, &attr, mr,NULL);
                        pthread_create(&tid5, &attr, lf,NULL);
                        pthread_create(&tid6, &attr, mf,NULL);
                        pthread_create(&tid7, &attr, rn,NULL);

                        pthread_join(tid, NULL);
                        pthread_join(tid2, NULL);
                        pthread_join(tid3, NULL);
                        pthread_join(tid4, NULL);
                        pthread_join(tid5, NULL);
                        pthread_join(tid6, NULL);
                        pthread_join(tid7, NULL);

                        // print out summary 
			printf("\nTrial #%d:\n",i+1);
                        for (n=0;n<count;n++) {
                                printf("%d ",seq[n]);
                        }
                        printf("\n\nPage Fault Counts:\n\n");
                        printf("W\tOP\tFF\tLR\tMR\tLF\tMF\tRN\n");
                        for (n=1;n<10;n++) {
                                printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n",n,op_count[n],ff_count[n],lr_count[n],mr_count[n],lf_count[n],mf_count[n],rn_count[n]);
				if (n != 1) {
				if (op_count[n] > op_count[n-1]) {
					sprintf(op_bel_det[i].msg,"Belady detected: OP W[%d]=%d < W[%d]=%d (Difference = %d)",n,op_count[n-1],op_count[n],op_count[n]-op_count[n-1]);
					op_bel_det[i].flag=1;
					op_bel_count++;
				}
				if (ff_count[n] > ff_count[n-1]) {
                                        sprintf(ff_bel_det[i].msg,"Belady detected: FF W[%d]=%d < W[%d]=%d (Difference = %d)",n,ff_count[n-1],ff_count[n],ff_count[n]-ff_count[n-1]);
                                        ff_bel_det[i].flag=1;
                                        ff_bel_count++;
                                }
				if (lr_count[n] > lr_count[n-1]) {
                                        sprintf(lr_bel_det[i].msg,"Belady detected: LR W[%d]=%d < W[%d]=%d (Difference = %d)",n,lr_count[n-1],lr_count[n],lr_count[n]-lr_count[n-1]);
                                        lr_bel_det[i].flag=1;
                                        lr_bel_count++;
                                }
				if (mr_count[n] > mr_count[n-1]) {
                                        sprintf(mr_bel_det[i].msg,"Belady detected: MR W[%d]=%d < W[%d]=%d (Difference = %d)",n,mr_count[n-1],mr_count[n],mr_count[n]-mr_count[n-1]);
                                        mr_bel_det[i].flag=1;
					mr_bel_count++;
                                }
				if (lf_count[n] > lf_count[n-1]) {
                                        sprintf(lf_bel_det[i].msg,"Belady detected: LF W[%d]=%d < W[%d]=%d (Difference = %d)",n,lf_count[n-1],lf_count[n],lf_count[n]-lf_count[n-1]);
                                        lf_bel_det[i].flag=1;
					lf_bel_count++;
                                }
				if (mf_count[n] > mf_count[n-1]) {
                                        sprintf(mf_bel_det[i].msg,"Belady detected: MF W[%d]=%d < W[%d]=%d (Difference = %d)",n,mf_count[n-1],mf_count[n],mf_count[n]-mf_count[n-1]);
                                        mf_bel_det[i].flag=1;
					mf_bel_count++;
                                }
				if (op_count[n] > op_count[n-1]) {
                                        sprintf(rn_bel_det[i].msg,"Belady detected: RN W[%d]=%d < W[%d]=%d (Difference = %d)",n,rn_count[n-1],rn_count[n],rn_count[n]-rn_count[n-1]);
                                        rn_bel_det[i].flag=1;
					rn_bel_count++;
                                }
				}
			}
			if (op_bel_det[i].flag == 1) 
				printf("%s",op_bel_det[i].msg);
                        if (ff_bel_det[i].flag == 1)
                                printf("%s",ff_bel_det[i].msg);
                        if (lr_bel_det[i].flag == 1)
                                printf("%s",lr_bel_det[i].msg);
                        if (mr_bel_det[i].flag == 1)
                                printf("%s",mr_bel_det[i].msg);
                        if (lf_bel_det[i].flag == 1)
                                printf("%s",lf_bel_det[i].msg);
                        if (mf_bel_det[i].flag == 1)
                                printf("%s",mf_bel_det[i].msg);
                        if (rn_bel_det[i].flag == 1)
                                printf("%s",rn_bel_det[i].msg);
		}
		printf("Summary of belady detected: \n");
		for (n=0;n<tries;n++) {
			if (op_bel_det[n].flag == 1)
                                printf("%d, OP",n+1);
                        if (ff_bel_det[i].flag == 1)
                                printf("%d, FF",n+1);
                        if (lr_bel_det[i].flag == 1)
                                printf("%d, LR",n+1);
                        if (mr_bel_det[i].flag == 1)
                                printf("%d, MR",n+1);
                        if (lf_bel_det[i].flag == 1)
                                printf("%d, LF",n+1);
                        if (mf_bel_det[i].flag == 1)
                                printf("%d, MF",n+1);
                        if (rn_bel_det[i].flag == 1)
                                printf("%d, RN",n+1);
		}
		printf("\nBelady's Occurrance Counts: \n");
                printf("\tOP\tFF\tLR\tMR\tLF\tMF\tRN\n\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n",op_bel_count,ff_bel_count,lr_bel_count,mr_bel_count,lf_bel_count,mf_bel_count,rn_bel_count);
		
	}
	else {
		fprintf(stderr, "Syntax: PRABA -f <sfile> | -b <slength> <tnumber>\n");
                return -1;
	}
}

void           *
fifo ()
{
	int 	working_set[9] = { 0 };
	int 	fault_count[10] = { 0 };
	int 	dist[9] = { 0 };
	int 	n,m,o,p,tmp,index,max,bool1,bool2,bool3,slength=count;
	for (m=1;m<10;m++) {				
		p=0;
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
				working_set[p] = seq[n];
				p = (p+1)%m;
				fault_count[m] = fault_count[m]+=1;
			}
		}
	}
        for (m=1;m<10;m++) {
		ff_count[m]=fault_count[m];
	}
}
	
void           *
lr ()
{
	struct frame {
		int data;
		int ref;
	};
	struct frame	working_set[9];
	int 	fault_count[10] = { 0 };
	int 	dist[9] = { 0 };
	int 	n,m,o,index,incr,min,bool1,bool2,bool3,slength=count;
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
		lr_count[m] = fault_count[m];
	}
}

void           *
mr()
{
	struct frame {
		int data;
		int ref;
	};
	struct frame	working_set[9];
	int 	fault_count[10] = { 0 };
	int 	dist[9] = { 0 };
	int 	n,m,o,index,incr,max,bool1,bool2,bool3,slength=count;
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
				max=working_set[0].ref;
				for(o=0;o<m;o++) {
					if (working_set[o].ref >= max) {
						max = working_set[o].ref;
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
		mr_count[m] = fault_count[m];
	}
}

void           *
lf ()
{
	int 	working_set[9] = { 0 };
	int 	used_count[10] = { 0 };
	int 	fault_count[10] = { 0 };
	int 	n,m,o,index,min,bool1,bool2,slength=count;

	for (m=1;m<10;m++) {				
		for(o=0;o<m;o++) {
			working_set[o]=0;
		}
		for(o=1;o<=9;o++) 
			used_count[o]=0;
		for (n = 0; n<slength; n++){
			bool1 = 0;
			bool2 = 0;
		 	for (o=0;o<m;o++) {
				if (seq[n] == working_set[o]) {
					working_set[o] = seq[n];
					used_count[working_set[o]]++;
					bool1 = 1;	
					bool2 = 1;
					break;
				}
			}
			if (bool1 == 0) {
				for(o=0;o<m;o++) {
					if (working_set[o] == 0) {
	               		                working_set[o] = seq[n];
						used_count[working_set[o]]++;
						fault_count[m] = fault_count[m]+=1;
						bool2 = 1;
						break;
					}
				}
			}
			if (bool2 == 0) {
				index=0;
				min = used_count[working_set[0]];
				for(o=0;o<m;o++) { 
					if ( min >= used_count[working_set[o]] ) {
						min = used_count[working_set[o]];
						index = o;
					}
				}
				working_set[index] = seq[n];
				used_count[working_set[o]]++;
				fault_count[m] = fault_count[m]+=1;
			}
		}
	}
        for (m=1;m<10;m++) {
		lf_count[m] = fault_count[m];	
	}
}

void           *
mf ()
{
	int 	working_set[9] = { 0 };
	int 	used_count[10] = { 0 };
	int 	fault_count[10] = { 0 };
	int 	n,m,o,index,max,bool1,bool2,slength=count;

	for (m=1;m<10;m++) {				
		for(o=0;o<m;o++) {
			working_set[o]=0;
		}
		for(o=1;o<=9;o++) 
			used_count[o]=0;
		for (n = 0; n<slength; n++){
			bool1 = 0;
			bool2 = 0;
		 	for (o=0;o<m;o++) {
				if (seq[n] == working_set[o]) {
					working_set[o] = seq[n];
					used_count[working_set[o]]++;
					bool1 = 1;	
					bool2 = 1;
					break;
				}
			}
			if (bool1 == 0) {
				for(o=0;o<m;o++) {
					if (working_set[o] == 0) {
	               		                working_set[o] = seq[n];
						used_count[working_set[o]]++;
						fault_count[m] = fault_count[m]+=1;
						bool2 = 1;
						break;
					}
				}
			}
			if (bool2 == 0) {
				index=0;
				max = used_count[working_set[0]];
				for(o=0;o<m;o++) { 
					if ( max <= used_count[working_set[o]] ) {
						max = used_count[working_set[o]];
						index = o;
					}
				}
				working_set[index] = seq[n];
				used_count[working_set[o]]++;
				fault_count[m] = fault_count[m]+=1;
			}
		}
	}
        for (m=1;m<10;m++) {
		mf_count[m] = fault_count[m];	
	}
}

void           *
rn ()
{
	int 	working_set[9] = { 0 };
	int 	fault_count[10] = { 0 };
	int 	n,m,o,r,bool1,bool2,slength=count;

	for (m=1;m<10;m++) {				
		for(o=0;o<m;o++) {
			working_set[o]=0;
		}
                srand(time(NULL));

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
                                r = (rand() % m);
				working_set[r] = seq[n];
				fault_count[m] = fault_count[m]+=1;
			}
		}
	}
        for (m=1;m<10;m++) {
		rn_count[m] = fault_count[m];	
	}
}

void           *
optimal ()
{
	int 	working_set[9] = { 0 };
	int 	fault_count[10] = { 0 };
	int 	dist[9] = { 0 };
	int 	n,m,o,p,tmp,index,max,bool1,bool2,bool3,slength=count;
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
		op_count[m] = fault_count[m];			
	}
}
				

