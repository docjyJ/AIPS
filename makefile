default : tsock

all : tsock debug

debug: tsock.o tsock_error.o
	gcc -Wall -g -o $@ $^

tsock: tsock.o tsock_error.o
	gcc -Wall -o $@ $^

clean :
	rm *.o tsock debug

%.o : %.c
	gcc -Wall -c $<
