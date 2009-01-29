#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 81
#define BUFFSIZE 32

/**
 * Returns the reverse of a given string.
 * @param text is a string to reverse
 * @return the location of the reversed string
 */
char *reverse(char *text) {
  int length = 0; // the length of the input text
  char *start;    // the start of the reversed text

  // find the length of the text
  char *tmp = text;
  while(*tmp != '\0') {
    tmp++;
    length++;
  }

  // length is okay here because we didn't include the nul char
  start = malloc(length * sizeof(char));

  int i, j=0;
  for(i = length-2; i >= 0; i--) {
    start[j++] = text[i];
  }
  // replace what used to be the newline with a nul
  start[j] = '\0';

  return start;
}

/**
 * Returns true if the given string is a palindrome.
 * @param text is a string to check
 * @return true if text is a palindrome
 */
int is_palindrome(char *text) {
  char *reversed = reverse(text);

  // loop through and check that the strings are equal
  while( *reversed != '\0' && *text != '\0') {
    // compare the two
    if ( *reversed != *text) {
      return 0;
    } else { // increment the two
      *reversed++;
      *text++;
    }
  }
  // everything was fine, they're equal
  return 4;
}

/**
 * Returns true if the given string is five characters long.
 * @param text is a string to check
 * @return true if text exactly five chars long
 */
int is_five_chars(char *text) {
  int length = 0;
  // loop through, but break early if we go past five
  while(*text != '\0' && length <= 6) {
    text++;
    length++;
  }
  return length == 6;
}

/**
 * Returns a char from a given file.
 * Much advice on this was taken from the ANSI C book.
 * @param a file descriptor
 * @return a char
 */
char my_getc(int fd) {

  static char buf[BUFFSIZE];
  static char *bufp = buf;
  static int n=0;

  // check if the buffer is empty
  if (n == 0) {
    // read the file to the buffer
    n = read(fd, buf, sizeof buf);
    bufp = buf;
  }

  if (--n >= 0) {
    return (unsigned char) *bufp++;
  } else {
    // there was a problem
    return -1;
  }
}

/**
 * My implementation of fgetc.
 * @param a char array to put the line into
 * @param the size of the char array
 * @param a file descriptor
 * @return status of the read
 */
int my_gets(char *text, int max, int fd) {
  // a variable to represent the latest char read
  char newest;
  while ((newest = my_getc(fd)) != '\n' && newest != -1) {
    // copy the char to the text array
    *text = newest;
    *text++;
  }

  // added this to keep with the fgets behavior
  *text = '\n';
  *text++;
  *text = '\0';

  // return the error if we found one
  if (newest == -1) { return -1; }
  // otherwise return successfully 
  return 0;
}

int main(int argc, char *argv[]) {
  char word[MAX];
  FILE *file;

  // check that we provided an arguement
  if(argc < 2) {
    printf("usage: %s dictionary-file\n", argv[0]);
    return 1;
  }

  // open the file for reading
  file = open(argv[1], O_RDONLY); 

  // make sure we can successfully open the file
  if(!file) {
    fprintf(stderr, "error: can't open file %s\n", argv[1]);
    return 2;

  } else { // open was successful
   
    // loop through, line by line
    // finally stopping on the end of the file
    while(my_gets(word, MAX, file) != -1) { 
      // only print the word if it's a palindrome
      if (is_five_chars(word) && is_palindrome(word)) {
        printf("%s\n", reverse(word));
      }
    }
    
    // close the file
    close(file);
    // return successfully
    return 0;
  }
}

/*
Here is some example text.
wow
racecar
1234
12345
123456
france
AINIA
*/
