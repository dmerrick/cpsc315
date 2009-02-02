/* 
 *  mypipe.c - emulates cmd1 | cmd2 | ... | cmdn
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int fd[2];
  // fd[0] is for reading
  // fd[1] is for writing

	if (pipe(fd) == -1) {
		perror("Pipe");
		exit(1);
	}

	switch (fork()) {
	case -1:
		perror("Fork");
		exit(2);
	case 0:				/* In the child */
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execl("/usr/bin/who", "who", (char *) 0);
		exit(3);
	default:				/* In the parent */
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execl("/usr/bin/wc", "wc", (char *) 0);
		exit(4);
	}	
}
