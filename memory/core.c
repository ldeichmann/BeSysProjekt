/* Implementation of core functionality of the OS					*/
/* this includes the main scheduling loop							*/
/* for comments on the functions see the associated .h-file */

/* ---------------------------------------------------------------- */
/* Include required external definitions */
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bs_types.h"
#include "globals.h"
#include "core.h"
#include "loader.h"
#include "m_list.h"



/* ----------------------------------------------------------------	*/
/* Declarations of global variables visible only in this file 		*/

PCB_t process;		// the only user process used for batch and FCFS
PCB_t* pNewProcess;	// pointer for new process read from batch
//blockedListElement_t blockedOne; // the only process that can be blocked
FILE* processFile; 
struct mList* memList; //### memList for memory management

/* ---------------------------------------------------------------- */
/*                Declarations of local helper functions            */



/* ---------------------------------------------------------------- */
/*                Externally available functions                    */
/* ---------------------------------------------------------------- */

void initOS(void)
{
	memList = makeMList(); //### initiate memList
	char filename[128] = PROCESS_FILENAME; 
	unsigned i;					// iteration variable
	systemTime=0;				// reset the system time to zero
	// open the file with process definitions
	processFile = openConfigFile (processFile, filename);	
	logGeneric("Process info file opened");
	srand( (unsigned)time( NULL ) );	// init the random number generator

	/* init the status of the OS */
	// mark all process entries invalid
	for (i=0; i<MAX_PROCESSES; i++) processTable[i].valid=FALSE;
	process.pid=0;			// reset pid
}

