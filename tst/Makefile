CFLAG = -g -std=c++14 -fmax-errors=1 -pthread
LDFlag = -lzezeon -lpthread -lmysqlcppconn
INCLUDE = -I../
CC = g++
OBJ = $(wildcard ../obj/*.o)
MAIN_OBJ = $(patsubst %.cpp, ../%.x, $(wildcard *.cpp))

all : $(MAIN_OBJ)

../%.x : %.cpp
	$(CC) $(INCLUDE) $(CFLAG) $< -o $@ $(LDFLAG)
