CC = g++
# CPPFLAGS = -g -O3 -std=c++11
CPPFLAGS = -g -static -O3 -std=c++11 -Wl,--no-as-needed ## Should fix threading problem on ubuntu
MPSLIB=mpslib/mpslib.a
# CPPFLAGS = -g -static -O3 -std=c++11 -Wl,--no-as-needed ## Should fix threading problem on ubuntu
# CPPFLAGS = -g -static -O3 -std=c++11
# CPPFLAGS = -g -O3 -std=c++11
LDLIBS =  -lstdc++ -lpthread

all: $(MPSLIB) mps_genesim mps_snesim_list mps_snesim_tree

$(MPSLIB):
	cd mpslib; make; cd ..

mps_genesim:
	$(CC) $(CPPFLAGS) mps_genesim.cpp ENESIM_GENERAL.cpp mpslib/mpslib.a -o $@ -I mpslib/ $(LDLIBS)

mps_snesim_tree:
	$(CC) $(CPPFLAGS) mps_snesim_tree.cpp SNESIMTree.cpp mpslib/mpslib.a -o $@ -I mpslib/ $(LDLIBS)

mps_snesim_list:
	$(CC) $(CPPFLAGS) mps_snesim_list.cpp SNESIMList.cpp mpslib/mpslib.a -o $@ -I mpslib/ $(LDLIBS)

remake:
	make clean
	make all

clean:
	rm -f *.o mps *.exe mps_genesim mps_snesim_tree mps_snesim_list mpslib/*.o $(MPSLIB)
