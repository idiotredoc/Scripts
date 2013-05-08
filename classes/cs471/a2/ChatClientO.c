#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <signal.h>
#define SA struct sockaddr

void *ReadFromKeyBoard(void *);
void ReadFromSocket();
void exit();
void SIGhandler(int);
int port;
char* host;

int main(int argc, char **argv){


    if(argc!=3){
     printf("Usage: %s <host> <port>\n",argv[0]);
     exit(-1);
    }

    signal(SIGINT,SIG_IGN);
        int sockfd, connfd;
        struct sockaddr_in servaddr;
        struct hostent *hp, *gethostbyname();
    pthread_t tid;
    pthread_attr_t attr;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        bzero(&servaddr, sizeof(servaddr));
        hp = gethostbyname(argv[1]);
        bcopy(hp->h_addr, &(servaddr.sin_addr.s_addr), hp->h_length);

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(atoi(argv[2]));
    port = atoi(argv[2]);
    port+=1;
    host = argv[1];

        if (connect(sockfd, (SA *) & servaddr, sizeof(servaddr)) < 0) {
                perror("connect error");
                exit(-1);
        }

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, &ReadFromKeyBoard, (void *)sockfd);
        ReadFromSocket(sockfd);
    pthread_join(tid,NULL);
}
void *ReadFromKeyBoard(void *sockfd){

        int             nread;
        char            buffer[512];
    signal(SIGINT, SIGhandler);
        for (;;) {
                nread = read(0, buffer, 512);
                if (nread == 0)
                        break;
                write((int)sockfd, buffer, nread);
        }
}
void ReadFromSocket(int sockfd){

        int             nread;
        char            buffer[512];
        for (;;) {
                nread = read(sockfd, buffer, 512);
                if (nread == 0)
                        break;
                write(1, buffer, nread);
        }
}
void SIGhandler(int sig){
    signal(sig, SIGhandler);
    int connifd;
    struct sockaddr_in servaddr;
    struct hostent *hp, *gethostbyname();
    char buffer[512];

        connifd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    hp = gethostbyname(host);
    bcopy(hp->h_addr, &(servaddr.sin_addr.s_addr), hp->h_length);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
        if (connect(connifd, (SA *) & servaddr, sizeof(servaddr)) < 0) {
                perror("connect error");
                exit(-1);
        }

    read(connifd, buffer, sizeof(buffer));
    psignal(sig, "\nReceived sigHandler signal");
    printf("%s\n",buffer);
}
