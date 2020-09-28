get_time.o = get_time.c

src_lib.o = src_lib.c

get_time: get_time.o src_lib.o
	gcc -Wall -o get_time get_time.o src_lib.o
run:
	./get_time
clean:
	rm get_time *.o
