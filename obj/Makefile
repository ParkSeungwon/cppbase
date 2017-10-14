CC = g++
OBJ = $(wildcard *.o)
EXE = $(patsubst %.x, ../%.x, $(wildcard *.x))
TST = $(patsubst %.t, ../%.x, $(wildcard *.t))
gtkmmLIB = $(shell pkg-config gtkmm-3.0 --libs)

all : $(EXE)
test : $(TST)

../%.x : %.x $(OBJ)
	$(CC) -o $@ $< $(OBJ) -lzezeon -lmysqlcppconn $(gtkmmLIB)

../%.x : %.t $(OBJ)
	$(CC) -o $@ $< $(OBJ) -lzezeon -lmysqlcppconn $(gtkmmLIB)

