/**
 * Programming Project 3
 * Resource Allocation
 *
 * @author Dana Merrick
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// constants
#define TRUE 1
#define FALSE 0
#define MAX 80

// method signatures
int bankers();
void printResources();
int init( char * );
void updateNeed( int );
void *requestResource( void * );
int allocate( int, int * );
int checkCompletion( int );

// these values will be read from the configuration file
int resources, processes;

// the global variables required for the Banker's Algorithm
int *Available;
int **Allocation;
int **Max;
int **Need;
int *Finish;

// loop variables to comply with C90 standard
int i,j; 

// set up mutexes for threading
pthread_mutex_t mutexAvailable;
pthread_mutex_t mutexAllocation;
pthread_mutex_t mutexMax;
pthread_mutex_t mutexNeed;


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
		Finish[i] = FALSE;

	// start the main loop
	int count = 0;
	do { 
		
		// check the number of running processes
		for( i=0; i<processes; i++ ) { 
			if( Finish[i] == FALSE ) {
				count++;
				//requestResource((void *)i);

        
        pthread_t threads[processes];
	    	int return_code;
				printf("Creating thread for process %d.\n", i);
				return_code = pthread_create(&threads[i], NULL, requestResource, (void *)i);
				if (return_code) {
					printf("ERROR; return code from pthread_create() is %d\n", return_code);
					exit(-1);
				}
				
			}
		} 


	} while( count != 0 );

	// out of the while loop, so all of the processes are finished
	printf("\n = Success! =\n");
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
	printf("\n    == Running Banker's Algorithm ==\n");

	// assume the state is unsafe
	int safe = FALSE;

	// set up Work array
	int *Work;
	if( !(Work = malloc( resources * sizeof( int ) )))
		return -1;

	// copy the Available array into the temporary array
	for( i=0; i<resources; i++ ) {
		Work[i] = Available[i];
	}

	// Find an index i such that...
	for( i=0; i<processes; i++ ) {
		// ...Finish[i] == FALSE and...
		if( Finish[i] == FALSE ) {
			// ...Need[i] <= Work
			for( j=0; j<resources; j++ ) {
				if( Need[i][j] > Work[j] ) {
					printf("Result is: UNSAFE.");
					printResources();
					return safe;
				}
			}

			for( j=0; j < resources; j++ ) {
				Work[j] += Allocation[i][j];
				//Finish[j] = TRUE;
			}
			
			// FIXME: does this go here?
			safe = TRUE;
		}
	}

	printf("Result is: %s.\n", safe ? "SAFE" : "UNSAFE");
	printResources();
	return safe;
}

/**
 * Print current system status to the screen.
 */
