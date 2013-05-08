#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <time.h>

#define MAX_BUFF_SIZE 30 

/* The mutex lock */
pthread_mutex_t produce_mutexes[MAX_BUFF_SIZE];
pthread_mutex_t consume_mutexes[MAX_BUFF_SIZE];
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pp_mutex = PTHREAD_MUTEX_INITIALIZER;	// To keep producer and consumer threads from printing at same time as main
pthread_mutex_t cp_mutex = PTHREAD_MUTEX_INITIALIZER;

// Record-keeping 
int prod_buff_slots[MAX_BUFF_SIZE][MAX_BUFF_SIZE];
int cons_buff_slots[MAX_BUFF_SIZE][MAX_BUFF_SIZE];
int prod_times[MAX_BUFF_SIZE][MAX_BUFF_SIZE];
int cons_times[MAX_BUFF_SIZE][MAX_BUFF_SIZE];
int prod_life[MAX_BUFF_SIZE];
int cons_life[MAX_BUFF_SIZE];

int thread_c = 0;
int n = 0;
struct slot{
	int state;
	int thread;
};
struct slot buff[MAX_BUFF_SIZE];		

main(int argc, char *argv[])
{
	time_t start, stop;
	time(&start); 		// record elapsed time

	if (argc == 1)
                n = 9;
        else
                n = atoi(argv[1]);

	const char * states[4];
	states[0] = "F";
	states[1] = "P";
	states[2] = "R";
	states[3] = "C";

	int i;
	int j;
	pthread_t       producer_thread;
	pthread_t       consumer_thread;
	void           *producer(void * ptr);
	void           *consumer(void * ptr);

	//Initialize various mutexes and arrays
	for (i = 0; i < n; i++){
		pthread_mutex_init(&consume_mutexes[i], NULL);
		pthread_mutex_init(&produce_mutexes[i], NULL);
		pthread_mutex_lock(&consume_mutexes[i]);        
		for (j = 0; j < n; j++){
			prod_buff_slots[i][j]=0;
			cons_buff_slots[i][j]=0;
			prod_times[i][j]=0;
			cons_times[i][j]=0;
		}
		prod_life[i]=0;
		cons_life[i]=0;
	}	
	for (i = 1; i <= n; i++){
		pthread_mutex_lock(&running_mutex);
		thread_c++;
		printf ("STARTING PRODUCER THREAD %d\n",i);
		pthread_mutex_unlock(&running_mutex);
		pthread_create(&consumer_thread, NULL, consumer, (void *)i);
	}
	for (i = 1; i <=n; i++){
		pthread_mutex_lock(&running_mutex);
                thread_c++;
		printf ("STARTING CONSUMER THREAD %d\n",i);
                pthread_mutex_unlock(&running_mutex);
		pthread_create(&producer_thread, NULL, producer, (void *)i);
	}
	
	while (thread_c > 0){
		sleep(n);
		pthread_mutex_lock(&pp_mutex);
                pthread_mutex_lock(&cp_mutex);
		printf("\n\n\tBuffer STATE:\n");
		printf("\tIndex: ");
		for (i = 0; i < n; i++){
			printf("\t%d", i);
		}
		printf("\n\tState: ");
		for (i = 0; i < n; i++){
			if ( buff[i].state == 0)
				printf("\t%s",states[0]);
			else if( buff[i].state == 1)
                                printf("\t%s",states[1]);
		        else if( buff[i].state == 2)
                                printf("\t%s",states[2]);
			else if( buff[i].state == 3)
                                printf("\t%s",states[3]);
                }
                printf("\n\tThread:");
		for (i = 0; i < n; i++){
                        printf("\t%d", buff[i].thread);
                }
		printf("\n\n");
		pthread_mutex_unlock(&pp_mutex);
                pthread_mutex_unlock(&cp_mutex);

	}
	printf ("\nDisplay Final State\n");

	printf ("\nProduction Buffer SLOTS:\n");
        printf("\tIndex: ");
        for (i = 1; i <= n; i++){
                printf("\t%d", i);
        }
        printf("\n");
        for (i = 0; i < n; i++){
                printf("\tP%d",i+1);
                for (j = 0; j < n; j++){
                        printf("\t%d",prod_buff_slots[i][j]);
                }
                printf("\n");
        }

        printf ("\nConsumption Buffer SLOTS:\n");
        printf("\tIndex: ");
        for (i = 1; i <= n; i++){
                printf("\t%d", i);
        }
        printf("\n");
        for (i = 0; i < n; i++){
                printf("\tC%d",i+1);
                for (j = 0; j < n; j++){
                        printf("\t%d",cons_buff_slots[i][j]);
                }
                printf("\n");
        }

	printf ("\nProduction TIMES in seconds:\n");
	printf("\tIndex: ");
        for (i = 1; i <= n; i++){
                printf("\t%d", i);
        }
	printf("\n");
	for (i = 0; i < n; i++){
		printf("\tP%d",i+1);
		for (j = 0; j < n; j++){
			printf("\t%d",prod_times[i][j]);
		}
		printf("\n");
	}

	printf ("\nConsumption TIMES in seconds:\n");
        printf("\tIndex: ");
        for (i = 1; i <= n; i++){
                printf("\t%d", i);
        }
        printf("\n");
        for (i = 0; i < n; i++){
                printf("\tC%d",i+1);
                for (j = 0; j < n; j++){
                        printf("\t%d",cons_times[i][j]);
                }
                printf("\n");
        }
	printf ("\nProducers LIFE SPAN in seconds:\n");
        printf("\tIndex: ");
        for (i = 1; i <= n; i++){
                printf("\t%d", i);
        }
	printf("\n\t");
	for (i = 0; i < n; i++){
		printf("\t%d", prod_life[i]);
	}
	printf("\n");

	printf ("\nConsumers LIFE SPAN in seconds:\n");
        printf("\tIndex: ");
        for (i = 1; i <= n; i++){
                printf("\t%d", i);
        }
        printf("\n\t");
        for (i = 0; i < n; i++){
                printf("\t%d", cons_life[i]);
        }
        printf("\n");
	
	time(&stop);
	printf("Total Time of All: %.2f seconds\n", difftime(stop,start));	
}	
		

