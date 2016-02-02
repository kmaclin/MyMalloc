#include "my_malloc.h"
#include <math.h>

/* You *MUST* use this macro when calling my_sbrk to allocate the 
 * appropriate size. Failure to do so may result in an incorrect
 * grading!
 */
#define SBRK_SIZE 2048

/* If you want to use debugging printouts, it is HIGHLY recommended
 * to use this macro or something similar. If you produce output from
 * your code then you will receive a 20 point deduction. You have been
 * warned.
 */
#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif


/* make sure this always points to the beginning of your current
 * heap space! if it does not, then the grader will not be able
 * to run correctly and you will receive 0 credit. remember that
 * only the _first_ call to my_malloc() returns the beginning of
 * the heap. sequential calls will return a pointer to the newly
 * added space!
 * Technically this should be declared static because we do not
 * want any program outside of this file to be able to access it
 * however, DO NOT CHANGE the way this variable is declared or
 * it will break the autograder.
 */
void* heap;

/* our freelist structure - this is where the current freelist of
 * blocks will be maintained. failure to maintain the list inside
 * of this structure will result in no credit, as the grader will
 * expect it to be maintained here.
 * Technically this should be declared static for the same reasons
 * as above, but DO NOT CHANGE the way this structure is declared
 * or it will break the autograder.
 */
metadata_t* freelist[8];
/**** SIZES FOR THE FREE LIST ****
 * freelist[0] -> 16
 * freelist[1] -> 32
 * freelist[2] -> 64
 * freelist[3] -> 128
 * freelist[4] -> 256
 * freelist[5] -> 512
 * freelist[6] -> 1024
 * freelist[7] -> 2048
 */

/*this function calculates the log base 2 of a size*/
int logbase2(size_t size)
{
	int s = size;
	int a = 1;

	for (int i = 0; i < 12; i++)
	{
		if (a == s)
		{
			return i;
		}
		a = a * 2;
	}

	return -1;
}

/*this function calculates the freelist array index of a size*/
int arrayIndex(size_t num)
{
	int size = 8;
	int index = -1;
	for (int i = 0; i < 8; i++)
	{
		size = size * 2;
		index++; 
		if (size == num)
		{
			break;
		}
	}
	return index;
}

/*this function adds a metadata node to the freelist*/
void addToList(metadata_t* node, int index)
{
	metadata_t* current = freelist[index];
	if (current == NULL)
	{
		freelist[index] = node;
	}
	else
	{
		/*puts node at the end of the list*/
		while(current->next != NULL)
		{
			current = current->next;
		}
		current->next = node;
		node->next = NULL;
		node->prev = current;
	}
}

/*this recursively breaks apart blocks (making them smaller)
  and it returns the block requested when my_malloc is called
*/
metadata_t* getMem(int num, int index)
{
	int val = index;
	metadata_t* ans;

	/*checks if freelist has a chunk of memory at the index of the size requested in my_malloc*/
	if (freelist[val] == NULL)
	{
		/*this while loop checks if there are any memory blocks in
		  the freelist at a specific index. if not, it continues to check
		  farther into freelist, higher in memory block sizes
		  until it finds something that is not null
		*/
		while (val < 7)
		{
			val++;
			/*if there is something in the freelist
			  it breaks it in half and adds two blocks to a lower sized index
			  then it calls get mem with the same data input, to do it again
			*/
			if (freelist[val] != NULL)
			{
				metadata_t* curr = freelist[val];
				metadata_t* addr2 = (metadata_t *)((char *)curr + (curr->size/2));
				curr->size = (curr->size)/2;
				curr->in_use = 0;
				addr2->size = curr->size;
				addr2->in_use = 0;

				if (curr->next != NULL)
				{
					freelist[val] = curr->next;
					(curr->next)->prev = NULL;
				}
				else
				{
					freelist[val] = NULL;
				}

				curr->next = addr2;
				curr->prev = NULL;
				addr2->prev = curr;
				addr2->next = NULL;

				addToList(curr, val - 1);
				return getMem(num, index);
			}
		}
		/*if no memory is found in the list anywhere above the initial index
		  allocate more memory by callin my_sbrk again
		*/
		metadata_t *nextbrk = my_sbrk(SBRK_SIZE);
		if (nextbrk == ((void *) -1))
		{
			ERRNO = OUT_OF_MEMORY;
			return NULL;
		}
		nextbrk->in_use = 0;
		nextbrk->size = SBRK_SIZE;
		nextbrk->next = NULL;
		nextbrk->prev = NULL;
		addToList(nextbrk, arrayIndex(nextbrk->size));
		return getMem(num, index);
	}
	else
	{
		/*if memory blocks have been found at that index,
		  return that block (take it out of the linked list there)
		*/
		metadata_t* current = freelist[val];
		current->in_use = 1;
		if (current->next != NULL)
		{
			freelist[val] = current->next;
			(current->next)->prev = NULL;
		}
		else
		{
			freelist[val] = NULL;
		}
		current->next = NULL;
		current->prev = NULL;
		ans = current;
	}
	return ans;
}

