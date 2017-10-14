all : 
	make -C gtk/
	make -C src/
	make -C obj/
	make -C tst/

PHONY : clean

clean :
	rm obj/*.?

test :
	make -C tst/ test
