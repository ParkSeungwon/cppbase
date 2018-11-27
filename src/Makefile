CFLAG = -g -c -std=c++17 -fmax-errors=1
INCLUDE = -I../ -I/usr/include -I/usr/include/python2.7 -I/usr/include/jsoncpp
CC = g++
OBJ = $(patsubst %.cc, ../obj/%.o, $(wildcard *.cc))
MAIN_OBJ = $(patsubst %.cpp, ../obj/%.x, $(wildcard *.cpp))

all : $(OBJ) $(MAIN_OBJ) $(OOBJ)

../obj/%.o : %.cc 
	$(CC) $(INCLUDE) $(CFLAG) -fPIC $< -o $@

../obj/%.x : %.cpp
	$(CC) $(INCLUDE) $(CFLAG) $< -o $@
