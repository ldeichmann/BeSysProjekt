
#include "bs_types.h"
#include "globals.h"
#include "loader.h"
#include "core.h"


/* ----------------------------------------------------------------	*/
/* Declare global variables according to definition in globals.h	*/
PCB_t processTable[MAX_PROCESSES]; 	// the process table
unsigned systemTime=0; 			// the current system time (up time)
extern unsigned usedMemory=0;	// amount of used physical memory



int main(int argc, char *argv[])
{	// starting point, all processing is done in called functions
	printf ("Starting system. Available memory: %u\n", MEMORY_SIZE );
	initOS();		// initialise
	logGeneric("System Initialised, starting batch");
	coreLoop();		// start scheduling loop
	logGeneric("Batch complete, shutting down");
	fflush(stdout);			// make sure the output on the console is complete 
	return 1; 
}