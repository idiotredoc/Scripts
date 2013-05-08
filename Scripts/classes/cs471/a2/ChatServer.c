#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <strings.h>
#include <string.h>

#define SA      struct sockaddr

void            exit();
void            *EchoToClient(void *ptr);
void		*Information(void *ptr);
int all_connfd[10];
int ports[10];
int addrs[10];
int i;

int main(int argc, char *argv[])
{
	if (argc != 2){
                printf("Usage Syntax:ChatServer <port>\n");
                return -1;
        }

	int             len, listenfd;
	struct sockaddr_in servaddr, cliaddr;
	int 		port;
	i=0;

	pthread_t information;
	pthread_t chat_threads;
	pthread_attr_t	attr, attrinfo;

	pthread_attr_init(&attrinfo);

	port = atoi(argv[1])+1;
	if (pthread_create(&information, &attrinfo, &Information, (void *) port ) == -1){
                        perror("Info Thread Creation failed\n");
                        exit(-1);
                }
	
	port--;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	if (bind(listenfd, (SA *) & servaddr, sizeof(servaddr)) == -1){
            perror("bind failed\n");
            exit(-1);
        }
	listen(listenfd, 0);

	pthread_attr_init(&attr);

	for(;;){
		len = sizeof(cliaddr);
		all_connfd[i] = accept(listenfd, (SA *) & cliaddr, &len);
		addrs[i]=inet_ntoa(cliaddr.sin_addr);
		ports[i]=ntohs(cliaddr.sin_port);
		printf("Chat Connection from: %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
		if (pthread_create(&chat_threads, &attr, &EchoToClient, (void *) all_connfd[i] ) == -1){
			perror("Chat Thread Creation failed\n");
			exit(-1);
		}
		i++;
	}
	/**
	for(i = 0; i < MAX_THREADS; ++i) {
		pthread_join(chat_threads[i], NULL);

	}

	pthread_join(information, NULL);
	**/
}

void *EchoToClient(void *ptr){
	int 		r;
	int             nread;
	char            buffer[512];
	for(;;){
		nread = read((int) ptr, buffer, 512);
		if (nread == 0){
			pthread_exit(0);
		}
		for(r=0;r<i;r++){
			write(all_connfd[r], buffer, nread);
		}
	}
}

void *Information(void *ptr){
	int prt = (int)ptr;
	char	buff[512];
	int x;
        int             len, listenfd2, connfd2;
        struct sockaddr_in servaddr2, cliaddr2;
        
	listenfd2 = socket(AF_INET, SOCK_STREAM, 0);
        bzero(&servaddr2, sizeof(servaddr2));
	servaddr2.sin_family = AF_INET;
        servaddr2.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr2.sin_port = htons(prt);
        if (bind(listenfd2, (SA *) & servaddr2, sizeof(servaddr2)) == -1){
            perror("bind failed\n");
            exit(-1);
        }
        listen(listenfd2, 0);
	for(;;){
	len = sizeof(cliaddr2);
       	connfd2 = accept(listenfd2, (SA *) & cliaddr2, &len);
	printf("Info Connection from: %s:%d\n", inet_ntoa(cliaddr2.sin_addr), ntohs(cliaddr2.sin_port));
	for(x=0;x<i;x++){
		snprintf(buff, sizeof(buff),"%s%s:%d\n",buff,addrs[x],ports[x]);
	}		
      	write(connfd2, buff, strlen(buff)+1);
	close(connfd2);
	snprintf(buff,sizeof(buff),"");
	}
	pthread_exit(0);
}
