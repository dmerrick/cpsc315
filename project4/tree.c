#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void parse_dir(char*, int);

/**
 * Prints a graphical view of the directory structure.
 */
int main(int argc, char *argv[]) {

  // root of the tree
  char *start_location;

  // check if we provided an arguement
  if( argc < 2 ) {
    // use the current dir
    start_location = ".";
  } else {
    // use the provided dir
    start_location = argv[1];
  }

  // run on start location
  parse_dir(start_location,0);

  // exit cleanly
  return 0;

}

/**
 * Recursively prints an indented directory structure of the current dir.
 * @param name of directory to parse
 * @param level of indentation to use
 */
void parse_dir( char *dir_name, int indent ) {
  DIR *dir_p;

  dir_p = opendir(dir_name); 

  // check that we opened correctly
  if ( dir_p == NULL ) {
      fprintf(stderr, "opendir() failed\n");
      exit(-1);
  }

  struct dirent *dirent_p;
  char *name;
  int i;

  dirent_p = readdir( dir_p );

  while ( dirent_p != NULL ) {
    name = dirent_p->d_name;

    if (strcmp(name,".")!=0 && strcmp(name,"..")!=0) {

      // add indents
      for( i=0; i<indent; i++)
        printf("  ");

      // print the name
      printf( "%s\n", name );

      // recurse if we found a directory
      if (dirent_p->d_type == DT_DIR)
        parse_dir(name,indent+1);

    }

    // move to the next file in the dir
    dirent_p = readdir( dir_p );
  }

  closedir(dir_p);
} 
