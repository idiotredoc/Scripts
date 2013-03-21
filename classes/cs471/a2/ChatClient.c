#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include        <time.h>
#include <pthread.h>
#include <signal.h>
#include <strings.h>
#include <string.h>

#define SA      struct sockaddr

void *read_from_stdin(void *);
void SIGhandler(int);
void            exit();
int prt;
char* host;

main(int argc, char **argv)
{
	int             sockfd;
	struct sockaddr_in servaddr;
	struct hostent *hp, *gethostbyname();
	char            buffer[512];
	int             nread;
	signal(SIGINT,SIG_IGN);
	pthread_t tid;
	pthread_attr_t attr;

	if(argc!=3){
		printf("Usage: ChatClient <host> <port>\n");
		exit(-1);
	}
	prt = atoi(argv[2]);
	host = argv[1];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	hp = gethostbyname(argv[1]);
	bcopy(hp->h_addr, &(servaddr.sin_addr.s_addr), hp->h_length);

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(prt);
	prt++;

	if (connect(sockfd, (SA *) & servaddr, sizeof(servaddr)) < 0) {
		perror("connect error");
		exit(-1);
	}
	
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, &read_from_stdin, (void *) sockfd);
	
	for (;;) {
		nread = read(sockfd, buffer, 512);
		if (nread == 0)
			exit(0);
		write(1, buffer, nread);
	}
	pthread_join(tid,NULL);
}

void *read_from_stdin(void *ptr){
	int 	nread;
	char	buffer[512];
	signal(SIGINT,SIGhandler);
	for (;;) {
                nread = read(0, buffer, 512);
                if (nread == 0)
                        exit(0);
                write((int)ptr, buffer, nread);
        }
}

void SIGhandler(int signl){
	signal(signl, SIGhandler);
	int connfd;
	struct sockaddr_in servaddr;
 	struct hostent *hp, *gethostbyname();
	char buffer[512];

        connfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
        hp = gethostbyname(host);
        bcopy(hp->h_addr, &(servaddr.sin_addr.s_addr), hp->h_length);
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(prt);
        if (connect(connfd, (SA *) & servaddr, sizeof(servaddr)) < 0) {
                perror("connect error");
                exit(-1);
        }

        read(connfd, buffer, sizeof(buffer));
        psignal(signl, "\nReceived sigHandler signal");
        printf("%s\n",buffer);
}

