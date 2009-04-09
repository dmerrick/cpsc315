#include <stdio.h>
#define SIZE 100

// define the struct we will be using
typedef struct { 
  char* s[SIZE];
  int top;
} stack;

/**
 * Initializes the stack to be empty.
 * @param stack
 */
void init(stack *stk) {
  stk->top=0;
}

/**
 * Returns true if stack is full.
 * @param stack
 * @return true if full
 */
int full(stack *stk) {
  return (stk->top >= SIZE);
}

/**
 * Returns true if stack is empty.
 * @param stack
 * @return true if empty
 */
int empty(stack *stk) {
  return (stk->top == 0);
}

/**
 * Pushes a string into the stack.
 * @param stack
 * @param string
 */
void push(stack *stk, char* string) {
  stk->s[ stk->top ] = string; 
  (stk->top)++;    
}

/**
 * Pops a string off of the stack.
 * @param stack
 * @return string pointer
 */
char* pop(stack *stk) {
  (stk->top)--;
  return (stk->s[stk->top]);
}

