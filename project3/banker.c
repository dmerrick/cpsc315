/**
 * Programming Project 3
 * Resource Allocation
 *
 * @author Dana Merrick
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// constants
#define TRUE 1
#define FALSE 0
#define MAX 80

// method signatures
int bankers();
void printResources();
int init( char *filename );

// these values will be read from the configuration file
int resources, processes;

// the global variables required for the Banker's Algorithm
int **Allocation;
int **Max;
int *Available;
int *Available_tmp;
int *maxres;
int *Running;

// loop variables to comply with C90 standard
int i,j; 

/**
 * Main method, reads from text file, initializes variables, and loops until finished.
 */
int main(int argc, char *argv[]) {
	
  // check that we provided an arguement
  if( argc < 2 ) {
    printf("usage: %s config_file\n", argv[0]);
    return 1;
  }

  // set up variables using config file
	init(argv[1]); 

	// initialize running to true for all processes 
  for( i=0; i<processes; i++)
    Running[i] = TRUE;
	
	// start the main loop
	int count = 0;
	do { 

		// check the number of running processes
		for( i=0; i<processes; i++ ) { 
			if( Running[i] == TRUE ) 
				count++; 
		} 

    // show the current resourse status
		printResources();
		
		// spawn threads and request resourses here.

    // temporary loop kill
		count = 0;

	} while( count != 0 );

	// out of the while loop, so all of the processes are finished
	printf("All processes finished without deadlock.\n"); 
	return 0;
}

/**
 * The Banker's Algorithm
 * This function utilizes the Banker's Algorithm to return a safe or unsafe
 * value to check if a resource is available.
 *
 * @return true if safe, false if unsafe
 */
int bankers() {
	// assume the state is unsafe
  int safe = FALSE;

  // copy the Available array into a temporary array
  for( i=0; i<resources; i++ ) {
    Available_tmp[i] = Available[i];
  }

  // look at each running process
  for( i=0; i<processes; i++ ) {
    if( Running[i] == TRUE ) {
      if( ((Max[i][0] - Allocation[i][0]) <= Available_tmp[0]) /* && rest here */) {
        for( j=0; j < resources; j++ ) {
          Available_tmp[j] += Allocation[i][j];
        }
        safe = TRUE;
      }
    }
  }
  return safe;
}

/**
 * Print current system status to the screen.
 */
void printResources() {
  printf("\n    == Current system resources ==");

  // print status of maxres
  printf("\n  === maxres ===\n");
	for( i=0; i<resources; i++ ) 
		printf("R%d\t", i+1);
	printf("\n");
	
	for( i=0; i<resources; i++ ) 
		printf("%d\t", maxres[i]); 
		
	// print status of Available
	printf("\n\n  === Available ===\n"); 
	for( i=0; i<resources; i++ ) 
		printf("R%d\t", i+1);
	printf("\n");
	
	for( i=0; i<resources; i++ ) 
		printf("%d\t", Available[i]);
	
	// print status of Max
	printf("\n\n  === Max ===\n"); 
	for( i=0; i<resources; i++ ) 
		printf("\tR%d", i+1);
	printf("\n");
	
	for( i=0; i<processes; i++) { 
		if( Running[i] == TRUE ) { 
			printf("P%d\t", i+1); 
			for( j=0; j<resources; j++) 
				printf("%d\t", Max[i][j]); 
			printf("\n"); 
		} 
	}
	
	// print status of Allocation
	printf("\n\n  === Allocation ===\n"); 
	for( i=0; i<resources; i++ ) 
		printf("\tR%d",i+1);
	printf("\n");
	
	for( i=0; i<processes; i++) { 
		if( Running[i] == TRUE ) {
			printf("P%d\t", i+1); 
			for( j=0; j<resources; j++) 
				printf("%d\t", Allocation[i][j]); 
			printf("\n"); 
		} 
	}
}

/**
 * Reads a configuration file and sets the global variables as needed.
 *
 * @param configuration file filename
 * @return status
 */
int init( char *filename ) {

  // open the config file
	FILE *file;
	if( !(file = fopen(filename,"r"))) {
		printf("error: can't open file %s\n",filename);
		return 9;
	}
	
	char buffer[MAX]; // the line read from the file
	char *tmp;        // each value on a line as a string
	
	// parse the first line: number of resource types
	fgets(buffer, MAX, file);
	resources = atoi(buffer);
	
  // now we can set up the maxres array
	if( !(maxres = malloc( resources * sizeof( int ) )))
		return 6;

	// parse second line: number of instances of each resource type
	fgets(buffer, MAX, file);
	i=0;
	// we'll use a tokenizer to split the string on spaces
	tmp = strtok(buffer," ");
	while( tmp != NULL) {
		maxres[i] = atoi(tmp);
		tmp = strtok(NULL, " ");
		i++;
	}
		  
	// parse third line: number of processes
	fgets(buffer, MAX, file);
	processes = atoi(buffer);
	
	// now that we have both processes and resources
  // we can set up the Max array (2D)
	if( !(Max = malloc( processes * sizeof( int* ) )))
		return 3;
	for( i=0; i<processes; i++ )
		if( !(Max[i] = malloc( resources * sizeof( int ) )))
		return 4;

	// parse rest of lines: max requests by each process
	i=0;
	while(fgets(buffer, MAX, file) != NULL) {
		j=0;
		// we'll use a tokenizer to split the string on spaces
		tmp = strtok(buffer," ");
		while( tmp != NULL) {
			Max[i][j] = atoi(tmp);
			tmp = strtok(NULL, " ");
			j++;
		}
		i++;
	}

  // now we can finish creating the other arrays

  // set up Allocation array (2D)
	if( !(Allocation = malloc( processes * sizeof( int* ) )))
		return 1;
	for( i=0; i<processes; i++ )
		if(!(Allocation[i] = malloc( resources * sizeof( int ) )))
		return 2;

	// set up Running array
	if( !(Running = malloc( processes * sizeof( int ) )))
		return 5;

	// set up Available array
	if( !(Available = malloc( resources * sizeof( int ) )))
		return 7;

	// set up Available_tmp array
	if( !(Available_tmp = malloc( resources * sizeof( int ) )))
		return 8;

	// close the file and exit cleanly
	fclose(file);
	return 0;
}

