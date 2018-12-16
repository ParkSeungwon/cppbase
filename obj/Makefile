CC = g++
SRC = $(wildcard *.src.o)
EXE = $(patsubst %.x, ../%.x, $(wildcard *.x))
TST = $(wildcard *.tst.o)
LIBS = -lnettle -lwolfssl

all : $(EXE) ../catch.x

../%.x : %.x $(SRC)
	$(CC) -o $@ $^ $(LIBS)

../catch.x : catch.tx $(SRC) $(TST)
	$(CC) -o $@ $^ $(LIBS)

#    $@ |현재 목표 파일의 이름
#    $* | 확장자를 제외한 현재 목표 파일의 이름
#    $< | 현재 필수 조건 파일 중 첫 번째 파일 이름
#    $? | 현재 대상보다 최슨에 변경된 함수 조건 파일 이름
#    $^ | 현재 모든 필수 조건 파일들

