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
	out->length = 0;
	out->head = makeMListNode(mfree, NULL, FREE_PID, 0, MEMORY_SIZE);
	return out;
}

//adds Process to list returns false if not enough space; returns true if item was added
Boolean addProcess(mList* list, PCB_t* process){
	mListNode* out = findNextFit(list, process->size);
	if (out == NULL){
		return FALSE;
	}
	else{
		if (out->length == process->size){
			unsigned outStart = out->start;
			free(out);
			//TODO
		}
	}
}

//removes Process from List
void removeProvess(mList* list, PCB_t* process){
	//TODO
}

//compacts list
void compact(mList* list){
	//TODO
}

//merges two free Nodes into one
mListNode merge(mListNode* a, mListNode* b){
	//TODO
}

//merges all adjactend free Nodes in mList
void mergeAll(mList* list){
	//TODO
}

//returns next fit, null is no fit
mListNode* findNextFit(mList* list, int len){
	mListNode* temp = list->head;
	while ((temp->type) == free && (temp->length) < len){
		temp = temp->next;
	}
	return temp;
}

