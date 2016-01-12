/*
Implementation von mList to manage Memory in SimOS
*/

#ifndef __M_LIST__
#define __M_LIST__

#include "bs_types.h"


typedef enum{
	free, used
}mType;

typedef struct{
	mType type;
	unsigned pid;
	int start;
	int length;
	mListNode* next;

}mListNode;

typedef struct{
	mListNode* head;
	int length;
}mList;

//function Prototypes

//makes new MListNode
mListNode* makeMListNode();

//makes new MList
mList* makeMList();

//adds Process to list returns false if not enough space; returns true if item was added
Boolean addProcess(mList* list, PCB_t* process);

//removes Process from List
void removeProvess(mList* list, PCB_t* process);

//compacts list
void compact(mList* list);

//merges two free Nodes into one
mListNode merge(mListNode* a, mListNode* b);

//merges all adjactend free Nodes in mList
void mergeAll(mList* list);



#endif /*__M_LIST__*/
