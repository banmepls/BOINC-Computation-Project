# Makefile for catalan example

CXXFLAGS = -O2 -Wall -I../../sched -I../../db -I../../lib

PROGS = catalan_compute catalan_validate catalan_assimilate

all: $(PROGS)

catalan_compute: catalan_compute.cpp
	$(CXX) $(CXXFLAGS) -o catalan_compute catalan_compute.cpp

catalan_validate: catalan_validate.cpp
	$(CXX) $(CXXFLAGS) -o catalan_validate catalan_validate.cpp

catalan_assimilate: catalan_assimilate.cpp
	$(CXX) $(CXXFLAGS) -o catalan_assimilate catalan_assimilate.cpp

clean:
	rm -f $(PROGS) *.o