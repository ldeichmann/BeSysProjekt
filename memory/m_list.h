/*
Implementation von mList to manage Memory in SimOS
*/

#ifndef __M_LIST__
#define __M_LIST__

#include "bs_types.h"
#include "globals.h"
#define FREE_PID MAX_PID+1


typedef enum{
	mfree, used
}mType;

typedef struct mListNode{
	mType type;
	unsigned pid;
	int start;
	int length;
	struct mListNode* next;
}mListNode;

typedef struct mList{
	mListNode* head;
	int length;
}mList;

//function Prototypes

//makes new MListNode
mListNode* makeMListNode(mType type, unsigned pid, int start, int length);

//makes new MList
mList* makeMList();

//adds Process to list returns false if not enough space; returns true if item was added
Boolean addProcess(mList* list, PCB_t* process);

//removes Process from List
void removeProcess(mList* list, PCB_t* process);

//compacts list
void compact(mList* list);

//swapes nodes
mListNode* swapper(mListNode* a, mListNode*b);

//merges two free Nodes into one
mListNode* merge(mListNode* a, mListNode* b);

//merges every mfree node after root with root if root is mfree
mListNode* mergeNext(mListNode* root);

//merges all adjactend free Nodes in mList
void mergeAll(mList* list);

mListNode* findNextFit(mList* list, int length);

#endif /*__M_LIST__*/
