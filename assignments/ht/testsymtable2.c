#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include "symtable.h"

#define _BSD_SOURCE
#define _XOPEN_SOURCE 500
#define _ISOC99_SOURCE

/*****************************testsymtable.c*************************
 * Author: Muhammad Asim Jamshed
 * Email: asim.jamshed@gmail.com
 * Assignment#: 3
 * Additional Comments: 
 * This program tests the robustness of SymTable. Please ensure that
 * you have a correctly functioning SymTable library before generating
 * the results. Test both versions of SymTable before submitting your
 * assignment.
 ********************************************************************/

#define KEYLENGTH 10
#define SELECT_FACTOR 10

/* simple (key, value) item */
typedef struct {
  int val1;
  int val2;
} ValueStruct;

/*
 * increment the value by *newValue
 */
void update(const char* key, void* value, void* newval) 
{
  ValueStruct* vs = (ValueStruct*)value;
  vs->val2 += *((int*)newval);
}

/* 
   get the time difference between start and end
   return value is elapsed time in miliseconds
*/
float
GetTimeDiffMSec(struct timeval* start, struct timeval* end)
{
  return ((end->tv_sec - start->tv_sec) * 1e3+ 
	  (end->tv_usec - start->tv_usec) * 1e-3);
}

/*
 * Program Description: The program creates a SymTable sym. (key,
 * value) pairs are deterministically inserted (SymTable_put()) in
 * sym. Random lookups are done over sym which tests
 * SymTable_get() functions.
 *
 * 1/10th of the generated (key, value) pairs are randomly
 * removed (SymTable_remove()) and finally SymTable_map() is
 * being tested
 *
 */
int main(int argc, char** argv) 
{
  int i, j;
  SymTable_T sym;
  unsigned int bindings;
  struct timeval tm_first, tm_start, tm_end;
  char key[KEYLENGTH];
  ValueStruct* vs;
  
  /* check the command line parameter */
  if (argc != 2) {
    fprintf(stderr, "Invalid i of arguments\n"
	     "Correct Usage: %s <number_of_bindings>\n", argv[0]);
    return EXIT_FAILURE;
  }

  bindings = strtoul(argv[1], NULL, 0);
  if (bindings == ULONG_MAX || bindings == 0) {
    fprintf(stderr, "Error in interpreting the bindings parameter: %s\n", 
	     strerror(errno));
    return EXIT_FAILURE;
  }

  /* create the ValueStruct vs array */
  vs = (ValueStruct*) calloc( sizeof(ValueStruct), bindings );
  if ( vs == NULL ) {
    fprintf( stderr, "ERROR: Can't allocate memory for ValueStruct array vs\n" );
    return EXIT_FAILURE;
  }

  /* create the table */
  if ((sym = SymTable_new()) == NULL) {
    fprintf(stderr, "Can't create symtable.\n");
    return EXIT_FAILURE;
  }

  /* insert the (key, value) for 'bindings' times */
  gettimeofday(&tm_start, NULL);
  tm_first = tm_start;
  for (i = bindings-1; i >= 0; i--) {
    snprintf(key, sizeof(key), "%ul", i);
    /* setting value*/
    vs[i].val1 = i;
    vs[i].val2 = i^bindings;
    if (SymTable_put(sym, key, &vs[i]) == 0) {
      fprintf(stderr, "WARNING: Couldn't put (%d, %d) for key %s in symb!\n",
	      vs[i].val1, vs[i].val2, key);
    }
  }
  gettimeofday(&tm_end, NULL);
  printf("Inserting %u items:\t%.3f ms\n",
	 bindings, GetTimeDiffMSec(&tm_start, &tm_end));

  /* now do random selective lookup */
  gettimeofday(&tm_start, NULL);
  for (i = 0; i < bindings/SELECT_FACTOR; i++) {
    ValueStruct* vptr;
    j = rand() % bindings;
    snprintf(key, sizeof(key), "%ul", j);
    vptr = SymTable_get(sym, key);
    if (vptr == NULL) {
      fprintf(stderr, "WARNING: Couldn't retrieve value for key%s\n", key);
    }
    if (vptr->val1 != j || vptr->val2 != (j^bindings)) {
      fprintf(stderr, "WARNING: The value: (%d, %d) retrieved for key %s is incorrect.\n",
	      vptr->val1, vptr->val2, key);
    }
  } 
  gettimeofday(&tm_end, NULL);
  printf("Random selective lookup:\t%.3f ms\n", 
	 GetTimeDiffMSec(&tm_start, &tm_end));

  /* srand() is called before rand() to feed a different seed
     to the pseudorandom generater */
  srand(time(NULL));

  /* now randomly pick items and update their values */
  gettimeofday(&tm_start, NULL);  
  for (i = 0; i < bindings/SELECT_FACTOR; i++) {
    /* pick a random key */
    j = rand() % bindings;
    snprintf(key, sizeof(key), "%ul", j);
    
    /* remove the key and re-insert it with the same value */
    if (SymTable_remove(sym, key) == NULL) {
      fprintf(stderr, "WARNING: Value of key %s could not be removed!\n", key);
    }
    if (SymTable_put(sym, key, &vs[j]) != 1) {
      fprintf(stderr, "WARNING: Value of key %s could not be inserted.\n", key);
    }
  } 
  gettimeofday(&tm_end, NULL);
  printf("Random updating test:\t%.3f ms\n", 
	 GetTimeDiffMSec(&tm_start, &tm_end));
  /* 
   * check if mapping works:
   * "increment val2 of each ValueStruct element with 10" 
   */
  i = 10;
  gettimeofday(&tm_start, NULL);
  
  SymTable_map(sym, update, &i);
  for (i = 0; i < bindings/SELECT_FACTOR; i++) {
    ValueStruct *res;

    j = rand() % bindings;
    snprintf(key, sizeof(key), "%ul", j);
    res = (ValueStruct*)SymTable_get(sym, key);
    if (res == NULL || res->val1 != j || res->val2 != (j^bindings)+10) {
      fprintf(stderr, "WARNING: Mapping error for key %s!\n", key);
    }    
  }
  gettimeofday(&tm_end, NULL);
  printf("Mapping and update:\t%.3f ms\n", 
	 GetTimeDiffMSec(&tm_start, &tm_end));

  printf("Total time taken:\t%.3f ms\n", 
	  GetTimeDiffMSec(&tm_first, &tm_end));

  /* make sure you free all the keys in SymTable_free()  */
  SymTable_free(sym);
  free( vs );

  return(EXIT_SUCCESS);
}

