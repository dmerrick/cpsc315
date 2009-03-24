/**
 * Programming Project 3
 * Resource Allocation
 *
 * @author Dana Merrick
 */

#include <stdio.h>
#include <stdlib.h>

// constants
#define TRUE 1
#define FALSE 0

// method signatures
int bankers();
void printResources();
int init( char *filename );
void requestResource( int process );
int allocate( int proc, int res[3] );
int checkCompletion( int proc );

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
		for(i=0; i<processes; i++) { 
			if( Running[i] == TRUE ) 
				count++; 
		} 

    // show the current resourse status
		printResources();
		
		// spawn threads and request resourses here.

	} while( count != 0 );

	// out of the while loop, so all of the processes are finished
	printf("All processes finished without deadlock."); 
	
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
 * Reads a configuration file and sets the variables as needed.
 *
 * @param configuration filename
 */
int init( char *filename ) {
	
	// temporarally hardwiring in these values:
	resources = 3;
	processes = 3;

  // set up Allocation 2D array
	if( !(Allocation = malloc( processes * sizeof( int* ) )))
		return 1;
	for( i=0; i<processes; i++ )
		if(!(Allocation[i] = malloc( resources * sizeof( int ) )))
		return 2;

	// set up Max 2D array
	if( !(Max = malloc( processes * sizeof( int* ) )))
		return 3;
	for( i=0; i<processes; i++ )
		if( !(Max[i] = malloc( resources * sizeof( int ) )))
		return 4;

	// set up Running array
	if( !(Running = malloc( processes * sizeof( int ) )))
		return 5;

	// set up maxres array
	if( !(maxres = malloc( resources * sizeof( int ) )))
		return 6;

	// set up Available array
	if( !(Available = malloc( resources * sizeof( int ) )))
		return 7;

	// set up Available_tmp array
	if( !(Available_tmp = malloc( resources * sizeof( int ) )))
		return 8;

  // hardwired values
	maxres[0]=6; maxres[1]=4; maxres[2]=7;
	Available[0]=6; Available[1]=4; Available[2]=7;
	Available_tmp[0]=6; Available_tmp[1]=4; Available_tmp[2]=7;
	
	Max[0][0]=3; Allocation[0][0]=1; Available[0]=3; 
	Max[0][1]=3; Allocation[0][1]=2; Available[1]=1; 
	Max[0][2]=2; Allocation[0][2]=2; Available[2]=1; 
	Max[1][0]=1; Allocation[1][0]=1; 
	Max[1][1]=2; Allocation[1][1]=0; 
	Max[1][2]=3; Allocation[1][2]=3; 
	Max[2][0]=1; Allocation[2][0]=1; 
	Max[2][1]=1; Allocation[2][1]=1; 
	Max[2][2]=5; Allocation[2][2]=1; 
	
	return 0;
}

/**
 * this routine mimics an OS resource request which basiclly checks if a resource is busy, if not gives it to your process, and then marks it busy. If it is busy to begin with some strategy is used to deny the request. Here, he deadlocks if the request cannot be done -- I think that means that the processes cannot complete because of lack of resources, no matter what you do (Besides runnign them one at a time). 
 *
 * @param process number
 */
void requestResource( int process ) {
//check if it is allocated, will it go to deadlock 
	int res[3]; 

	//process--; 
	
	if( Running[process] == TRUE ) 
	{ 
		printf("\nCurrently this process needs the following resources:\n"); 
		for( i=0; i<resources; i++ ) 
			printf("R%d\t",i+1);
		printf("\n");
		for( i=0; i<resources; i++) 
			printf("%d\t",Max[process][i]-Allocation[process][i]); 
		for(i=0; i<resources; i++) 
		{ 
			loop_3: 
			printf("\nEnter number of resource %d to allocate to process %d:\n",i+1,process+1); 
			scanf("%d",&res[i]); 
			if((res[i]>(Max[process][i]-Allocation[process][i]))||(res[i]>Available[i])) 
			{ 
				printf("\nInvalid value!, try again!"); 
				goto loop_3; 
			} 
		} 
		
		getchar(); 
		
		// alocate the resource to the process
		if( allocate(process,res) == TRUE ) { 
			printf("\nResources successfully allocated.\n");
			
			if( checkCompletion(process) == TRUE ) 
				printf("\nProcess %d has completed its execution and its resources have been released.\n",process+1); 
		} else {
			printf("\nResources cannot be allocated as it may lead to deadlock.\n");
		}
		
	} 
	else 
	{ 
		printf("\nInvalid process number.\n"); 
		getchar(); 
	} 
} 

// Allocate a resource to a process, used in the above routine.
// This is just management code to mark the appropriate stuff when an allocation is allowed. 
int allocate(int proc, int res[3]) {
	 
	for(i=0; i<resources; i++) { 
		Allocation[proc][i] += res[i]; 
		Available[i] -= res[i]; 
	} 
	
	if( bankers() == FALSE) {
		for(i=0; i<resources; i++) { 
			Allocation[proc][i] -= res[i]; 
			Available[i] += res[i]; 
		} 
		return FALSE; 
	} 
	return TRUE; 
} 

int checkCompletion(int proc) { 
	if((Allocation[proc][0]==Max[proc][0])&&(Allocation[proc][1]==Max[proc][1])&&(Allocation[proc][2]==Max[proc][2])) { 
		for(i=0; i<resources; i++ ) { 
			Available[i] += Max[proc][i]; 
		}
		// process is complete!
		Running[proc] = FALSE; 
		return TRUE; 
	}
	// not quite finished, so return false
	return FALSE; 
}
