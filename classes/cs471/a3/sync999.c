#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#define MAX_BUFF_SIZE 20 

/* The mutex lock */
pthread_mutex_t produce_mutexes[MAX_BUFF_SIZE];
pthread_mutex_t consume_mutexes[MAX_BUFF_SIZE];
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;

int thread_c = 0;

int n = 0;
struct slot{
	int state;
	int thread;
};

struct slot buff[MAX_BUFF_SIZE];		

main(int argc, char *argv[])
{
	if (argc == 1)
                n = 9;
        else
                n = atoi(argv[1]);

	int i;
	pthread_t       producer_thread;
	pthread_t       consumer_thread;
	void           *producer(void * ptr);
	void           *consumer(void * ptr);

	for (i = 0; i < n; i++){
		pthread_mutex_init(&consume_mutexes[i], NULL);
		pthread_mutex_init(&produce_mutexes[i], NULL);
		pthread_mutex_lock(&consume_mutexes[i]);        
	}	
	for (i = 1; i <= n; i++){
		pthread_mutex_lock(&running_mutex);
		thread_c++;
		printf ("STARTING PRODUCER THREAD %d\n",i);
		pthread_mutex_unlock(&running_mutex);
		pthread_create(&consumer_thread, NULL, consumer, &i);
	}
	for (i = 1; i <=n; i++){
		pthread_mutex_lock(&running_mutex);
                thread_c++;
		printf ("STARTING CONSUMER THREAD %d\n",i);
                pthread_mutex_unlock(&running_mutex);
		pthread_create(&producer_thread, NULL, producer, &i);
	}
	
	while (thread_c > 0){
		sleep(n);
		printf("\n\n\tBuffer STATE:\n");
		printf("\t\tIndex:\n");
	/**	for (i = 0; i < n; i++){
			char buf[100];
			sprintf(buf, sizeof buf, "6%d", i);
		}
		printf("%s\n\t\tState:");
		for (i = 0; i < n; i++){
                        char buf[100];
                        sprintf(buf, sizeof buf, "6%d", buff[i].state);
                }
                printf("%s\n\t\tThread:");
		for (i = 0; i < n; i++){
                        char buf[100];
                        sprintf(buf, sizeof buf, "6%d", buff[i].thread);
                }
	**/
	}
	printf ("\nDisplay Final State\n");
}	
		

void           *
producer(void * ptr)
{
	int		id = *(int *) ptr;
	int             item = 1;	
	int 		i;
		for (item; item<=n; item++){
			for (i = 0; i<n; i++){
				if (buff[i].state == 0){
					pthread_mutex_lock(&produce_mutexes[i]);     //acquire the mutex lock
                			buff[i].state = 1;		// CRITICAL SECTION
					buff[i].thread = pthread_self();
                                        pthread_mutex_unlock(&consume_mutexes[i]);   //release the mutex lock
					srand(time(NULL));
		                        int r = (rand() % n+1);
					sleep(r);  
					buff[i].state = 2;
					buff[i].thread = 0;
					printf("Producer %d produced item %d at Slot %d in %d seconds.\n",id, item,i,r);
			                fflush(stdout);
					break;
				}
				else{
	                                int r = (rand() % n+1);
        	                        sleep(r);
				}
			}
		}
	pthread_mutex_lock(&running_mutex);
   	thread_c--;
	pthread_mutex_unlock(&running_mutex);
	pthread_exit(NULL);
}
void           *
consumer(void * ptr)
{
        int             id = *(int*) ptr;
	int             item = 1;
        int             i;
                for (item; item<=n; item++){
                        for (i = 0; i<n; i++){
                                if (buff[i].state == 2){
                                        pthread_mutex_lock(&consume_mutexes[i]);     //acquire the mutex lock
                                        buff[i].state = 3;              // CRITICAL SECTION
                                        buff[i].thread = pthread_self();
                                        pthread_mutex_unlock(&produce_mutexes[i]);   //release the mutex lock
					srand(time(NULL));
                                        int r = (rand() % n);
                                        sleep(r);
                                        buff[i].state = 0;
                                        buff[i].thread = 0;
                                        printf("Consumer %d consumed item %d at Slot %d in %d seconds. \n",id, item,i,r);
					fflush(stdout);
					break;
                                }
                                else{
                                        int r = (rand() % n);
                                        sleep(r);
                                }
                        }
                }
	pthread_mutex_lock(&running_mutex);
	thread_c--;
	pthread_mutex_unlock(&running_mutex);
        pthread_exit(NULL);
}

