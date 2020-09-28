INC=${PWD}/include
LIB=${PWD}/lib

get_time: get_time.o src_lib.o
	gcc -Wall -o get_time get_time.o src_lib.o
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
