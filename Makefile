OBJS = sortem.o chknam.o snesim.o
CC = gfortran
CFLAGS = -O3 -static
#CFLAGS = -g -O3 
LDLIBS = 
PROG = snesim

snesim: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG) $(LDLIBS)

snesim.o: snesim.f
	$(CC) $(CFLAGS) -c snesim.f -o snesim.o

sortem.o: sortem.f
	$(CC) $(CFLAGS) -c sortem.f -o sortem.o

chknam.o: chknam.f
	$(CC) $(CFLAGS) -c chknam.f -o chknam.o



clean:
	rm -f *.o snesim

all: snesim




