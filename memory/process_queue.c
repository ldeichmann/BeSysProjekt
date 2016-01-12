//Process Queue implementation
#include "bs_types.h"
#include "process_queue.h"
#include <stdlib.h>

processQueueNode* makeProcessQueueNode(){
	processQueueNode* out = malloc(sizeof(processQueueNode));
	return out;
}

processQueue* makeProcessQueue(){
	processQueue* out = malloc(sizeof(processQueue));
	out->length = 0;
	out->head = NULL;

	return out;
}

void enqueue(processQueue* queue, PCB_t* pcb){
	//if (queue.head.content.pid == NULL){
	if (queue->head == NULL){
		queue->head = makeProcessQueueNode(pcb);
	}
	if (queue->tail == NULL){
		queue->tail = makeProcessQueueNode(pcb);
	}
	else{
		queue->tail->next = makeProcessQueueNode(pcb);
		queue->tail = queue->tail->next;
	}
	queue->length = (queue->length) + 1;

}

PCB_t* dequeue(processQueue* queue){
	if (isEmpty(queue)){return NULL;}

	PCB_t* out = queue->head->content;
	processQueueNode* temp = queue->head->next;
	free(queue->head);
	queue->head = temp;
	queue->length = (queue->length) - 1;
	return out;
}

Boolean isEmpty(processQueue* queue){
	if (queue->length == 0){
		return TRUE;
	}
	else{
		return FALSE;
	}
}