//Process Queue implementation
#include "bs_types.h"
#include "process_queue.h"
#include <stdlib.h>
#include <stdio.h>

processQueueNode* makeProcessQueueNode(PCB_t* pcb){
	processQueueNode* out = malloc(sizeof(processQueueNode));
	out->content = pcb;
	out->next = NULL;
	return out;
}

processQueue* makeProcessQueue(){
	processQueue* out = malloc(sizeof(processQueue));
	out->length = 0;
	out->head = NULL;
	out->tail = NULL;

	return out;
}

void enqueue(processQueue* queue, PCB_t* pcb){
	processQueueNode* temp = makeProcessQueueNode(pcb);
	if (queue->head == NULL){
		queue->head = temp;
	}
	if (queue->tail == NULL){
		queue->tail = temp;
	}
	else{
		queue->tail->next = temp;
		queue->tail = queue->tail->next;
	}
	queue->length = (queue->length) + 1;

}

PCB_t* dequeue(processQueue* queue){
	if (isEmpty(queue)){ 
		printf("Queue is Empty\n"); 
		return NULL; 
	}

	PCB_t* out = queue->head->content;
	processQueueNode* temp = queue->head->next;
	free(queue->head);
	queue->head = temp;
	queue->length = (queue->length) - 1;
	return out;
}

Boolean isEmpty(processQueue* queue){
	if ((*queue).length == 0){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

void printQueue(processQueue* queue){
	processQueueNode* temp = queue->head;
	while (1){
		if (temp == NULL){ return; }
		printf("%u\n", temp->content->pid);
		temp = temp->next;
	}
}