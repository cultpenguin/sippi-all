OBJS = sortem.o chknam.o
CC = gfortran
CPPFLAGS = -g -O3 
LDFLAGS = -g
LDLIBS = -lstdc++ -lpthread 
PROG = snesim

snesim: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(PROG).f -o $(PROG) $(LDLIBS)

sortem.o:
	$(CC) $(CPPFLAGS) -c sortem.f -o sortem.o

chknam.o:
	$(CC) $(CPPFLAGS) -c chknam.f -o chknam.o



clean:
	rm -f *.o snesim

all: snesim




