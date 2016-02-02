#include <stdlib.h>
#include "my_malloc.h"
#include "list.h"

extern metadata_t* freelist[8];


int main() {
	
	/*
	 * Test code goes here
	 */
/*
	void* pte;
	pte = my_malloc(4);
	fprintf(stderr, "pt: %p\n", (void *)((char *)pte - sizeof(metadata_t)));

	//printFreeList();

	void* p;
	p = my_malloc(4);
	fprintf(stderr, "p: %p\n", p);


	//printFreeList();

	void* pr;
	pr = my_malloc(1010);
	fprintf(stderr, "pr: %p\n", pr);


	//printFreeList();

	void* no;
	no = my_malloc(50);
	fprintf(stderr, "pointer: %p\n", no);

	printFreeList();
	fprintf(stderr, "\n\n");
	my_free(pr);
	printFreeList();
	fprintf(stderr, "\n\n");
	my_free(pte);
	printFreeList();
	fprintf(stderr, "\n\n");
	my_free(p);
	printFreeList();
	fprintf(stderr, "\n\n");
	my_free(no);
	printFreeList();
	fprintf(stderr, "\n\n");
	
	LIST list;
	list.size = 0;
	list.head = NULL;
	list.tail = NULL;
	LIST* l = &list;

	pushFront(l, 4);
	printFreeList();
	fprintf(stderr, "\n\n");
	pushFront(l, 5);
	printFreeList();
	fprintf(stderr, "\n\n");
	pushBack(l, 6);
	printFreeList();
	fprintf(stderr, "\n\n");
	pushBack(l, 7);
	printFreeList();
	fprintf(stderr, "\n\n");
	pushFront(l, 8);
	printFreeList();
	fprintf(stderr, "\n\n");

	printList(l);
	fprintf(stderr, "\n\n");

	int data = popFront(l);
	fprintf(stderr, "data: %d\n", data);
	printFreeList();
	fprintf(stderr, "\n\n");

	data = popBack(l);
	fprintf(stderr, "data: %d\n", data);
	printFreeList();
	fprintf(stderr, "\n\n");

	printList(l);

	LIST* ptr = my_memmove(my_malloc(sizeof(l)), (void*)l, sizeof(sizeof(NODE) * 3 + sizeof(l)));

	printList(ptr);
	pushFront(ptr, 9);
	printList(l);
	printList(ptr);

	char* pt = my_calloc( 1,(sizeof(NODE)*3 + sizeof(l)));
	char* start = pt;
	for (int i = 0; i < (sizeof(NODE) * 3 + sizeof(l)); i++)
	{
		start = pt + i;
		fprintf(stderr, "p: %d\n", *start);
	}
	*/
	return 0;
}
