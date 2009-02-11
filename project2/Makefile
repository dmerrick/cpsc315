CC=gcc 
CFLAGS=-Wall -O

all: client1 client2 client3 client4 server1 server2 server3 server4 ftpserver ftpclient

client1: client1.c
	${CC} -o client1 ${CFLAGS} client1.c

client2: client2.c
	${CC} -o client2 ${CFLAGS} client2.c

client3: client3.c
	${CC} -o client3 ${CFLAGS} client3.c

client4: client4.c
	${CC} -o client4 ${CFLAGS} client4.c

server1: server1.c
	${CC} -o server1 ${CFLAGS} server1.c

server2: server2.c
	${CC} -o server2 ${CFLAGS} server2.c

server3: server3.c
	${CC} -o server3 ${CFLAGS} server3.c

server4: server4.c
	${CC} -o server4 ${CFLAGS} server4.c

ftpserver: ftpserver.c
	${CC} -o ftpserver ${CFLAGS} ftpserver.c

ftpclient: ftpclient.c
	${CC} -o ftpclient ${CFLAGS} ftpclient.c

clean:
	rm -f *.o client1 client2 client3 client4 server1 server2 server3 server4 ftpserver ftpclient
