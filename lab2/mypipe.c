/* 
 *  mypipe.c - emulates cmd1 | cmd2 | ... | cmdn
 *
 *  Currently only works for programs in the /usr/bin/ directory.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  // verify we have enough arguements
  if (argc < 3) {
    printf("usage: %s cmd1 cmd2 [...]\n", argv[0]);
    exit(2);
  }
  
  // file descriptor array is used for pipes
	int fd[2];
  // fd[0] is for reading
  // fd[1] is for writing

  // calling pipe() populates the fd array
	if (pipe(fd) == -1) {
		perror("Error creating pipe.");
		exit(1);
	}

  // this represents the number of times we will have to loop
  int loops = argc - 2;
  // the commands to execute, left and right side
  char lcmd[100], rcmd[100];

  // the current arguement indices we're using
  int i=1,j=2;

  while ( loops > 0 ) {
	  switch ( fork() ) {
	    case -1:
		    perror("Fork error.");
		    exit(2);
	    case 0:
		    dup2(fd[1], STDOUT_FILENO);
		    close(fd[0]);
		    close(fd[1]);

        // set up command to execute
        sprintf(lcmd,"/usr/bin/%s",argv[i]);
        // and execute it
		    execl(lcmd, argv[i], (char *) 0);

		    exit(3);
	    default:
		    dup2(fd[0], STDIN_FILENO);
		    close(fd[0]);
		    close(fd[1]);

        // set up command to execute
        sprintf(rcmd,"/usr/bin/%s",argv[j]);
        // and execute it
		    execl(rcmd, argv[j], (char *) 0);
  
		    exit(4);
	  }	

    // move the indices
    i++;
    j++;
    // decrement loops
    loops--;
  }
  return 0;
}
