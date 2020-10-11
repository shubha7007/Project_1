#ifeq("${OS}","Windows")
#ifdef OS
ifeq ($(OS),Windows_NT)
#PWD=C:\Users\shubham\lol\Project_1\project_1
#PWD = Get-Location
PWD:= $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
INC=${PWD}\include
LIB=${PWD}\lib
SRC=${PWD}\src

get_time: get_time.o src_lib.o
	gcc -Wall -o get_time get_time.o src_lib.o -lws2_32 


get_time.o: ${PWD}\src\get_time.c 
	gcc -I${INC} -c ${PWD}\src\get_time.c

src_lib.o : ${LIB}\src_lib.c 
	gcc -I${INC} -c ${PWD}\lib\src_lib.c

script:
	2.sh

clean:
	del get_time.exe
	del *.o

else
UNAME_S := $(shell uname -s)
endif

ifeq ($(UNAME_S),Linux)
INC=${PWD}/include
LIB=${PWD}/lib

get_time: get_time.o src_lib.o
	gcc -Wall -o get_time get_time.o src_lib.o
	mkdir build
	mv *.o ./build

get_time.o: ${PWD}/src/get_time.c 
	gcc -I${INC} -c ${PWD}/src/get_time.c

src_lib.o : ${LIB}/src_lib.c 
	gcc -I${INC} -c ${PWD}/lib/src_lib.c

clean:
	rm get_time
	rm -r ./build
endif
