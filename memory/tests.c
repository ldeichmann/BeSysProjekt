#include "bs_types.h"
#include "m_list.h"
#include "globals.h"
#include <stdlib.h>

void print_mlist(mList* m) {
	mListNode* temp;

	temp = m->head;
	while (1) {
		if (temp == NULL) { break; }
		printf("<\n");
		printf("%u\n", temp->pid);
		printf("%u\n", temp->type);
		printf("%u\n", temp->start);
		printf("%u\n", temp->length);
		printf(">\n");
		temp = temp->next;
	}
}

int test_mlist(){

	// creates 4 processes, removes them and calls compact

	// TODO: real verification of output, not just eyeballing it...

	PCB_t* proc1 = malloc(sizeof(PCB_t));
	PCB_t* proc2 = malloc(sizeof(PCB_t));
	PCB_t* proc3 = malloc(sizeof(PCB_t));
	PCB_t* proc4 = malloc(sizeof(PCB_t));

	mList* list = makeMList();


	proc1->pid = 1;
	proc2->pid = 2;
	proc3->pid = 3;
	proc4->pid = 4;

	proc1->size = 40;
	proc2->size = 7;
	proc3->size = 500;
	proc4->size = 2;

	addProcess(list, proc1);
	addProcess(list, proc2);
	addProcess(list, proc3);
	addProcess(list, proc4);

	print_mlist(list);

	removeProcess(list, proc2);
	printf("TEST: removed proc2\n");

	print_mlist(list);

	printf("TEST: compacting\n");
	compact(list);

	print_mlist(list);

	removeProcess(list, proc1);
	printf("TEST: removed proc1\n");

	print_mlist(list);

	printf("TEST: compacting\n");
	compact(list);

	print_mlist(list);
	removeProcess(list, proc3);
	printf("TEST: removed proc3\n");
	print_mlist(list);
	removeProcess(list, proc4);
	printf("TEST: removed proc4\n");
	print_mlist(list);
	printf("TEST: compacting\n");
	compact(list);
	print_mlist(list);

}