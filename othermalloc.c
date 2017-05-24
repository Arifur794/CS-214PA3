

#include "othermalloc.h"


static MemryEntry_t * rootBig;
static MemryEntry_t * rootSmall;
static int initial = 0;

static char big_block[BiggerSize];
static char small_block[SmallerSize];
	

//implementation of malloc
void * my_malloc(unsigned int size, char * filename, int numLine)
{
	
	MemryEntry_t * p;
 	MemryEntry_t * successor; 
 	

	if(!initial)
	{
		//big root creation
		rootBig = (MemryEntry_t *) big_block;
		rootBig->previous = rootBig->successor = 0;
		rootBig->size = BiggerSize - sizeof(MemryEntry_t);
		rootBig->isUnfree = 1;

		//small root creation
		rootSmall = (MemryEntry_t *) small_block;
		rootSmall->previous = rootSmall->successor = 0;
		rootSmall->size = SmallerSize - sizeof(MemryEntry_t);
		rootSmall->isUnfree = 1;
		
		initial = 1;
    }
  	
  	if(size < BIGORSMALL) //small chunk or big chunk?
  		p = rootSmall;
  	else
  		p = rootBig;
  		
  	int triedAlready = 0;
  	
  retry:
  	do
  	{
  		//search opposite section
  		if(triedAlready)
  		{
  			if(size < BIGORSMALL)
  				p = rootBig;
  			else
  				p = rootSmall;
  		}
  	
  		if(p->size < size) //block too small?
  		{
  			p = p->successor;
  			
  			//not enoughh mem?
  			if(p == 0)
  			{
  				//swictch section
  				if(!triedAlready)
  				{
  					triedAlready = 1; //cont
  					
  					goto retry;
  				}
  				
  				// no space for the second time
  				fprintf(stderr, "ERROR: No more available memory for this size chunk in file %s at line %d.\n", filename, numLine);
  				return NULL;
  			}
  		}
  		else if(!p->isUnfree) //when already taken
  		{
  			p = p->successor;
  		}
  		else if( p->size < (size + sizeof(MemryEntry_t) ) ) 
  			//no more can fit after this
  		{
  			p->isUnfree = 0;
  			return (char *) p + sizeof(MemryEntry_t);
  		}
  		else //take mem, for head
  		{
  			successor = (MemryEntry_t *) ( (char *)p + sizeof(MemryEntry_t) + size );
  			successor->previous = p;
  			successor->successor = p->successor;
  			if(p->successor != 0)
  				p->successor->previous = successor;
  			p->successor = successor;
  			
  			successor->isUnfree = 1; // leftover is available
  			successor->size = p->size - sizeof(MemryEntry_t) - size; //size when leftover
  			
  			p->size = size;
  			p->isUnfree = 0;
  			return (char *)p + sizeof(MemryEntry_t);
  		}
  		
  	}while(p);
  	
  	return 0;
}



//  Free; Implementation of free which searches for the pointer and frees it from memory if it was allocated by the above implementation of Malloc.  At all   other conditions, it produces an error.  
 
void my_free(void * p, char * filename, int numLine)
{
	if( (p < (void *)big_block || p > ( (void *)big_block + BiggerSize) ) && ( p < (void *)small_block || p > ((void *)small_block + SmallerSize) ) )
	{
		fprintf(stderr, "Error:  Cannot free pointer that was not allocated in file %s line %d.\n", filename, numLine);
		return;
	}
	
	MemryEntry_t * pointer;
	MemryEntry_t * previous;
	MemryEntry_t * successor;
	
	int isMemryEntry = 0;
	
	
	pointer = (MemryEntry_t *)big_block;
	while(pointer)//P in big block?
	{
		if(p == (void *)pointer + sizeof(MemryEntry_t) )
		{
			isMemryEntry = 1;
			break;
		}
		
		pointer = pointer->successor;
	}
	
	//to check if p is in small spot or not
	pointer = (MemryEntry_t *)small_block;
	while(pointer && !isMemryEntry)
	{
		if(p == (void *)pointer + sizeof(MemryEntry_t) )
		{
			isMemryEntry = 1;
			break;
		}
		
		pointer = pointer->successor;
	}
	
	
	if(!isMemryEntry)//to check if the malloc created it
	{
		fprintf(stderr, "ERROR:You Cannot free a pointer that was not initialized by the malloc in the file %s at line %d.\n", filename, numLine);
		return;
	}
	
	pointer = (MemryEntry_t *)( (char *)p - sizeof(MemryEntry_t) );
	
	
	if(pointer->isUnfree)//free?
	{
		fprintf(stderr,"ERROR:You Cannot free the pointer that is already free in the file %s at line %d.\n", filename, numLine);
		return;
	}
	
	//free? combine with current
	if( (previous = pointer->previous) && previous->isUnfree)
	{
		previous->size += sizeof(MemryEntry_t) + pointer->size;
		pointer->isUnfree = 1;
		previous->successor = pointer->successor;
		if(pointer->successor)
			pointer->successor->previous = previous;
	}
	else
	{
		pointer->isUnfree = 1;
		previous = pointer;
	}
	
	//next chunk free? combine with current
	if( (successor = pointer->successor) && successor->isUnfree)
	{
		previous->size += sizeof(MemryEntry_t) + successor->size;
		previous->isUnfree = 1;
		previous->successor = successor->successor;
		if(successor->successor)
			successor->successor->previous = previous;
	}
	
	return;
	
}