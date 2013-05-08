CC		=	gcc -g
JVC = javac
SOLARISHOME 	= /usr/dt
RM		= rm -rf 
CFLAGS		= $(NORMCFLAGS)	
JVCFLAGS =
LIBS       =  -lnsl  -lm -lsocket

.SUFFIXES : .class .java
.java.class :
	$(JVC) $(JVCFLAGS) $<

PROGS = ChatServer  ChatClient


all: $(PROGS)

ChatServer:  ChatServer.o
	$(CC) $(CFLAGS) -o ChatServer ChatServer.c $(LIBS)
ChatClient:  ChatClient.o
	$(CC) $(CFLAGS) -o ChatClient ChatClient.c $(LIBS)

clean:;
	$(RM) $(PROGS) *.class *.ln *.o core errs ,* *~ *.a .emacs_* tags \
	TAGS make.log MakeOut  "#"*