/*gets the actual size the the user will get back after
  my_malloc is called.
*/
int getMemBlockSize(int size)
{
	int num = 8;

	for (int i = 0; i < 8; i++)
	{
		num = num * 2;
		if ((num - size) > 0)
		{
			break;
		}
	}
	return num;
}


void* my_malloc(size_t size)
{
	/*count is static so that we can know when my_sbrk is called first*/
	static int count;
	ERRNO = NO_ERROR;
	void* start = NULL;
	metadata_t* value = NULL;
	/*adds the metadata_t size to the size to get the true size of the
	  the memory block to be returned
	*/
	int trueSize = size + sizeof(metadata_t);

	/*set error code if trueSize is larger that largest size that can be requested*/
	if (trueSize > SBRK_SIZE)
	{
		ERRNO = SINGLE_REQUEST_TOO_LARGE;
		return NULL;
	}

	int num = getMemBlockSize(trueSize);
	int index = arrayIndex(num);

	/*check if my_sbrk has been called previously*/
  	if (!count)
  	{
  		/*heap is equal to the first call to my_sbrk*/
  		heap = my_sbrk(SBRK_SIZE);
  		if (heap != ((void *) -1))
  		{
  			/*puts heap memory in freelist*/
	  		metadata_t *first = heap;
	  		first->in_use = 0;
	  		first->prev = NULL;
	  		first->next = NULL;
	  		first->size = SBRK_SIZE;
	  		freelist[7] = first;

	  		/*sets count (static) to 1 since sbrk has been called for the first time*/
  			count = 1;
  			/*if the memory needed is the size of the amount returned
  			  by my_sbrk, then return that memory, which is pointed to
  			  by the freelist, + the size of the metadata
  			*/
  			if (num == SBRK_SIZE)
  			{
  				return (freelist[7] + sizeof(metadata_t));
  			}
  			else
  			{
  				/*if you need a smaller memory chunk, call get mem,
  				  a recursive function that returns a memory block
  				*/
  				value = getMem(num, index);
  				/*if value is NULL, return it*/
  				if (value == NULL)
  				{
  					return value;
  				}
  				/*if not NULL add size of metadata and set start equal to that*/
  				start = (char*)value + sizeof(metadata_t);
  			}
  		}
  		else
  		{
  			/*if there is no memory on first call, highly unlikely,
  			  then set the error code and return NULL
  			*/
  			ERRNO = OUT_OF_MEMORY;
  			return NULL;
  		}
  	}
  	else
  	{
  		/*if my_sbrk has been called the first time*/
  		/*call getMem immediately to get memory*/
  		value = getMem(num, index);

  		if (value == NULL)
		{
			return value;
		}
		
  		start = (char*)value + sizeof(metadata_t);
  	}
  /*return memory address, start, to the user*/
  return start;
}

void* my_calloc(size_t num, size_t size)
{
	/*calls my_malloc to get block of memory num * size large*/
	int s = num * size;
	char* ptr = my_malloc(num * size);
	char* start = ptr;
	
	/*if my_malloc returns NULL, then the memory reqested was too large*/
	/*so we return NULL*/
	if (ptr == NULL)
	{
		ERRNO = SINGLE_REQUEST_TOO_LARGE;
		return NULL;
	}

	/*loop through each byte of memory and set it to zero*/

	for (int i = 0; i < s; i++)
	{
		start = ptr + i;
		*start = 0;
	}

	ERRNO = NO_ERROR;

	/*return pointer*/
	return (void *)ptr;
}

/*returns the address of the buddy*/
metadata_t* getAddress(metadata_t* add)
{
	/*address2 = ((address1 - heap) XOR (1 << log2(address1->size))) + heap;*/
	int add2 = (((int)add - (int)heap) ^ (1 << (logbase2(add->size)))) + (int)heap;
	metadata_t* a2 = (metadata_t *)add2;

	/*if the sizes are not the same, return null.*/
	if (a2->size == add->size)
	{
		return a2;
	}
	else
	{
		return NULL;
	}
}

