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

  char *start_location = ".";

  // run on start location
  parse_dir(start_location);


  return 0;

}

void parse_dir( char *dir_name ) {
  DIR *dir_p;

  dir_p = opendir(dir_name); 

  // check that we opened correctly
  if ( dir_p == NULL ) {
      printf(stderr, "opendir() failed\n");
      exit(-1);
  }

  struct dirent *dirent_p;
  char *name;

  dirent_p = readdir( dir_p );
  while ( dirent_p != NULL ) {
    name = dirent_p->d_name;

    if (strcmp(name,".")!=0 && strcmp(name,"..")!=0) {
      printf( "%s\n", name );

      if (dirent_p->d_type == DT_DIR) {
        parse_dir(name);
      } 
    }

    // move to the next file in the dir
    dirent_p = readdir( dir_p );
  }

  closedir(dir_p);
} 
