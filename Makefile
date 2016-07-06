CC = g++
# CPPFLAGS = -g -O3 -std=c++11
CPPFLAGS = -g -static -O3 -std=c++11 -Wl,--no-as-needed ## Should fix threading problem on ubuntu
MPSLIB=mpslib/mpslib.a
# CPPFLAGS = -g -static -O3 -std=c++11 -Wl,--no-as-needed ## Should fix threading problem on ubuntu
# CPPFLAGS = -g -static -O3 -std=c++11
# CPPFLAGS = -g -O3 -std=c++11
LDLIBS =  -lstdc++ -lpthread

all: mps_genesim mps_snesim_list mps_snesim_tree

.PHONY: mpslib
mpslib:
	make -C mpslib

mps_genesim: mpslib
	$(CC) $(CPPFLAGS) mps_genesim.cpp ENESIM_GENERAL.cpp $(MPSLIB) -o $@ -I mpslib/ $(LDLIBS)

mps_snesim_tree: mpslib
	$(CC) $(CPPFLAGS) mps_snesim_tree.cpp SNESIMTree.cpp $(MPSLIB) -o $@ -I mpslib/ $(LDLIBS)

mps_snesim_list: mpslib
	$(CC) $(CPPFLAGS) mps_snesim_list.cpp SNESIMList.cpp $(MPSLIB) -o $@ -I mpslib/ $(LDLIBS)


.PHONY: clean
clean:
	rm -f *.o mps *.exe mps_genesim mps_snesim_tree mps_snesim_list mpslib/*.o $(MPSLIB)
