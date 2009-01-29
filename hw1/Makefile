CC=gcc 
CFLAGS=-Wall -O 

D1=-Dcase1

palin: palin.c
	$(CC) $(CFLAGS) -o palin palin.c

palin1: palin1.c
	$(CC) $(CFLAGS) -o palin1 palin1.c

palin2d1: palin2.c
	$(CC) $(CFLAGS) -Dcase1 -o palin2d1 palin2.c

palin2d32: palin2.c
	$(CC) $(CFLAGS) -Dcase32 -o palin2d32 palin2.c

palin2d256: palin2.c
	$(CC) $(CFLAGS) -Dcase256 -o palin2d256 palin2.c

palin2d1024: palin2.c
	$(CC) $(CFLAGS) -Dcase1024 -o palin2d1024 palin2.c

palin2d8192: palin2.c
	$(CC) $(CFLAGS) -Dcase8192 -o palin2d8192 palin2.c

all: palin palin1 palin2d1 palin2d32 palin2d256 palin2d1024 palin2d8192

clean:
	rm -f palin palin1 palin2d1 palin2d32 palin2d256 palin2d1024 palin2d8192 *.o *~

