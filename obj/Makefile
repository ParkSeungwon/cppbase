CC = g++
OBJ = $(wildcard *.o)
EXE = $(patsubst %.x, ../%.x, $(wildcard *.x))
TST = $(wildcard %.t)
LIBS = 

all : $(EXE) ../catch.x

../%.x : %.x $(OBJ)
	$(CC) -o $@ $< $(OBJ) $(LIBS)

../catch.x : catch.tx $(OBJ) $(TST)
	$(CC) -o $@ $< $(OBJ) $(TST)  $(LIBS)