void           *
producer(void * ptr)
{
	int		id = (int) ptr;
	int             item = 1;	
	int 		i;
	do{
		for (i = 0; i<n; i++){
			if (buff[i].state == 0){
				pthread_mutex_lock(&produce_mutexes[i]);
               			buff[i].state = 1;		// CRITICAL SECTION				  
				buff[i].thread = id;
                                pthread_mutex_unlock(&consume_mutexes[i]);   //release the mutex lock
				srand(time(NULL));
				int r = (rand() % n)+1;
				prod_life[id-1]=r+prod_life[id-1];
				prod_buff_slots[id-1][item-1]=i;
				prod_times[id-1][item-1]=r;
                                sleep(r);
				buff[i].state = 2;
				buff[i].thread = 0;
		                pthread_mutex_lock(&pp_mutex);
				printf("Producer %d produced item %d at Slot %d in %d seconds.\n",id, item,i,r);
		                pthread_mutex_unlock(&pp_mutex);
				item++;
		                fflush(stdout);
				break;
			}
			else{
	                        int r = (rand() % n)+1;
				prod_life[id-1]+=r;
                                sleep(r);
			}
		}
	}while(item<=n);
	pthread_mutex_lock(&running_mutex);
   	thread_c--;
	pthread_mutex_unlock(&running_mutex);
        printf("<<< Producer %d is DONE after %d seconds >>>\n",id,prod_life[id-1]);
	pthread_exit(NULL);
}
void           *
consumer(void * ptr)
{
        int             id = (int) ptr;
	int             item = 1;
        int             i;
        do{        
        	for (i = 0; i<n; i++){
                	if (buff[i].state == 2){
				pthread_mutex_lock(&consume_mutexes[i]);
                                buff[i].state = 3;              // CRITICAL SECTION
                                buff[i].thread = id;
                                pthread_mutex_unlock(&produce_mutexes[i]);   //release the mutex lock
				srand(time(NULL));
                                int r = (rand() % n)+1;
				cons_life[id-1]=r+cons_life[id-1];
                                cons_buff_slots[id-1][item-1]=i;
				cons_times[id-1][item-1]=r;
                                sleep(r);
                                buff[i].state = 0;
                                buff[i].thread = 0;
		                pthread_mutex_lock(&cp_mutex);
                                printf("Consumer %d consumed item %d at Slot %d in %d seconds. \n",id, item,i,r);
		                pthread_mutex_unlock(&cp_mutex);
				item++;
				fflush(stdout);
				break;
                        }
                        else{
                                int r = (rand() % n)+1;
				cons_life[id-1]+=r;
                                sleep(r);
                        }
                }
        }while(item<=n);
	pthread_mutex_lock(&running_mutex);
	thread_c--;
	pthread_mutex_unlock(&running_mutex);
	printf("<<< Consumer %d is DONE after %d seconds >>>\n",id,cons_life[id-1]);
        pthread_exit(NULL);
}

