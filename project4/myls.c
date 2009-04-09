#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// method signature
void print_file( struct dirent* );

/**
 * Prints a graphical view of the directory structure.
 */
int main(int argc, char *argv[]) {

  // directory to list
  char *dir_name;

  DIR *dir_p;
  struct dirent *dirent_p;
  char *name;


  // check if we provided an arguement
  if( argc < 2 ) {
    // use the current dir
    dir_name = ".";
  } else {
    // use the provided dir
    dir_name = argv[1];
  }


  dir_p = opendir(dir_name); 

  // check that we opened correctly
  if ( dir_p == NULL ) {
      fprintf(stderr, "opendir() failed\n");
      exit(-1);
  }

  dirent_p = readdir( dir_p );

  while ( dirent_p != NULL ) {
    name = dirent_p->d_name;

    // print the name
    print_file(dirent_p);

    // move to the next file in the dir
    dirent_p = readdir( dir_p );
  }

  closedir(dir_p);

  // exit cleanly
  return 0;

}

void print_file( struct dirent *dirent_p) {
  char *name = dirent_p->d_name;
  printf( "%s\n", name );
}