/*remove block of memory from freelist by manipulating linked list stuff*/
void removeBlock(metadata_t* block)
{
	/*checks on four cases, next and prev are both NULL, are both not NULL,
	  prev is NULL and next isn't, or next is NULL and prev isn't
	  removes block accordingly
	*/
	if ((block->next != NULL) && (block->prev != NULL))
	{
		(block->next)->prev = block->prev;
		(block->prev)->next = block->next;
	}
	else if ((block->prev != NULL) && (block->next == NULL))
	{
		(block->prev)->next = NULL;
	}
	else if ((block->prev == NULL) && (block->next != NULL))
	{
		freelist[arrayIndex(block->size)] = block->next;
		(block->next)->prev = NULL;
	}
	else
	{
		freelist[arrayIndex(block->size)] = NULL;
	}

	/*sets blocks pointers to NULL since it has been removed*/
	block->next = NULL;
	block->prev = NULL;
}

/*
 *joins blocks by removing them from freelist, finding the first block,
 *and by changing the size of the new block formed
 *returns the address of the new block
 */
metadata_t* joinBlocks(metadata_t* buddy, metadata_t* block)
{
	/*remove buddy and block from list*/	
	removeBlock(buddy);
	removeBlock(block);

	/*finds the first block by comparing memory addresses*/
	metadata_t* first;

	if (buddy > block)
	{
		first = block;
	}
	else
	{
		first = buddy;
	}


	/*join blocks*/
	first->size = first->size * 2;

	return (metadata_t*) first;
}

/*helper function to print the freelist for testing*/
void printFreeList(void)
{
	for(int i = 0; i < 8; i++)
	{
		metadata_t* current = freelist[i];
		if (current != NULL)
		{
			while (current != NULL)
			{
				fprintf(stderr, "size: %d\n", current->size);
				//fprintf(stderr, "memAdd: %p\n", (void *)current);
				current = current->next;
			}
		}
	}
}

void my_free(void* ptr)
{
	/*subtracts size of metadata_t to get a metadata_t pointer*/
	metadata_t* pt = (metadata_t*)((char *) ptr - sizeof(metadata_t));
	/*gets address of buddy of argument*/
	metadata_t* add2 = getAddress(pt);
	/*sets arguments in_use to zero*/
	pt->in_use = 0;
	
	/*recursively recombines block sizes after checking for certain conditions*/
	if ((add2 != NULL) && (add2->size != SBRK_SIZE) && (!add2->in_use))
	{	
		metadata_t* block1 = joinBlocks(add2, pt);
		addToList(block1, arrayIndex(block1->size));
		/*
		  since the pointer, after calling my_free, is immediately turned
		  to a metadata pointer, we add back the size of metadata_t and call
		  my_free on the new block formed after combining the other blocks
		*/
		my_free((char *) block1 + sizeof(metadata_t));
	}
	else
	{

		/*if there is no buddy, if the size is equal to the max size,
		  if the buddy block is in_use (any combination of these things)
		  we then just add the memory into freelist
		*/
		ERRNO = NO_ERROR;
		addToList(pt, arrayIndex(pt->size));
	}
}

void* my_memmove(void* dest, const void* src, size_t num_bytes)
{
	/*
	  Four cases with memmove, the src and dest memory blocks do not overlap at all,
	  the src and dest completely/exactly overlap, the dest and src overlap but the dest
	  comes before the src, the dest and src overlap but the src comes first. Only one is
	  a special case in copying, and that is the last case. The others can use the same method.
	*/
	char* destination = (char *) dest;
	char* source = (char *) src;
	int i;
	
	/*if they are equal, just return the destination*/
	if (dest == src)
	{
		ERRNO = NO_ERROR;
		return (void *) destination;
	}	
	else if (dest > src)
	{
		/*if the dest is greater than src, copy from end to beginning*/
		for (i = num_bytes - 1; i >= 0; i--)
		{
			destination[i] = source[i];
		}
	}
	else
	{
		/*any other case, copy from beginning to end*/
		for (i = 0; i < num_bytes; i++)
		{
			destination[i] = source[i];
		}
	}
	ERRNO = NO_ERROR;
	return dest;
}
