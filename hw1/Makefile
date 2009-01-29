CC=gcc 
CFLAGS=-Wall -O

palin: palin.o
	${CC} -o palin ${CFLAGS} palin.o

palin1: palin1.o
	${CC} -o palin1 ${CFLAGS} palin1.o

palin2: palin2.o
	${CC} -o palin2 ${CFLAGS} palin2.o

all: palin palin1 palin2

clean:
	rm -f palin palin1 palin2 *.o *~

