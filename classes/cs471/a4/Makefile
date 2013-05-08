CC		=	gcc 
RM		= rm -rf 
CFLAGS		= $(NORMCFLAGS)	
LIBS       = -lnsl  -lm -lrt -lpthread

PROGS = PRABA 

all: $(PROGS)

PRABA:  PRABA.o
	$(CC) $(CFLAGS) -o PRABA PRABA.c $(LIBS)
clean:;
	$(RM) $(PROGS)  *.o core 
