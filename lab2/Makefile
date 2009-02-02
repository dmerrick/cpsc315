CC=gcc 
CFLAGS=-Wall -O

all: proc forktest parent child whowc unix_pipe

proc: proc.c
	${CC} -o proc ${CFLAGS} proc.c

forktest: forktest.c
	${CC} -o forktest ${CFLAGS} forktest.c

parent: parent.c
	${CC} -o parent ${CFLAGS} parent.c

child: child.c
	${CC} -o child ${CFLAGS} child.c

whowc: whowc.c
	${CC} -o whowc ${CFLAGS} whowc.c

unix_pipe: unix_pipe.c
	${CC} -o unix_pipe ${CFLAGS} unix_pipe.c

clean:
	rm -f *.o proc forktest parent child whowc unix_pipe
