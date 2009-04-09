CC=gcc 
CFLAGS=-Wall -O

all: mypwd tree myls

mypwd: mypwd.c
	${CC} -o mypwd ${CFLAGS} mypwd.c

tree: tree.c
	${CC} -o tree ${CFLAGS} tree.c

myls: myls.c
	${CC} -o myls ${CFLAGS} myls.c

clean:
	rm -f *.o mypwd tree myls
