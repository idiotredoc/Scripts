//Nathan Lutz CS471 Operating Systems
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <time.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <fcntl.h>

#define SA struct sockaddr
typedef char * string;

int main(int argc, char* argv[]) 
{
//	srand(time(NULL));

	pid_t pid;
	pid_t pidS;
	pid_t pidD;
        pid_t pidSarr[7];
        pid_t pidDarr[7];

	char date[10] = "Today";
        int stat;
        int stimer;  //son time counter
	int dtimer; //daughter timer
	int pcount = 1;	//Pair counter
	int pidcount = 0; //PID Array Index

	/** Initialize Shared Memory **/
	
	string ipc[4];
	ipc[0] =  "shared memory";
	ipc[1] =  "sockets";
	ipc[2] =  "pipe";
	ipc[3] =  "shared file";

	int             segment_id,segment_id2,segment_id3;
	char           *shared_memory="Nathan Lutz";
	const int       segment_size = 4096;

	/** allocate  a shared memory segment */
	segment_id = shmget(IPC_PRIVATE, segment_size, S_IRUSR | S_IWUSR);

	/** attach the main shared memory segment */
	shared_memory = (char *) shmat(segment_id, NULL, 0);

	/** shared memeory to keep track of time**/
	int r;
	int 		*shm_time1;
	int           *shm_time2;

	segment_id2 = shmget(IPC_PRIVATE, segment_size, S_IRUSR | S_IWUSR);
	segment_id3 = shmget(IPC_PRIVATE, segment_size, S_IRUSR | S_IWUSR);
	
	shm_time1 = (int *) shmat(segment_id2, NULL, 0);
	shm_time2 = (int *) shmat(segment_id3, NULL, 0);

	/** Set up Socket **/
	int             len, listenfd, connfd;
	struct sockaddr_in servaddr, cliaddr;
	char            buff[512];
	time_t          ticks;

	int             sockfd, n;
	char            recvline[512];
	struct hostent *hp, *gethostbyname();

	//** Set up pipe**/
	char buf[100];
	int pfd[2];
	pipe(pfd);
	char msg[64];

	//** set up shared file**//
	int sfd;
	int sfd2;
	char buffer[1024];

	//argument checking
	if (argc != 2)
	{
		printf("Usage Syntax:\nFourPairs [-s | -c]\n");
		return -1;
	}

	//Concurrent version
	if (strcmp(argv[1], "-c") == 0)
	{
                for (pcount; pcount <=4; pcount++)
                {       
                         /* fork 2 children process */
                        if (pidSarr[pidcount] = fork()) // Parent
                        {
                                if (pidDarr[pidcount] = fork()) // Parent
                                {
                                        pid = getpid();
                                        printf ("\nPair P%d: Using %s.\n",pcount,ipc[pidcount]);
                                        printf ("Created Son %d of Pair P%d.\n",pidSarr[pidcount],pcount);
                                        printf ("Created Daughter %d of Pair P%d.\n",pidDarr[pidcount],pcount);
                                }
                                else    // Daughter
                                {
                                        pid = getpid();
					switch ( pidcount )
					{
                                        case 0:  //Shared Memory
                                                sleep(1); //wait 1 before read shm
                                                printf ("%s\n", shared_memory);
                                                sleep(2);       //Sleep for 4 secs
                                                ticks = time(NULL);
                                                sprintf(shared_memory, "I am fine my Brother.\nYour sister %d\n%.24s\r\n",pid,ctime(&ticks));
						srand(time(NULL));
						r = (rand() % 6 + 4);
                                                shm_time1[0] = (r + 3);
                                                sleep(r);     //Sleep for random time > 4 < 10s
                                                break;
                                        case 1: //Sockets
                                                sockfd = socket(AF_INET, SOCK_STREAM, 0);

                                                bzero(&servaddr, sizeof(servaddr));
                                                hp = gethostbyname( "localhost" );
                                                bcopy ( hp->h_addr, &(servaddr.sin_addr.s_addr), hp->h_length);

                                                servaddr.sin_family = AF_INET;
                                                servaddr.sin_port = htons(11777);

                                                if (connect(sockfd, (SA *) & servaddr, sizeof(servaddr)) < 0)
                                                {
                                                        perror("connect error");
                                                        exit(-1);
                                                }
                                                sleep(1);
                                                read(sockfd, recvline, 512);
                                                printf("%s\n", recvline);
                                                sleep(2);
                                                ticks = time(NULL);
                                                snprintf(buff, sizeof(buff), "I am fine my brother.\nYour sister %d\n%.24s\r\n", pid,ctime(&ticks));
                                                write(sockfd, buff, strlen(buff));
                                                close(sockfd);
						srand(time(NULL));
                                                r = (rand() % 6 + 4);
                                                shm_time1[1] = (r + 3);
                                                sleep(r);     //Sleep for random time > 4 < 10s
                                                break;
                                        case 2: //Pipe
                                                sleep(1);
                                                n=read(pfd[0], buf, 100);
                                                write(1, buf, n);
                                                sleep(2);
                                                ticks = time(NULL);
                                                sprintf(msg, "I am fine my brother.\nYour sister %d\n%.24s\r\n", pid,ctime(&ticks));

                                                write(pfd[1], msg, strlen(msg)+1);
						srand(time(NULL));
                                                r = (rand() % 6 + 4);
                                                shm_time1[2] = (r + 3);
                                                sleep(r);     //Sleep for random time > 4 < 10s
                                                break;

                                        case 3: //Shared File
                                                sleep(1);
                                                if ((sfd2 = open("/tmp/nlutztmp", O_RDWR)) < 0)
                                                {
                                                        perror("TempFile Error");
                                                        exit(1);
                                                }
                                                n = read(sfd2, buffer, sizeof(buffer));
                                                write(1, buffer, n);                                                                                
						sleep(2);
                                                ticks = time(NULL);
                                                sprintf(msg, "I am fine my brother.\nYour sister %d\n%.24s\r\n", pid,ctime(&ticks));

                                                write(sfd2, msg, strlen(msg)+1);
						srand(time(NULL));
                                                r = (rand() % 6 + 4);
                                                shm_time1[3] = (r + 3);
                                                sleep(r);     //Sleep for random time > 4 < 10s

                                                break;
                                        default:
                                                printf ("Default Case\n");
                                                break;
					}
                                        return 0;
                                }
                        }
                        else // Son
                        {
                                pid = getpid();
				switch (pidcount )
                                {
                                case 0:    
					ticks = time(NULL);
                                        sprintf(shared_memory, "How are you my sister?\nYour brother %d\n%.24s\r\n",pid,ctime(&ticks));
					sleep(4);	//Sleep for 4 secs
					printf ("%s\n", shared_memory);
					srand(time(NULL));
                                                r = (rand() % 10);
                                                shm_time2[0] =(r + 4);
                                                sleep(r);     //Sleep for random time < 10s
                                                break;
                                        case 1:
                                                listenfd = socket(AF_INET, SOCK_STREAM, 0);
                                                bzero(&servaddr, sizeof(servaddr));
                                                servaddr.sin_family = AF_INET;
                                                servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
                                                servaddr.sin_port = htons(11777);

                                                if ( bind(listenfd, (SA *) & servaddr, sizeof(servaddr)) < 0)
                                                {
                                                        perror("binding the socket\n");
                                                        exit(-1);
                                                }
                                                listen(listenfd, 0);

                                                        len = sizeof(cliaddr);
                                                        connfd = accept(listenfd, (SA *) & cliaddr, &len);

                                                        ticks = time(NULL);
                                                        snprintf(buff, sizeof(buff), "How are you my sister?\nYour brother %d\n%.24s\r\n", pid,ctime(&ticks));
                                                        write(connfd, buff, strlen(buff));
                                                        sleep(4);
                                                        read(connfd, recvline, 512);
                                                        printf("%s", recvline);
                                                        close(connfd);
						srand(time(NULL));
                                                r = (rand() % 10);
                                                shm_time2[1] =(r + 4);
                                                sleep(r);     //Sleep for random time < 10s
                                                break;
                                        case 2:
                                                ticks = time(NULL);
                                                sprintf(msg, "How are you my sister?\nYour brother %d\n%.24s\r\n\n", pid,ctime(&ticks));
                                                write(pfd[1], msg, strlen(msg)+1);
                                                sleep(4);
                                                n=read(pfd[0], buf, 100);
                                                write(1, buf, n);
						srand(time(NULL));
                                                r = (rand() % 10);
                                                shm_time2[2] =(r + 4);
                                                sleep(r);     //Sleep for random time < 10s
                                                break;
                                        case 3:
                                                ticks = time(NULL);
                                                if ((sfd = open("/tmp/nlutztmp", O_RDWR | O_TRUNC | O_CREAT, 0600)) < 0)                                    {
                                                        perror("TempFile Error");
                                                        exit(1);
                                                }
                                                sprintf(msg, "How are you my sister?\nYour brother %d\n%.24s\r\n\n", pid,ctime(&ticks));
                                                write(sfd, msg, strlen(msg)+1);
                                                sleep(4);
                                                n = read(sfd, buffer, sizeof(buffer));
                                                write(1, buffer, n);
						srand(time(NULL));
                                                r = (rand() % 10);
                                                shm_time2[3] =(r + 4);
                                                sleep(r);     //Sleep for random time < 10s
                                        break;
				default:
                                                printf ("Default Case\n");
                                                break;

                                }
                                return 0;
                        }
			pidcount+=1;
                }
                
		while ( (pid = waitpid(-1, &stat, 0)) > 0)
	        {
        		if (pid == pidDarr[0])
                		printf ("\nDaughter %d of Pair P1 EXITED after (%d) seconds\n", pid,shm_time1[0]);
	                else if (pid == pidDarr[1])
                                printf ("\nDaughter %d of Pair P2 EXITED after (%d) seconds\n", pid,shm_time1[1]);
			else if (pid == pidDarr[2])
                                printf ("\nDaughter %d of Pair P3 EXITED after (%d) seconds\n", pid,shm_time1[2]);
			else if (pid == pidDarr[3])
                                printf ("\nDaughter %d of Pair P4 EXITED after (%d) seconds\n", pid,shm_time1[3]);
			else if (pid == pidSarr[0])
        	                printf ("\nSon %d of Pair P1 EXITED after (%d) seconds\n", pid,shm_time2[0]);
			else if (pid == pidSarr[1])
                                printf ("\nSon %d of Pair P2 EXITED after (%d) seconds\n", pid,shm_time2[1]);
			else if (pid == pidSarr[2])
                                printf ("\nSon %d of Pair P3 EXITED after (%d) seconds\n", pid,shm_time2[2]);
			else if (pid == pidSarr[3])
                                printf ("\nSon %d of Pair P4 EXITED after (%d) seconds\n", pid,shm_time2[3]);
        	} 
		//Detach from shm
		if (shmdt(shared_memory) == -1) {
			fprintf(stderr, "Unable to detach\n");
		}
		/** now remove the shared memory segment */
		shmctl(segment_id, IPC_RMID, NULL);
		return 0;
	}
	
	//Sequential version
	else if (strcmp(argv[1], "-s") == 0)
	{
		for (pcount; pcount <=4; pcount++)
		{	
			 /* fork 2 children process */
	                if (pidSarr[pidcount] = fork()) // Parent
        	        {
                	        if (pidDarr[pidcount] = fork()) // Parent
                        	{
	                                pid = getpid();
        	                        printf ("\n\nPair P%d: Using %s.\n",pcount,ipc[pidcount]);
                	                printf ("Created Son %d of Pair P%d.\n",pidSarr[pidcount],pcount);
                        	        printf ("Created Daughter %d of Pair P%d.\n\n",pidDarr[pidcount],pcount);
			                while ( (pid = waitpid(-1, &stat, 0)) > 0)
       				        {
		                        if (pid == pidDarr[0])
		                                printf ("\nDaughter %d of Pair P1 EXITED after (%d) seconds\n", pid,shm_time1[0]);
        		                else if (pid == pidDarr[1])
                		                printf ("\nDaughter %d of Pair P2 EXITED after (%d) seconds\n", pid,shm_time1[1]);
        	        	        else if (pid == pidDarr[2])
	                        	        printf ("\nDaughter %d of Pair P3 EXITED after (%d) seconds\n", pid,shm_time1[2]);
		                        else if (pid == pidDarr[3])
        		                        printf ("\nDaughter %d of Pair P4 EXITED after (%d) seconds\n", pid,shm_time1[3]);
                		        else if (pid == pidSarr[0])
                        		        printf ("\nSon %d of Pair P1 EXITED after (%d) seconds\n", pid,shm_time2[0]);
		                        else if (pid == pidSarr[1])
        		                        printf ("\nSon %d of Pair P2 EXITED after (%d) seconds\n", pid,shm_time2[1]);
	        	                else if (pid == pidSarr[2])
        	        	                printf ("\nSon %d of Pair P3 EXITED after (%d) seconds\n", pid,shm_time2[2]);
	                	        else if (pid == pidSarr[3])
        	                	        printf ("\nSon %d of Pair P4 EXITED after (%d) seconds\n", pid,shm_time2[3]);
                                	}
	                        }
        	                else    // Daughter
                	        {
					srand(time(NULL));
                        	        pid = getpid();
                                        switch ( pidcount )
                                        {
                                        case 0:  //Shared Memory
                                                sleep(1); //wait 1 before read shm
						printf ("%s\n", shared_memory);
						sleep(2);       //Sleep for 4 secs
						ticks = time(NULL);
                                                sprintf(shared_memory, "I am fine my Brother.\nYour sister %d\n%.24s\r\n",pid,ctime(&ticks));
						r = (rand() % 6 + 4);
						shm_time1[0] = (r + 3);
                                                sleep(r);     //Sleep for random time > 4 < 10s
                                                break;
                                        case 1: //Sockets
                                                sockfd = socket(AF_INET, SOCK_STREAM, 0);

                                                bzero(&servaddr, sizeof(servaddr));
                                                hp = gethostbyname( "localhost" );
                                                bcopy ( hp->h_addr, &(servaddr.sin_addr.s_addr), hp->h_length);

                                                servaddr.sin_family = AF_INET;
                                                servaddr.sin_port = htons(11777);

                                                if (connect(sockfd, (SA *) & servaddr, sizeof(servaddr)) < 0)
                                                {
                                                        perror("connect error");
                                                        exit(-1);
                                                }
                                                sleep(1);
						read(sockfd, recvline, 512);
						printf("%s\n", recvline);
						sleep(2);
						ticks = time(NULL);
						snprintf(buff, sizeof(buff), "I am fine my brother.\nYour sister %d\n%.24s\r\n", pid,ctime(&ticks));
                                                write(sockfd, buff, strlen(buff));
                                                close(sockfd);
                                                r = (rand() % 6 + 4);
                                                shm_time1[1] = (r + 3);
                                                sleep(r);     //Sleep for random time > 4 < 10s                                                
						break;
                                        case 2: //Pipe
						sleep(1);
						n=read(pfd[0], buf, 100);
						write(1, buf, n);
						sleep(2);
						ticks = time(NULL);
						sprintf(msg, "I am fine my brother.\nYour sister %d\n%.24s\r\n", pid,ctime(&ticks));
						write(pfd[1], msg, strlen(msg)+1);
                                                r = (rand() % 6 + 4);
                                                shm_time1[2] = (r + 3);
                                                sleep(r);     //Sleep for random time > 4 < 10s
                                                break;
                                        case 3: //Shared File
						sleep(1);
						if ((sfd2 = open("/tmp/nlutztmp", O_RDWR)) < 0) 
						{
							perror("TempFile Error");
							exit(1);
						}
						n = read(sfd2, buffer, sizeof(buffer));
						write(1, buffer, n);										    
						sleep(2);
						ticks = time(NULL);
						sprintf(msg, "I am fine my brother.\nYour sister %d\n%.24s\r\n", pid,ctime(&ticks));

                                                write(sfd2, msg, strlen(msg)+1);
                                                r = (rand() % 6 + 4);
                                                shm_time1[3] = (r + 3);
                                                sleep(r);     //Sleep for random time > 4 < 10s
                                                break;
                                        default:
                                                printf ("Default Case\n");
                                                break;
                                        }
                	                return 0;	
                        	}
	                }
        	        else // Son
	                {
				srand(time(NULL));
        	                pid = getpid();
                                switch (pidcount )
                                        {
                                        case 0:
						ticks = time(NULL);
                                                sprintf(shared_memory, "How are you my sister?\nYour brother %d\n%.24s\r\n",pid,ctime(&ticks));
                                                sleep(4);       //Sleep for 4 secs
                                                printf ("%s\n", shared_memory);
						
						r = (rand() % 10);
                                                shm_time2[0] =(r + 4);
                                                sleep(r);     //Sleep for random time < 10s
                                                break;
                                        case 1:
                                                listenfd = socket(AF_INET, SOCK_STREAM, 0);
                                                bzero(&servaddr, sizeof(servaddr));
                                                servaddr.sin_family = AF_INET;
                                                servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
                                                servaddr.sin_port = htons(11777);

                                                if ( bind(listenfd, (SA *) & servaddr, sizeof(servaddr)) < 0)
                                                {
                                                        perror("binding the socket\n");
                                                        exit(-1);
                                                }
                                                listen(listenfd, 0);

                                                        len = sizeof(cliaddr);
                                                        connfd = accept(listenfd, (SA *) & cliaddr, &len);

                                                        ticks = time(NULL);
                                                        snprintf(buff, sizeof(buff), "How are you my sister?\nYour brother %d\n%.24s\r\n", pid,ctime(&ticks));
                                                        write(connfd, buff, strlen(buff));
							sleep(4);
							read(connfd, recvline, 512);
							printf("%s", recvline);
                                                        close(connfd);
                                                r = (rand() % 10);
                                                shm_time2[1] =(r + 4);
                                                sleep(r);     //Sleep for random time < 10s
                                                break;
                                        case 2:
						ticks = time(NULL);
						sprintf(msg, "How are you my sister?\nYour brother %d\n%.24s\r\n\n", pid,ctime(&ticks));		
						write(pfd[1], msg, strlen(msg)+1);
						sleep(4);
						n=read(pfd[0], buf, 100);
						write(1, buf, n);
                                                r = (rand() % 10);
                                                shm_time2[2] =(r + 4);
                                                sleep(r);     //Sleep for random time < 10s
                                                break;
                                        case 3:
						ticks = time(NULL);
						if ((sfd = open("/tmp/nlutztmp", O_RDWR | O_TRUNC | O_CREAT, 0600)) < 0) 				    {
							perror("TempFile Error");
							exit(1);
						}
						sprintf(msg, "How are you my sister?\nYour brother %d\n%.24s\r\n\n", pid,ctime(&ticks));
						write(sfd, msg, strlen(msg)+1);
						sleep(4);
						n = read(sfd, buffer, sizeof(buffer));
						write(1, buffer, n);
                                                r = (rand() % 10);
                                                shm_time2[3] =(r + 4);
                                                sleep(r);     //Sleep for random time < 10s
                                                break;
					default:
                                                printf ("Default Case\n");
                                                break;

                                        }
	                        return 0;
        	        }
			pidcount +=1;
		}	
		//Detach from shm
                if (shmdt(shared_memory) == -1) {
                        fprintf(stderr, "Unable to detach\n");
                }
                /** now remove the shared memory segment */
                shmctl(segment_id, IPC_RMID, NULL);
                return 0;

		//Detach from shm
                if (shmdt(shm_time1) == -1) {
                        fprintf(stderr, "Unable to detach\n");
                }
                /** now remove the shared memory segment */
                shmctl(segment_id2, IPC_RMID, NULL);
                return 0;

		//Detach from shm
                if (shmdt(shm_time2) == -1) {
                        fprintf(stderr, "Unable to detach\n");
                }
                /** now remove the shared memory segment */
                shmctl(segment_id3, IPC_RMID, NULL);
                return 0;

	}
	else
	{
		printf ("Invalid option!");
		return -1;
	}
}
