#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

// method signature
void print_file( struct dirent* );
int file_select( struct dirent* );

/**
 * Prints a graphical view of the directory structure.
 */
int main(int argc, char *argv[]) {

  // directory to list
  char *dir_name;

  // directory listing
  struct dirent **dir_list;

  int count, i;


  // check if we provided an arguement
  if( argc < 2 ) {
    // use the current dir
    dir_name = ".";
  } else {
    // use the provided dir
    dir_name = argv[1];
  }

  // read directory listing into array
  count = scandir(dir_name, &dir_list, file_select, alphasort);

  // loop through each file
  for (i=1; i<count+1; ++i)
    print_file(dir_list[i-1]);

  // exit cleanly
  return 0;

}

int file_select(struct dirent *dirent_p) {
   
  if (strcmp(dirent_p->d_name,".")==0 || strcmp(dirent_p->d_name, "..")==0)
    return 0;
  else
    return 1;
}

void print_file( struct dirent *dirent_p) {
  char *name = dirent_p->d_name;

  // get file stats
  struct stat buffer;
  lstat(name, &buffer);

  // user and group information
  struct passwd *pwd = getpwuid(buffer.st_uid);
  struct group *grp = getgrgid(buffer.st_gid);
  char *gid = grp->gr_name;
  char *pwid = pwd->pw_name;

  // last modified time
  time_t lastmod = buffer.st_mtime;
  char time[80];

  // mode
  unsigned long mode = buffer.st_mode;

  // links and size
  long int links = buffer.st_nlink;
  long int size = buffer.st_size;

  // format time
  strftime(time, sizeof(time), "%Y-%m-%d %H:%M", localtime(&lastmod));

  printf("%lo %ld %s %s %ld %s %s\n", mode, links, pwid, gid, size, time, name);

}

