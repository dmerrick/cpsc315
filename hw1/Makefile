CC=gcc 
CFLAGS=-Wall -O

palin: palin.o
	${CC} -o palin ${CFLAGS} palin.o

palin1: palin1.o
	${CC} -o palin1 ${CFLAGS} palin1.o

all: palin palin1

clean:
	rm -f palin palin1 *.o *~

