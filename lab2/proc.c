#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	printf("Hi.\n");
	fork();
	printf("Bye.\n");
	exit(0);
}
