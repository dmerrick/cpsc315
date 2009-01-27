#include <stdio.h>
#define MAX 81

/**
 * Returns the reverse of a given string.
 * @param text is a string to reverse
 * @return the location of the reversed string
 */
char *reverse(const char *text) {
  int length = 0; // the length of the input text
  char *start;    // the start of the reversed text

  // find the length of the text
  char *tmp = text;
  while(*tmp != '\0') {
    tmp++;
    length++;
  }

  start = malloc(length * sizeof(char));

  int i;
  for(i = length-1; i >= 0; i--) {
    start[length-i-1] = text[i];
  }
  // replace what used to be the newline with a nul
  start[length] = '\0';

  return start;
}

/**
 * Returns true if the given string is a palindrome.
 * @param text is a string to check
 * @return true if text is a palindrome
 */
int is_palindrome(const char *text) {
  printf("DEBUG: text = %s", text);
  printf("DEBUG: reverse = %s", reverse(text));
  //return text == reverse(text);
  return 0;
}

int main(int argc,char *argv[]) {
  char word[MAX];
  FILE *file;

  // check that we provided an arguement
  if(argc < 2) {
    printf("usage: %s dictionary-file\n", argv[0]);
    return 1;
  }

  // open the file for reading
  file = fopen(argv[1], "r"); 

  // make sure we can successfully open the file
  if(file == NULL) {
    printf("error: can't open file %s\n", argv[1]);
    return 2;

  } else { // open was successful
    
    // loop through, line by line
    // finally stopping on the NULL pointer
    while(fgets(word, MAX, file) != NULL) { 
      // only print the word if it's a palindrome
      if (is_palindrome(word)) {
        printf("%s", reverse(word));
      }
    }

    // close the file
    fclose(file);
    // return successfully
    return 0;
  }
}

/*
Here is some example text.
wow
racecar
1234567890
france
*/
