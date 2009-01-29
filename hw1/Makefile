CC=gcc 
CFLAGS=-Wall -O 

DICTFILE=/usr/share/dict/words

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
	rm -f palin palin1 palin2d1 palin2d32 palin2d256 palin2d1024 palin2d8192 *.o *~ *.out
package:
	tar czvf project1.tar.gz README Makefile *.c *.out

test:
	./palin $(DICTFILE) > palin.out
	./palin1 $(DICTFILE) > palin1.out
	./palin2d1 $(DICTFILE) > palin2d1.out
	./palin2d32 $(DICTFILE) > palin2d32.out
	./palin2d256 $(DICTFILE) > palin2d256.out
	./palin2d1024 $(DICTFILE) > palin2d1024.out
	./palin2d8192 $(DICTFILE) > palin2d8192.out

time:
	time ./palin $(DICTFILE) >/dev/null 2>&1
	time ./palin1 $(DICTFILE) >/dev/null 2>&1
	time ./palin2d1 $(DICTFILE) >/dev/null 2>&1
	time ./palin2d32 $(DICTFILE) >/dev/null 2>&1
	time ./palin2d256 $(DICTFILE) >/dev/null 2>&1
	time ./palin2d1024 $(DICTFILE) >/dev/null 2>&1
	time ./palin2d8192 $(DICTFILE) >/dev/null 2>&1
	time grep -q '^\(.\)\(.\).\2\1$$' $(DICTFILE)