void printResources() {
	printf("\n    == Current system resources ==");

	// print status of Available
	printf("\n  === Available ===\n"); 
	for( i=0; i<resources; i++ ) 
		printf("\tR%d", i);
	printf("\n");

	for( i=0; i<resources; i++ ) 
		printf("\t%d", Available[i]);

	// print status of Allocation
	printf("\n\n  === Allocation ===\n"); 
	for( i=0; i<resources; i++ ) 
		printf("\tR%d",i);
	printf("\n");

	for( i=0; i<processes; i++) { 
		if( Finish[i] == FALSE ) {
			printf("P%d\t", i); 
			for( j=0; j<resources; j++) 
				printf("%d\t", Allocation[i][j]); 
			printf("\n"); 
		} 
	}

	// print status of Max
	printf("\n  === Max ===\n"); 
	for( i=0; i<resources; i++ ) 
		printf("\tR%d", i);
	printf("\n");

	for( i=0; i<processes; i++) { 
		if( Finish[i] == FALSE ) { 
			printf("P%d\t", i); 
			for( j=0; j<resources; j++) 
				printf("%d\t", Max[i][j]); 
			printf("\n"); 
		} 
	}

	// print status of Need
	printf("\n  === Need ===\n");
	for( i=0; i<resources; i++ ) 
		printf("\tR%d", i);
	printf("\n");

	for( i=0; i<processes; i++) { 
		if( Finish[i] == FALSE ) { 
			printf("P%d\t", i); 
			for( j=0; j<resources; j++) 
				printf("%d\t", Need[i][j]); 
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

	// now we can set up the Available array
	if( !(Available = malloc( resources * sizeof( int ) )))
		return 6;

	// parse second line: number of instances of each resource type
	fgets(buffer, MAX, file);
	i=0;
	// we'll use a tokenizer to split the string on spaces
	tmp = strtok(buffer," ");
	while( tmp != NULL) {
		Available[i] = atoi(tmp);
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

	// set up Need array (2D)
	if( !(Need = malloc( processes * sizeof( int* ) )))
		return 7;
	for( i=0; i<processes; i++ )
		if(!(Need[i] = malloc( resources * sizeof( int ) )))
		  return 8;
	
	// fill in Need array for each process
	for( i=0; i<processes; i++ ) {
		//FIXME: why wont i increment past 0?
		updateNeed(i);
	}
	
	// quick hack
	updateNeed(1);
	updateNeed(2);
	
	// set up Finish array
	if( !(Finish = malloc( processes * sizeof( int ) )))
		return 5;

	// close the file and exit cleanly
	fclose(file);
	return 0;
}

/**
 * Updates calculates the current values inside the Need array for a given process.
 *
 * @param process number
 */
void updateNeed(int process) {
	//printf("Updating Need for process %d.",process);
	for( i=0; i<resources; i++ )
		Need[process][i] = Max[process][i] - Allocation[process][i];
}

/**
 * Creates a random request vector and requests it.
 *
 * @param process number
 */
void *requestResource( void *proc ) {

  // sleep for a random period
	sleep(1+(int)(10.0*(rand() / 10)));

	int Request[resources];
	int process = (int)proc;
	
	// generate a random request vector
	for(i=0; i<resources; i++) {
		Request[i] = rand() % Need[process][i];
		// this will always be less than what we need, so we dont have to check
		
		printf("Customer %d is requesting %d unit(s) from R%d.\n",process,Request[i],i);
	}
  
  /* uncomment this if we're not using checkCompletion() and allocate()
  for(i=0; i<resources; i++)
    if( Request[i] > Available[i])
      return; // this process has to wait

  for(i=0; i<resources; i++) {
	  Available[i] -= Request[i];
		Allocation[process][i] += Request[i];
		Need[process][i] -= Request[i];
	}
	*/

  // allocate the resource to the process
	if( allocate( process, Request) == TRUE ) {
		printf("Approved.\n");
		if( checkCompletion( process ) == TRUE ) {
			printf("\nProcess %d has completed!\n",process);
		  sleep(rand() % 10);
	  }
	} else {
		printf("Denied.\n");
		//wait.
		sleep(rand() % 10);
	}

  // exit the thread
	pthread_exit(NULL);

}

/**
 * Attempts to allocate a request vector to a process. 
 *
 * @param process number
 * @param request vector 
 * @return true if successfully allocated
 */
int allocate(int process, int *Request) {
	 
	for(i=0; i<resources; i++) {
		pthread_mutex_lock( &mutexAllocation );
		Allocation[process][i] += Request[i]; 
		pthread_mutex_unlock( &mutexAllocation );
		
		pthread_mutex_lock( &mutexAvailable );
		Available[i] -= Request[i];
		pthread_mutex_unlock( &mutexAvailable );
	} 
	
	if( bankers() == FALSE) {
		for(i=0; i<resources; i++) { 
			pthread_mutex_lock( &mutexAllocation );
			Allocation[process][i] -= Request[i]; 
			pthread_mutex_unlock( &mutexAllocation );
			
			pthread_mutex_lock( &mutexAvailable );
			Available[i] += Request[i]; 
			pthread_mutex_unlock( &mutexAvailable );
		} 
		return FALSE; 
	} 
	return TRUE; 
} 

/**
 * Checks if a given process has completed.
 *
 * @param process number
 * @return true if the process is complete
 */
int checkCompletion(int process) {
	for( i=0; i<resources; i++) {
		if( !(Allocation[process][i] == Max[process][i]))
			return FALSE;
			
		for(i=0; i<resources; i++ ) { 
			Available[i] += Max[process][i]; 
		}
		
		// process is complete!
		Finish[process] = TRUE; 
		return TRUE; 
	}
  return TRUE; 
}


