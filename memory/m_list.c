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
//DEPRECATED
/*void removeProcess(mList* list, PCB_t* process){
	printf("removeProcess\n");
	mListNode* out = list->head;
	mListNode* tmp;
	// is head the process we are looking for?
	if (out->pid == process->pid) {
		list->head = out->next;
		free(out);
	}
	else {
		while (out->next != NULL); { // as long as there is a next element, check it
			if (out->next->pid == process->pid) { // next element is process
				if (out->next->next == NULL) { // next element has no element afterwards
					free(out->next); // free the element without
					out->next = NULL; // set next to null
				}
				else { // there is a next->next element
					tmp = out->next->next; // save the next->next element
					free(out->next); // free the element without
					out->next = tmp; // set next to the new element
				}
			}
		} 
	}
	list->length--; // we have to assume process was actually in the list, if it wasn't, we've made a wrong choice before
	printf("removeProcess done\n");
}*/


//compacts list
void compact(mList* list){
	printf("COMPACT");

	mListNode* out = list->head;
	mListNode* tmp;

	// the strategy here is do move all the free blocks to the end, merging free blocks as we move along

	while (out->next != NULL) { // while there is an element after out
		if (out->type == mfree) { // out is free, move it down memory lane
			while (out->next->type == mfree) {  // merge all the free blocks
				out = merge(out, out->next);
			}
			/*// now swap out with out->next
			out->next->start = out->start;
			out->start = out->next->start + out->next->length;
			tmp = out->next->next;
			out->next->next = out;
			out->next = tmp;*/
			swap(out, out->next);

		} else {
			out = out->next;
		}
	}
}

int swap(mListNode* a, mListNode* b){
	unsigned pidSwap;
	int startSwap;
	int lengthSwap;
	mType typeSwap;

	//swap Type
	typeSwap = a->type;
	a->type = b->type;
	b->type = typeSwap;

	//swap PID
	pidSwap = a->pid;
	a->pid = b->pid;
	b->pid = pidSwap;

	//swap length
	lengthSwap = a->length;
	a->length = b->length;
	b->length = lengthSwap;

	//swap start
	startSwap = a->start;
	a->start = b->start;
	b->start = startSwap + b->length;
}

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

