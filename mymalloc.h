

#include <stdlib.h>

typedef long aligner; //alignment to long boundary

union head // block head
{
  struct
  {
    union head * pointer;  //next free block on list
    size_t size;  //block size
  } s;
  aligner x;  //alignment for blocks
};
typedef union head Head;



void * m_malloc(size_t numbytes);
static Head * mocore(size_t nu);
void m_free(void * gpp);