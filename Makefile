OBJS = sortem.o chknam.o
CC = gfortran
CFLAGS = -O3 
#CFLAGS = -g -O3 
LDLIBS = 
PROG = snesim

snesim: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(PROG).f -o $(PROG) $(LDLIBS)

sortem.o:
	$(CC) $(CFLAGS) -c sortem.f -o sortem.o

chknam.o:
	$(CC) $(CFLAGS) -c chknam.f -o chknam.o



clean:
	rm -f *.o snesim

all: snesim




