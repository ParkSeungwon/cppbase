SHELL := /bin/bash

all : 
	incltouch.x
	for i in `ls -d */`; do \
		if [ $$i == "obj/" ]; then continue; fi; \
		if [ -f $${i}/Makefile ]; then make -C $$i; fi;\
	done
	make -C obj/

clean :
	for i in obj/*.?; do \
		if [[ $$i != *.tst.x ]]; then rm $$i; fi; \
	done
	rm *.x

# I do not touch tst/catch.cpp.
# It is always same.
# incltouch will touch source files, according to inclusion of header files.
# incltouch generate tree of header inclusion.
# So it can track inclusion tree. 
# But there is one including rule.
# Any header file that is made by you and will be edited should use " ", not < >.
# And you should alwasy use directory structure.
# root dir - src directories.
# And you should not add include path except -I../
# In the end, the header inclusion will be like #include "src/header.h" when
# it is in the different directory.
# Or "header.h" when it is in the same directory.

