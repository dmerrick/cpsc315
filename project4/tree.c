#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


/**
 * Prints a graphical view of the directory structure.
 */
int main(int argc, char *argv[]) {

  // define needed variables
  DIR *dir_p;
  char *dir_name;

  char *start_location = ".";

  // start setting up the loop variables...

  // open current directory
  dir_p = opendir(start_location); 

  parse_dir(dir_p);

  closedir(dir_p);

  return 0;

}

void parse_dir( DIR *dir_p) {
  // check that we opened correctly
  if ( dir_p == NULL ) {
      printf(stderr, "opendir() failed\n");
      exit(-1);
  }

  struct dirent *dirent_p;

  dirent_p = readdir( dir_p );
  while ( dirent_p != NULL ) {
      printf( "%s\n", dirent_p->d_name );
      dirent_p = readdir( dir_p );
  }

} 
