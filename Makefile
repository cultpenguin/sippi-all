CC = g++
# CPPFLAGS = -g -O3 -std=c++11
CPPFLAGS = -g -static -O3 -std=c++11 -Wl,--no-as-needed ## Should fix threading problem on ubuntu
# CPPFLAGS = -g -static -O3 -std=c++11 -Wl,--no-as-needed ## Should fix threading problem on ubuntu
# CPPFLAGS = -g -static -O3 -std=c++11
# CPPFLAGS = -g -O3 -std=c++11
LDLIBS =  -lstdc++ -lpthread


OBJS_mps_genesim = utility.o io.o coords3d.o mpsalgorithm.o enesim.o enesim_general.o mps_genesim.o
OBJS_mps_snesim_list = utility.o io.o coords3d.o coords4d.o mpsalgorithm.o snesim.o snesimlist.o mps_snesim_list.o
OBJS_mps_snesim_tree = utility.o io.o coords3d.o coords4d.o mpsalgorithm.o snesim.o snesimtree.o mps_snesim_tree.o


# MPS CLASS OBJECTS
#mps_class_objects: $(OBJS_mps)
#	$(CC) $(CPPFLAGS) $(OBJS) -o mps_class_objects $(LDLIBS)

# PROGRAMS
mps_genesim: $(OBJS_mps_genesim)
	$(CC) $(CPPFLAGS) $(OBJS_mps_genesim) -o mps_genesim $(LDLIBS)


mps_snesim_list: $(OBJS_mps_snesim_list)
	$(CC) $(CPPFLAGS) $(OBJS_mps_snesim_list) -o mps_snesim_list $(LDLIBS)

mps_snesim_tree: $(OBJS_mps_snesim_tree)
	$(CC) $(CPPFLAGS) $(OBJS_mps_snesim_tree) -o mps_snesim_tree $(LDLIBS)

# MAIN OBJECTS
mps_genesim.o: Coords3D.h ENESIM.h ENESIM_GENERAL.h MPSAlgorithm.h Utility.h IO.h mps_genesim.cpp
		$(CC) $(CPPFLAGS) -c mps_genesim.cpp -o mps_genesim.o

mps_enesim_metropolis.o: Coords3D.h ENESIM.h ENESIM_METROPOLIS.h MPSAlgorithm.h Utility.h IO.h mps_enesim_metropolis.cpp
		$(CC) $(CPPFLAGS) -c mps_enesim_metropolis.cpp -o mps_enesim_metropolis.o

mps_snesim_list.o: Coords3D.h SNESIMList.h SNESIM.h MPSAlgorithm.h Utility.h IO.h mps_snesim_list.cpp
	$(CC) $(CPPFLAGS) -c mps_snesim_list.cpp -o mps_snesim_list.o

mps_snesim_tree.o: Coords3D.h SNESIMTree.h SNESIM.h MPSAlgorithm.h Utility.h IO.h mps_snesim_tree.cpp
	$(CC) $(CPPFLAGS) -c mps_snesim_tree.cpp -o mps_snesim_tree.o

# OBJECTS

# THE MAIN MPS CLASS
mpsalgorithm.o: Coords3D.h MPSAlgorithm.h Utility.h IO.h MPSAlgorithm.cpp
	$(CC) $(CPPFLAGS) -c MPSAlgorithm.cpp -o mpsalgorithm.o

# THE TWO MAIN TYPES OF ALGORITHMS
enesim.o: Coords3D.h ENESIM.h MPSAlgorithm.h Utility.h IO.h ENESIM.cpp
	$(CC) $(CPPFLAGS) -c ENESIM.cpp -o enesim.o

snesim.o: Coords3D.h Coords4D.h SNESIM.h MPSAlgorithm.h Utility.h IO.h SNESIM.cpp
	$(CC) $(CPPFLAGS) -c SNESIM.cpp -o snesim.o

# UTILITIES
coords3d.o : Coords3D.h Coords3D.cpp
	$(CC) $(CPPFLAGS) -c Coords3D.cpp -o coords3d.o

coords4d.o : Coords3D.h Coords4D.h Coords4D.cpp
	$(CC) $(CPPFLAGS) -c Coords4D.cpp -o coords4d.o

io.o: IO.h IO.cpp
	$(CC) $(CPPFLAGS) -c IO.cpp -o io.o

utility.o: Utility.h Utility.cpp
	$(CC) $(CPPFLAGS) -c Utility.cpp -o utility.o


# THE ALGORITHMS
enesim_general.o: Coords3D.h ENESIM.h ENESIM_GENERAL.h MPSAlgorithm.h Utility.h IO.h ENESIM_GENERAL.cpp
	$(CC) $(CPPFLAGS) -c ENESIM_GENERAL.cpp -o enesim_general.o

snesimlist.o: Coords3D.h Coords4D.h SNESIMList.h SNESIM.h MPSAlgorithm.h Utility.h IO.h SNESIMList.cpp
	$(CC) $(CPPFLAGS) -c SNESIMList.cpp -o snesimlist.o

snesimtree.o: Coords3D.h Coords4D.h SNESIMTree.h SNESIM.h MPSAlgorithm.h Utility.h IO.h SNESIMTree.cpp
	$(CC) $(CPPFLAGS) -c SNESIMTree.cpp -o snesimtree.o


clean:
	rm -f *.o mps *.exe mps_genesim mps_snesim_tree mps_snesim_list

all: mps_genesim mps_snesim_list mps_snesim_tree
