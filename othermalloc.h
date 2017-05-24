

#ifndef OTHER_MALLOC_H
#define OTHER_MALLOC_H

#include <stdio.h>
//#include <unistd.h>

#define MAXSIZE 5000
#define THRESHOLD .05
#define BIGORSMALL MAXSIZE*THRESHOLD
#define SmallerSize MAXSIZE/5*2
#define BiggerSize MAXSIZE-SmallerSize

struct MemryEntry
{
  	unsigned int   	  size;
  	unsigned char     isUnfree;
  	struct MemryEntry * previous;
  	struct MemryEntry * successor;
};
typedef struct MemryEntry MemryEntry_t;

//Prototype Functions
void * my_malloc(unsigned int size, char * filename, int numLine);
void   my_free(void * p, char * filename, int numLine);

#endif