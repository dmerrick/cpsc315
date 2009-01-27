#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 10

int main(int argc,char *argv[]) {
  char *source, *dest;
  char buf[100];

  int file1, file2;

  // check that we provided an arguement
  if(argc < 3) {
    printf("usage: %s source destination\n", argv[0]);
    return 1;
  }

  // set readable names
  source = argv[1];
  dest = argv[2];

  // open the files
  file1 = open(source, O_RDONLY);
  file2 = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 00644);

  // check for errors
  if (!file1) {
    fprintf(stderr, "ERROR: %s could not be opened.", source);
  }
  if (!file2) {
    fprintf(stderr, "ERROR: %s could not be opened.", dest);
  }

  // loop through and copy
  int n;
  while ((n = read(file1, buf, BUFSIZE)) > 0) {
    // write to the file
    if (write(file2,buf,n) < n) {
      fprintf(stderr, "ERROR: error writing to file");
      return 2;
    }
  }

  // if we didn't end on zero, there was a problem
  if (n < 0) {
    fprintf(stderr, "ERROR: error reading file");
    return 3;
  }

  // close the files
  close(file1);
  close(file2);
  return 0;
}
