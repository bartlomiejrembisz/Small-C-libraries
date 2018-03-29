#
# Created by nas, 09/11/16
#

INCLUDEDIR = ../headers/
CXXFLAGS   = -std=c++11 -I $(INCLUDEDIR) -Wall -Wfatal-errors

vpath %.h $(INCLUDEDIR)

all: position.o parseNMEA.o

position.o : position.cpp position.h
	g++ -c $(CXXFLAGS) position.cpp -o position.o

parseNMEA.o : parseNMEA.cpp parseNMEA.h position.h
	g++ -c $(CXXFLAGS) parseNMEA.cpp -o parseNMEA.o

clean:
	rm -f position.o parseNMEA.o
