CXX = g++
PKGS = 
CPPFLAGS = -I../ -MMD -MP
CXXFLAGS = -g -c -std=c++23 -fmax-errors=1 $(shell pkg-config --cflags $(PKGS))
DIR = $(notdir $(shell pwd))
OBJ = $(patsubst %.cc, ../obj/%.$(DIR).o, $(wildcard *.cc))
MAIN_OBJ = $(patsubst %.cpp, ../obj/%.$(DIR).x, $(wildcard *.cpp))
DEPS = $(OBJ:.o =.d) $(MAIN_OBJ:.o =.d)

all : $(OBJ) $(MAIN_OBJ) $(OOBJ)

../obj/%.$(DIR).o : %.cc 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

../obj/%.$(DIR).x : %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

-include: $(DEPS)
