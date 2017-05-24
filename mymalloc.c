


#include "mymalloc.h"



static char     myblock[5000]; 




static Head   emptL; //empty list

static Head * pointerfree = NULL;  //start of free lift





//m_malloc-storage allocator

void * m_malloc(size_t numbytes)

{

  Head * p;

  Head * pointerprev;

  size_t   numunits;  



  numunits = (numbytes + sizeof(Head)-1) / sizeof(Head) + 1; 



  pointerprev = pointerfree;

  if( pointerprev == NULL) // none for free  list

    {

      emptL.s.pointer = pointerfree = pointerprev = &emptL;

      emptL.s.size = 0;

    }



  for(p = pointerprev->s.pointer; ; pointerprev = p, p = p->s.pointer)

    {

      if(p->s.size >= numunits) 

	{

	  if(p->s.size == numunits) //perf size

	    pointerprev->s.pointer = p->s.pointer;

	  else  //make sure the end gets allocated

	    {

	      p->s.size -= numunits;

	      p+= p->s.size;

	      p->s.size = numunits;

	    }

	  pointerfree = pointerprev;

	  return (void *) (p+1);

	}

      if(p == pointerfree) //free list

	{

	  if( (p = mocore(numunits)) == NULL)

	    return NULL;  //0

	}

    }



}







#define FALLOC 1024 //unit# request min


//mocore-asking system for more mem 

static Head * mocore(size_t nu)

{

  char * bmnnb;

  char * sbrk(int);

  Head * sup;



  if(nu < FALLOC)

    nu = FALLOC;

  bmnnb = sbrk(nu * sizeof(Head));

  if(bmnnb == (char *) -1)  //space isn't available

    return NULL;

  sup = (Head *) bmnnb;

  sup->s.size = nu;

  m_free( (void *)(sup + 1) );

  return pointerfree;



}






void m_free(void * gpp)

{

  Head * kjsys;

  Head * p;



  kjsys = (Head *)gpp - 1;  //pointer for block head 



  for(p = pointerfree; !(kjsys > p && kjsys < p->s.pointer); p = p->s.pointer)

    {

      if(p >= p->s.pointer && (kjsys > p || kjsys < p->s.pointer) )

	break;  //block freed from start or end

    }



  if(kjsys + kjsys->s.size == p->s.pointer)

    {

      kjsys->s.size += p->s.pointer->s.size;

      kjsys->s.pointer = p->s.pointer->s.pointer;

    }

  else

    kjsys->s.pointer = p->s.pointer;

  

  if(p + p->s.size == kjsys)  //joining lower 

    {

      p->s.size += kjsys->s.size;

      p->s.pointer = kjsys->s.pointer;

    }

  else

    {

      p->s.pointer = kjsys;

    }

  pointerfree = p;



}





int main()

{

  



  return 0; 

}