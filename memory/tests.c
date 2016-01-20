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
		printf("PID: %u\n", temp->pid);
		printf("TYP: %u\n", temp->type);
		printf("STA: %u\n", temp->start);
		printf("LEN: %u\n", temp->length);
		printf(">\n");
		temp = temp->next;
	}
}

int compare_mListNode(mListNode* node, int pid, mType type, int start, int length) {

	if (node->pid == pid && node->type == type && node->start == start && node->length == length) {
		return 1;
	}
	else {
		return 0;
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

	int test = 0;


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

	if (compare_mListNode(list->head, 1, used, 0, 40) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;
	if (compare_mListNode(list->head->next, 2, used, 40, 7) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;
	if (compare_mListNode(list->head->next->next, 3, used, 47, 500) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;
	if (compare_mListNode(list->head->next->next->next, 4, used, 547, 2) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;
	if (compare_mListNode(list->head->next->next->next->next, FREE_PID, mfree, 549, 475) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;

	print_mlist(list);

	removeProcess(list, proc2);
	printf("TEST: removed proc2\n");

	print_mlist(list);

	printf("TEST: compacting\n");
	compact(list);

	if (compare_mListNode(list->head, 1, used, 0, 40) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;
	if (compare_mListNode(list->head->next, 3, used, 40, 500) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;
	if (compare_mListNode(list->head->next->next, 4, used, 540, 2) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;
	if (compare_mListNode(list->head->next->next->next, FREE_PID, mfree, 542, 482) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;


	print_mlist(list);

	removeProcess(list, proc1);
	printf("TEST: removed proc1\n");

	print_mlist(list);

	printf("TEST: compacting\n");
	compact(list);

	if (compare_mListNode(list->head, 3, used, 0, 500) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;
	if (compare_mListNode(list->head->next, 4, used, 500, 2) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;
	if (compare_mListNode(list->head->next->next, FREE_PID, mfree, 502, 522) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;

	print_mlist(list);
	removeProcess(list, proc3);
	printf("TEST: removed proc3\n");
	print_mlist(list);


	if (compare_mListNode(list->head, FREE_PID, mfree, 0, 500) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;
	if (compare_mListNode(list->head->next, 4, used, 500, 2) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;
	if (compare_mListNode(list->head->next->next, FREE_PID, mfree, 502, 522) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;

	removeProcess(list, proc4);
	printf("TEST: removed proc4\n");
	print_mlist(list);
	printf("TEST: compacting\n");
	compact(list);
	print_mlist(list);

	if (compare_mListNode(list->head, FREE_PID, mfree, 0, 1024) == 0) {
		printf("Test %d failed\n", test);
	}
	else {
		printf("Test %d sucessful\n", test);
	}
	test++;

}