CC=gcc-8
CXX=g++-8

CFLAGS=-O2 -Wall
AVXFLAGS=-mavx -msse2
PTHREADFLAGS=-pthread

SRC=src
BIN=bin

COMMONSRC=$(SRC)/dotprod.cpp $(SRC)/chol.cpp $(SRC)/print.cpp $(SRC)/tictoc.cpp $(SRC)/equal.cpp

all: chol sandbox

chol:
	$(CXX) $(COMMONSRC) $(SRC)/main.cpp -o $(BIN)/chol $(CFLAGS) $(AVXFLAGS) $(PTHREADFLAGS)

sandbox:
	$(CXX) $(COMMONSRC) $(SRC)/sandbox.cpp -o $(BIN)/sandbox $(CFLAGS) $(AVXFLAGS) $(PTHREADFLAGS)

.PHONY: clean

clean:
	rm -f $(BIN)/*