CC=g++
CPPFLAGS = -g -static -O3 -std=c++11 -Wl,--no-as-needed 
INCLUDES = -I.
SRC_DIR = .

# variant 1
SRCS ?= $(shell find $(SRC_DIR) -name '*.cpp' | sed "s/[^\/]*\/[^\/]*\///")
# variant 2
# SRCS = Utility.cpp IO.cpp Coords3D.cpp Coords4D.cpp MPSAlgorithm.cpp SNESIM.cpp ENESIM.cpp

%.o : $(SRC_DIR)/%.cpp
	$(CC) -o $@ $(INCLUDES) $(CPPFLAGS) -c $^

all : mpslib.a

mpslib.a : $(SRCS:.cpp=.o) 
	$(AR) rsv $@ $^

clean:
	rm -f *.o *.a