void coreLoop(void)
	{
	PCB_t *candidateProcess=NULL;		// next process to start, already stored in process table
	unsigned candiatePid;				// pid of new process candicate (waiting to be started)
	PCB_t* nextReady=NULL;				// pointer to process that finishes next	
	Boolean fileComplete = FALSE;		// end of pending processes in the file indicator
	unsigned runningCount=0;			// counter of currently running processes
	unsigned i,pid;						// used for accessing and counting processes 
	unsigned minRemaining, remaining;	// used for processing information on processes
	unsigned delta=0;					// time interval by which the systemTime is advanced

	do {	// loop until no more process to run 
		// select and run a process
		if (candidateProcess==NULL)	// no candidate read from file yet
			{
			logGeneric("No candidate read, reading next process from file");
			// read the next process for the file and store in process table
			candiatePid=getNextPid();					// get next valid pid
			processTable[candiatePid].pid=candiatePid;		// and store in PCB
			// now really read the process information: 
			candidateProcess = readNextProcess (processFile, &processTable[candiatePid]);
			if (candidateProcess!=NULL)
				{	// there are still jobs listed in the file   
				candidateProcess->valid = TRUE;		// flag as valid entry
				logPid(candidateProcess->pid, "Process loaded from file"); 
				}
			else	// no more processes to be started 
				{
				logGeneric("No further process listed for execution."); 
				fileComplete=TRUE; 
				}
			}
		// if there are still processes to be started, the next candidate of these is now known 
		if (candidateProcess!=NULL)	
			{	// there is a process pending
			if (candidateProcess->start<=systemTime)	// test if the process is ready to be started
				{	// the process is ready to be started
				// now search for a suitable piece of memory for the process
				/* +++ this needs to be extended for real memory management +++	*/
				// this simple if must be replaced with searching for a memory location:
				//if (usedMemory+candidateProcess->size <= MEMORY_SIZE)
				if (addProcess(memList, candidateProcess)) //### check for space in memory and add to memList if possible
					{	// enough memory available, and location in memory found: start process
					candidateProcess->status=running;	// all active processes are marked active
					runningCount++;						// and add to number of running processes
					usedMemory = usedMemory+candidateProcess->size;	// update amount of used memory
					systemTime=systemTime + LOADING_DURATION;	// account for time used by OS
					logPidMem(candidateProcess->pid, "Process started and memory allocated"); 
					candidateProcess=NULL;	// process is now a running process, not a candidate any more 
					}
				else 
					{
					logPidMem(candidateProcess->pid, "Process too large, not started"); 
					}
				}
			else 
				{
				logPidMem(candidateProcess->pid, "Process read but not yet ready"); 
				if (runningCount==0)
					{	// no other process will advance the system time
						systemTime=candidateProcess->start; 
						logGeneric("CPU was idle until now."); 
					}
				}
			}
		// In case the candidate was started, check for another candiate before running the proceses
		if ((!fileComplete) && (candidateProcess==NULL)) continue; 
		/* the use of "continue" is not optimal, but as "break" in this case tolerable for a first implementation
		// The checks for starting a process are done. Now work on the running processes 
		/* +++++++++++++++++++++ */
		// get the process that will be completed next by searching the process table
		minRemaining = UINT_MAX;		// init search value 
		pid=1;							// start search with lowest valid pid
		i=0;							// init counter for running processes 
		nextReady=NULL;					// init result
		while ((i<runningCount) && (pid<MAX_PROCESSES)) 
			{
			if ((processTable[pid].valid==TRUE) && (processTable[pid].status==running))
				{
				remaining=processTable[pid].duration-processTable[pid].usedCPU;
				if (remaining<minRemaining)
					{	// found a process finishing earlier than last match
					nextReady=&processTable[pid];		// get pointer to that process
					minRemaining=remaining;				// update minimum
					}
				i++;	// one more running process used
				}
			pid++;		// next process entry
			}
		// the next process to end is found. 
		delta=minRemaining;		// all processes advance until first to quit is finished
		// Compare with the waiting candidate (if any)
		if (candidateProcess!=NULL) 
			{
			if ((candidateProcess->start > systemTime) 
				&& (candidateProcess->start-systemTime < minRemaining))
				{	// the waiting process can start before any running one is complete
				delta=(candidateProcess->start-systemTime)/runningCount;	// processes share available systemTime 
				nextReady=NULL;		// no process to terminate now
				}
			}
		//Now update OS-stats and terminate that process (if needed)
		pid=1;							// start search with lowest valid pid and 
		i=0;							// init counter for running processes 
		while ((i<runningCount) && (pid<MAX_PROCESSES)) 
			{
			if ((processTable[pid].valid==TRUE) && (processTable[pid].status==running))
				{
				// update time already spent in process
				processTable[pid].usedCPU=processTable[pid].usedCPU+delta;
				systemTime=systemTime+delta;		// each process consumes CPU time
				i++;	// one more running process updated
				}
			pid++;		// next process entry
			}
		// all running processes are updated, now quit the completed process
		/* +++ this needs to be extended for real memory management +++	*/
		if (nextReady!=NULL)	// check of a process needs to be terminated
			{					// loop may be running even if no processes are active
			removeProcess(memList, nextReady);
			usedMemory=usedMemory-nextReady->size;	// mark memory of the process free
			logPidMem(nextReady->pid, "Process terminated, memory freed"); 
			deleteProcess (nextReady);	// terminate process
			runningCount--;				// one running process less 
			}
		} 
	// loop until no running processes exist any more and no process is waiting t be started
	while ((runningCount>0) || (fileComplete==FALSE));		
	}


		
/* ----------------------------------------------------------------- */
unsigned getNextPid()
	{
	static unsigned pidCounter=1; 
	unsigned i=0;		// iteration variable;
	// determine next available pid make sure not to search infinitely
	while ((processTable[pidCounter].valid) && (i<MAX_PID))
		{
			// determine next available pid 
			pidCounter=(pidCounter+1) % MAX_PID;
			if (pidCounter==0) pidCounter++;	// pid=0 is invalid
			i++;		// count the checked entries
		}
	if (i==MAX_PID) return 0;			// indicate error
	else		   return pidCounter;
	}


	
/* ----------------------------------------------------------------- */
/*                       Local helper functions                      */
/* ----------------------------------------------------------------- */


