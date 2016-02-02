#include <stdlib.h>
#include "./list.h"
#include "my_malloc.h"

/**
 * pushFront
 * Takes in ptr to a list and data to add
 * Should add new node containing the data to the head of the list, and increment size
 */
void pushFront(LIST *list, int data)
{
	/*creates a new node using my_malloc. checks if valid memory is returned*/
	NODE* node = my_malloc(sizeof(NODE));
	if (node == NULL)
	{
		return;
	}
	else
	{
		node->data = data;
		node->next = NULL;
		node->prev = NULL;
	}
	/*checks if the list has anything in it. if not, make node head & tail*/
	if ((list->size == 0) && (list->head == NULL))
	{
		list->head = node;
		list->tail = node;
		list->size = 1;
	}
	else
	{
		/*adds to the front of linked list*/
		NODE* temp = list->head;
		list->head = node;
		node->next = temp;
		temp->prev = list->head;
		list->size = list->size + 1;
	}
}

/**
 * pushFront
 * Takes in ptr to a list and data to add
 * Should add new node containing the data to the tail of the list, and increment size
 */
void pushBack(LIST *list, int data)
{
	/*creates a new node using my_malloc. checks if valid memory is returned*/
	NODE* node = my_malloc(sizeof(NODE));
	if (node == NULL)
	{
		return;
	}
	else
	{
		node->data = data;
		node->next = NULL;
		node->prev = NULL;
	}
	/*checks if the list has anything in it. if not, make node head & tail*/
	if ((list->size == 0) && (list->head == NULL))
	{
		list->head = node;
		list->tail = node;
		list->size = 1;
	}
	else
	{
		/*adds to the end of linked list*/
		NODE* temp = list->tail;
		temp->next = node;
		node->prev = temp;
		list->tail = node;
		list->size = list->size + 1;
	}
}

/**
 * popFront
 * Takes in ptr to a list
 * Remove and free node at the front of the list, and decrement size
 * Return the value of that node
 * Return 0 if the size of the list is 0
 */
int popFront(LIST *list)
{
	if (list->size == 0)
	{
		return 0;
	}
	else if (list->size == 1)
	{
		/*returns the data of the head of the list if there is only a head node*/
		NODE* ret = list->head;
		int data = ret->data;
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
		my_free(ret);
		return data;
	}
	else
	{
		/*returns the data of the head of the list if there are many nodes*/
		NODE* ret = list->head;
		int data = ret->data;
		list->head = ret->next;
		(list->head)->prev = NULL;
		list->size = list->size - 1;
		my_free(ret);
		return data;
	}
}

/**
 * popBack
 * Takes in ptr to a list
 * Remove and free node at the back of the list, and decrement size
 * Return the value of that node
 * Return 0 if the size of the list is 0
 */
int popBack(LIST *list)
{
	/*returns 0 if size is 0*/
    if (list->size == 0)
	{
		return 0;
	}
	else if (list->size == 1)
	{
		/*returns the data of the head of the list if there is only a head node*/
		NODE* ret = list->head;
		int data = ret->data;
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
		my_free(ret);
		return data;
	}
	else
	{
		/*returns the data of the tail of the list if there are many nodes*/
		NODE* ret = list->tail;
		int data = ret->data;
		list->tail = ret->prev;
		(list->tail)->next = NULL;
		list->size = list->size - 1;
		my_free(ret);
		return data;
	}
}

/*prints the list*/
void printList(LIST* list)
{
	if (list->size == 0)
	{
		return;
	}
	NODE* current = list->head;
	int count = 0;
	while(current != NULL)
	{
		fprintf(stderr, "node %d: %d\n", count, current->data);
		count++;
		current = current->next;
	}

}


