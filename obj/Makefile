CC = g++
OBJ = $(wildcard *.o)
OOBJ = $(wildcard *.O)
TOBJ = $(wildcard *.t)
EXE = $(patsubst %.x, ../%.x, $(wildcard *.x))
gtkmmLIB = $(shell pkg-config gtkmm-3.0 --libs)
LIBS = -lpthread -lboost_python -ljsoncpp -lgmpxx -lgmp -lwolfssl

all : $(EXE) ../catch.x ../tls_crypt.so

../%.x : %.x $(OBJ)
	$(CC) -o $@ $< $(OBJ) $(LIBS)

../tls_crypt.so : $(OBJ)
	$(CC) -shared -o $@ $(OBJ) $(LIBS) 

../catch.x : catch.tx $(TOBJ) $(OOBJ)
	$(CC) -o $@ $< $(TOBJ) $(LIBS)
