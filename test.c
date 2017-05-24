


//other malloc test implementation

#include "othermalloc.h"
#include <string.h>

#define malloc(x) my_malloc(x, __FILE__, __LINE__)
#define free(x)	  my_free(x, __FILE__, __LINE__)


/*  Test 1:  Free memory that has been dynamically allocated.
 *  Dynamically allocate a string (character array). 
 *  	Free the string. 
 *		Free 2 bytes behind the string.  An error should occur.  
 * 		Free the string again.  An error should occur.
 *  
 */
void programtest1()
{
	printf("Start test1\n");
	
	char * stringer = (char *) malloc(sizeof(char) * 5);
	
	free(stringer);
	free(stringer-2);
	free(stringer);
	
	return;
}



/*  Test 2:  Try to free memory that was never dynamically allocated.
 *  Declare an integer.  
 *  	Try to free the integer.  An error should occur.
 */
void programtest2()
{
	printf("Start test2\n");
	
	int y = 9;
	free(&y);
	
	return;
}



/*  Test 3:  One section too full, other has room
 *  Expected Output:  Both arrays should be successfully allocated.
 */
void programtest3()
{
	printf("Start test3\n");
	
	char * Aarray = (char *) malloc(sizeof(char) * 2495);
	char * Barray = (char *) malloc(sizeof(char) * 500);
	
	if(Aarray)
		printf("Aarray was allocated\n");
		
	if(Barray)
		printf("Barray was allocated\n");
		
	free(Aarray);
	free(Barray);
}



/*  Test 4:  Redundantly freeing the same pointer
 *  Expected Output:  Error message  for freeing a pointer that was already
 *  freed.
 */
void programtest4()
{
	printf("Start test4\n");
	char * t;
	t = (char *) malloc(100);
	free(t);
	t = (char *) malloc(100);
	free(t);
	
	free(t);
}



int main()
{
	int numforCase = 0;
	
	while(1)
	{
		printf("Enter test case number: \n");
		scanf("%d", &numforCase);
	
		switch(numforCase)
		{
			case 1:
				programtest1();
				break;
			case 2: 
				programtest2();
				break;
			case 3:
				programtest3();
				break;
			case 4:
				programtest4();
				break;
			default:
				printf("Not a case number (choose between 1-4).  Please run again.\n");
				break;
		}
	break;
	}

	
	return 0;
	
}