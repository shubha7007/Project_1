#ifeq("${OS}","Windows")
#ifdef OS
ifeq ($(OS),Windows_NT)
PWD=C:\shubham\delete_it\Project_1\project_1
INC=${PWD}\include
LIB=${PWD}\lib
SRC=${PWD}\src

get_time: get_time.o src_lib.o
	gcc -Wall -o get_time get_time.o src_lib.o -lws2_32 


get_time.o: ${PWD}\src\get_time.c 
	gcc -I${INC} -c ${PWD}\src\get_time.c

src_lib.o : ${LIB}\src_lib.c 
	gcc -I${INC} -c ${PWD}\lib\src_lib.c

endif

UNAME_S := $(shell uname -s)
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

run:
	./get_time
install:
	sudo cp get_time /usr/local/bin
	sudo cp ${INC}/get_time.h /usr/local/include
	sudo cp ${INC}/config.h /usr/local/include
	sudo chmod 777 /usr/local/include/config.h
clean:
	rm -r ./build/*.o
	rm get_time
endif
