CC=gcc 
CFLAGS=-Wall -O

all: mypwd tree

mypwd: mypwd.c
	${CC} -o mypwd ${CFLAGS} mypwd.c

tree: tree.c
	${CC} -o tree ${CFLAGS} tree.c

clean:
	rm -f *.o mypwd tree
