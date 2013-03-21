CC		=	gcc 
RM		= rm -rf 
CFLAGS		= $(NORMCFLAGS)	
LIBS       = -lsocket -lnsl  -lm -lrt -lpthread

PROGS = sync999

all: $(PROGS)

sync999:  sync999.o
	$(CC) $(CFLAGS) -o sync999 sync999.c $(LIBS)
clean:;
	$(RM) $(PROGS) *.o core 
