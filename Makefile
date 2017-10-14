all : 
	make -C gtk/
	make -C src/
	make -C obj/
	make -C test/

PHONY : clean

clean :
	rm obj/*.o obj/*.x *.x

ls :
	echo $(EXE)
