/*
PROCESS_QUEUE struct incl. Nodes for managing Process that dont fit into Memory into Queue
*/

#ifndef __PROCESS_QUEUE__
#define __PROCESS_QUEUE__

#include "bs_types.h"

//QueueNode
typedef struct{
	PCB_t* content;
	struct processQueueNode* next;
}processQueueNode;

//Process Queue
typedef struct{
	processQueueNode* head;
	processQueueNode* tail;
	int length;
}processQueue;

//Function Prototypes

//creates new processQueueNode
processQueueNode* makeProcessQueueNode(PCB_t* pcb);

//Creates new ProcessQueue
processQueue* makeProcessQueue();

//Adds new Item to the Queue
void enqueue(processQueue* queue, PCB_t* process);

//removes and returns Item from Queue
PCB_t* dequeue(processQueue* queue);

//returns true if queue is Empty
Boolean isEmpty(processQueue* queue); 


#endif /*__PROCESS_QUEUE__*/
