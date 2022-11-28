/*--------------------------------------------------------------------*/
/* dynarray.h                                                         */
/* Author: Bob Dondero                                                */
/*--------------------------------------------------------------------*/

#ifndef DYNARRAY_INCLUDED
#define DYNARRAY_INCLUDED

typedef struct DynArray *DynArray_T;
/* A DynArray_T is an array whose length can expand dynamically. */

DynArray_T DynArray_new(int iLength);
/* Return a new DynArray_T whose length is iLength.
   It is a checked runtime error for iLength to be negative. */

void DynArray_free(DynArray_T oDynArray);
/* Free oDynArray. */

int DynArray_getLength(DynArray_T oDynArray);
/* Return the length of oDynArray.
   It is a checked runtime error for oDynArray to be NULL. */

void *DynArray_get(DynArray_T oDynArray, int iIndex);
/* Return the iIndex'th element of oDynArray.
   It is a checked runtime error for oDynArray to be NULL.
   It is a checked runtime error for iIndex to be less than 0 or
   greater than or equal to the length of oDynArray. */

void *DynArray_set(DynArray_T oDynArray, int iIndex,
    const void *pvElement);
/* Assign pvElement to the iIndex'th element of oDynArray.  Return the
   old element.
   It is a checked runtime error for oDynArray to be NULL.
   It is a checked runtime error for iIndex to be less than 0 or
   greater than or equal to the length of oDynArray. */

int DynArray_add(DynArray_T oDynArray, const void *pvElement);
/* Add pvElement to the end of oDynArray, thus incrementing its length.
   It is a checked runtime error for oDynArray to be NULL. */

void DynArray_addAt(DynArray_T oDynArray, int iIndex,
    const void *pvElement);
/* Add pvElement to oDynArray such that it is the iIndex'th element.
   It is a checked runtime error for oDynArray to be NULL. 
   It is a checked runtime error for iIndex to be less than 0 or
   greater than the length of oDynArray. */

void *DynArray_removeAt(DynArray_T oDynArray, int iIndex);
/* Remove and return the iIndex'th element of oDynArray.
   It is a checked runtime error for oDynArray to be NULL. 
   It is a checked runtime error for iIndex to be less than 0 or
   greater than or equal to the length of oDynArray. */

void DynArray_toArray(DynArray_T oDynArray, void **ppvArray);
/* Fill ppvArray with the elements of oDynArray.
   It is a checked runtime error for oDynArray or ppvArray to be NULL.
   It is an unchecked runtime error for ppvArray to be too small
   to hold all elements of oDynArray.  */

void DynArray_map(DynArray_T oDynArray,
    void (*pfApply)(void *pvElement, void *pvExtra),
    const void *pvExtra);
/* Apply function *pfApply to each element of oDynArray, passing
   pvExtra as an extra argument.  That is, for each element pvElement of
   oDynArray, call (*pfApply)(pvElement, pvExtra).
   It is a checked runtime error for oDynArray or pfApply to be
   NULL. */

void DynArray_sort(DynArray_T oDynArray,
    int (*pfCompare)(const void *pvElement1, const void *pvElement2));
/* Sort oDynArray in the order determined by *pfCompare.
 *pfCompare should return -<0, 0, or >0 depending upon whether
 *pvElement1 is less than, equal to, or greater than *pvElement2,
 respectively.
 It is a checked runtime error for oDynArray or pfCompare to be
 NULL. */

int DynArray_search(DynArray_T oDynArray, void *pvSoughtElement,
    int (*pfCompare)(const void *pvElement1, const void *pvElement2));
/* Linear search oDynArray for *pvSoughtElement using *pfCompare to
   determine equality.  Return the index at which *pvSoughtElement
   is found, or -1 if there is no such index.
 *pfCompare should return 0 if *pvElement1 is equal to pvElement2,
 and non-0 otherwise.
 It is a checked runtime error for oDynArray or pfCompare to be
 NULL. */

int DynArray_bsearch(DynArray_T oDynArray, void *pvSoughtElement,
    int (*pfCompare)(const void *pvElement1, const void *pvElement2));
/* Binary search oDynArray for *pvSoughtElement using *pfCompare to
   determine equality.  Return the index at which *pvSoughtElement
   is found, or -1 if there is no such index.
 *pfCompare should return <0, 0, or >0 depending upon whether
 *pvElement1 is less than, equal to, or greater than *pvElement2,
 respectively.
 It is a checked runtime error for oDynArray or pfCompare to be
 NULL.
 It is an unchecked runtime error for oDynArray not to be sorted
 as determined by *pfCompare. */

#endif
