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

