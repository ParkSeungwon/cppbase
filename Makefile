all : 
	incltouch.x
	make -C src/
	make -C tst/
	make -C obj/
	./catch.x

PHONY : clean

clean :
	rm obj/*.?
	rm *.x

