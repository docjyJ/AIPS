default : tsock_v1

all : tsock_v0 tsock_v1

tsock_v0: tsock_v0.o
	gcc -Wall -o $@ $^

tsock_v1: tsock_v1.o tsock_lib.o
	gcc -Wall -o $@ $^

clean :
	rm *.o tsock_v0 tsock_v1

%.o : %.c
	gcc -Wall -c $<
