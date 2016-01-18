/*
Implementation von mList to manage Memory in SimOS
*/

#include "bs_types.h"
#include "m_list.h"
#include "globals.h"
#include <stdlib.h>

//makes new MListNode
mListNode* makeMListNode(mType type, unsigned pid, int start, int length){
	mListNode* out = malloc(sizeof(mListNode));
	out->type = type;
	out->pid = pid;
	out->start = start;
	out->length = length;
	out->next = NULL;
	return out;
}

//makes new MList
mList* makeMList(){
	mList* out = malloc(sizeof(mList));
	out->head = makeMListNode(mfree, FREE_PID, 0, MEMORY_SIZE);
	out->length = 1;
	return out;
}

//adds Process to list returns false if not enough space; returns true if item was added
Boolean addProcess(mList* list, PCB_t* process){
	mListNode* out = findNextFit(list, process->size); // get element after which there is fitting space
	mListNode* tmp;
	if (out == NULL){ // there is no fitting space
		return FALSE; 
	}
	else{ // there is a fitting space
		if (out->next == NULL) { // there is only free space, we're probably the first process or everyone else is dead

			tmp = makeMListNode(used, process->pid, 0, process->size); //create new node
			tmp->next = out; // set out as our next node
			list->head = tmp; // set us to head as we are first now
			out->length = out->length - process->size;
			out->start = out->start + process->size;


		}
		else if (out->next->length == process->size){ // the space is exactly the size we need
			out->next->type = used; // so we'll just hijack the mListNode
			out->next->pid = process->pid;
		}
		else { // the space is larger than our process
			//TODO: Verify that we aren't leaving 1 "byte" between these

			out->next->length = (out->next->length - process->size); // shrink free space
			out->next->start = (out->next->start + process->size); // move start 
			tmp = makeMListNode(used, process->pid, (out->start + out->length) , process->size); //create new node
			tmp->next = out->next; // set next for the new node
			out->next = tmp; // connect our new node with out

		}
		list->length++;
	}
}

//removes Process from List
void removeProcess(mList* list, PCB_t* process){
	mListNode* tmp = list->head;
	while (tmp != NULL){
		if (tmp->pid == process->pid){
			tmp->type = mfree;
			tmp->pid = FREE_PID;
			return;
		}
		tmp = tmp->next;
	}
}

//compacts list
void compact(mList* list){
	printf("COMPACT\n");

	mListNode* out = list->head;
	mListNode* tmp;


	// the strategy here is do move all the free blocks to the end, merging free blocks as we move along
	// but we'll have to take a look at the first two elements first

	if (out->type == mfree) {
		while (out->next != NULL && out->next->type == mfree) {  // merge all the free blocks
			out = merge(out, out->next);
		}
		if (out->next != NULL) {
			list->head = swapper(out, out->next);; // changing the list head first
			out = list->head;
		}
	}
	

	while (out->next != NULL && out->next->next != NULL) { // while there are two elements after out

		while (out->next != NULL && out->next->next != NULL && out->next->type == mfree && out->next->next->type == mfree) {  // merge all the free blocks
			out = merge(out->next, out->next->next);
		}
		if (out->next != NULL && out->next->next != NULL) { // if there still are two elements after out
			if (out->next->type == mfree) { // and the first is mfree, the second isn't
				out->next = swapper(out->next, out->next->next);
			}
			else {
				out = out->next;
			}

		}

	}
}

mListNode* swapper(mListNode* a, mListNode* b) {
	mListNode* tmp;

	printf("Swapper: Switching %d and %d\n", a->pid, b->pid);

	tmp = b->next; // save the next->next element
	b->start = a->start; //swap the starts
	a->start = b->start + b->length; //calculate out->next new start
	b->next = a; // move out->next after out->next->next
	a->next = tmp; // switch out->next

	return b;
}

//DEPRECATED
//int swap(mListNode* a, mListNode* b){
//	unsigned pidSwap;
//	int startSwap;
//	int lengthSwap;
//	mType typeSwap;
//
//	//swap Type
//	typeSwap = a->type;
//	a->type = b->type;
//	b->type = typeSwap;
//
//	//swap PID
//	pidSwap = a->pid;
//	a->pid = b->pid;
//	b->pid = pidSwap;
//
//	//swap length
//	lengthSwap = a->length;
//	a->length = b->length;
//	b->length = lengthSwap;
//
//	//swap start
//	startSwap = a->start;
//	a->start = b->start;
//	b->start = startSwap + b->length;
//}

//merges b in to a
mListNode* merge(mListNode* a, mListNode* b){
	a->length = a->length + b->length;
	a->next = b->next;
	free(b); // FREE needs to be checked
	return a;
}


// WARNUNG: REDUNDANTER CODE! Brauchen wir diese Methode?
//merges all adjactend free Nodes in mList
void mergeAll(mList* list){

	mListNode* out = list->head;
	while (out->next != NULL) { // while there is an element after out
		if (out->type == mfree) { // out is free, move it down memory lane
			while (out->next->type == mfree) {  // merge all the free blocks
				out = merge(out, out->next);
			}
		}
		else {
			out = out->next;
		}
	}
}


//returns node before next fit, null is no fit
mListNode* findNextFit(mList* list, int len){
	mListNode* temp = list->head;

	if ( ( list->head->next == NULL ) && ( list->head->type == mfree ) ) { // there is only one block and it's free
		return list->head;
	}

	while (temp->next != NULL){ // go through every node and look for a next that is free and fits
		if ((temp->next->type == mfree) && (temp->next->length >= len)) {
			return temp;
		}
		temp = temp->next;

	}
	// there were no fitting blocks
	compact(list); //If no fit compact list and return NULL //to be changed for optimization
	return NULL;
}

