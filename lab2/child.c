/* 
 * child.c - the child program.  This program replaces the parent's program.
 */
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Process[%d]: child in execution ...\n", getpid()); 
    sleep(1);
    printf("Process[%d]: child in execution ...\n", getpid()); 
    exit(0);	
}
