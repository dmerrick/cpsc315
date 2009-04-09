#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


/**
 * Prints the full path of the current working directory.
 */
int main(int argc, char *argv[]) {

  // define needed variables
  DIR *dir_p;
  struct dirent *dirent_p;
  ino_t last_inode, parent_inode;
  char *dir_name;
  char *output="";

  // start setting up the loop variables...

  // open current directory
  dir_p = opendir("."); 

  // check that we opened correctly
  if ( dir_p == NULL ) {
      printf(stderr, "opendir() failed\n");
      exit(-1);
  }
  
  // read the current directory
  dirent_p = readdir(dir_p);
  // save the inode of the current directory
  parent_inode = dirent_p->d_ino;
  last_inode = NULL;

  // loop through the tree until we get to the root
  while( last_inode != parent_inode ) {

    // save the old parent inode for the next loop
    last_inode = parent_inode;
    //printf( "last_inode:\t%i\n", last_inode );

    // move up to the parent directory
    chdir("..");
    //system("pwd");


    // open current directory
    dir_p = opendir(".");
    
    // check that we opened correctly
    if ( dir_p == NULL ) {
        printf(stderr, "opendir() failed\n");
        exit(-1);
    }
  
    // read the info of the parent directory
    dirent_p = readdir( dir_p );
    // save the parent's parent's inode
    parent_inode = dirent_p->d_ino;
    //printf( "parent_inode:\t%i\n", parent_inode );


    // scan through current directory for last inode
    while ( dirent_p->d_ino != last_inode ) {
        dirent_p = readdir( dir_p );
    }
    // save the directory name of the last folder traversed
    dir_name = dirent_p->d_name;

    // print the directory name
    asprintf( &output, "%s/%s", dir_name, output);

    //printf( "%s: %i\n", dirent_p->d_name, dirent_p->d_ino );

    // close the directory
    closedir( dir_p );

  }

  printf("%s\n", output);
  return 0;

}


