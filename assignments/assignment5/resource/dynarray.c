/*--------------------------------------------------------------------*/
/* dynarray.c                                                         */
/* Author: Bob Dondero                                                */
/*--------------------------------------------------------------------*/

#include "dynarray.h"
#include <assert.h>
#include <stdlib.h>

enum {MIN_PHYS_LENGTH = 2};
enum {GROWTH_FACTOR = 2};

/*--------------------------------------------------------------------*/

/* A DynArray consists of an array, along with its logical and
   physical lengths. */

struct DynArray
{
  /* The number of elements in the DynArray from the client's
     point of view. */
  int iLength;

  /* The number of elements in the array that underlies the
     DynArray. */
  int iPhysLength;

  /* The array that underlies the DynArray. */
  const void **ppvArray;
};

/*--------------------------------------------------------------------*/

#ifndef NDEBUG
static int DynArray_isValid(DynArray_T oDynArray)

  /* Check the invariants of oDynArray.  Return 1 (TRUE) iff oDynArray
     is in a valid state. */

{
  if (oDynArray->iLength < 0) return 0;
  if (oDynArray->iPhysLength < MIN_PHYS_LENGTH) return 0;
  if (oDynArray->iLength > oDynArray->iPhysLength) return 0;
  if (oDynArray->ppvArray == NULL) return 0;
  return 1;
}
#endif

/*--------------------------------------------------------------------*/

DynArray_T DynArray_new(int iLength)

  /* Return a new DynArray_T whose length is iLength.
     It is a checked runtime error for iLength to be negative. */

{
  DynArray_T oDynArray;

  assert(iLength >= 0);

  oDynArray = (struct DynArray*)malloc(sizeof(struct DynArray));
  assert(oDynArray != NULL);
  oDynArray->iLength = iLength;
  if (iLength > MIN_PHYS_LENGTH)
    oDynArray->iPhysLength = iLength;
  else
    oDynArray->iPhysLength = MIN_PHYS_LENGTH;
  oDynArray->ppvArray =
    (const void**)calloc((size_t)oDynArray->iPhysLength,
        sizeof(void*));
  assert(oDynArray->ppvArray != NULL);

  return oDynArray;
}

/*--------------------------------------------------------------------*/

void DynArray_free(DynArray_T oDynArray)

  /* Free oDynArray. */

{
  if (oDynArray == NULL)
    return;

  free(oDynArray->ppvArray);
  free(oDynArray);
}

/*--------------------------------------------------------------------*/

int DynArray_getLength(DynArray_T oDynArray)

  /* Return the length of oDynArray.
     It is a checked runtime error for oDynArray to be NULL. */

{
  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));

  return oDynArray->iLength;
}

/*--------------------------------------------------------------------*/

void *DynArray_get(DynArray_T oDynArray, int iIndex)

  /* Return the iIndex'th element of oDynArray.
     It is a checked runtime error for oDynArray to be NULL.
     It is a checked runtime error for iIndex to be less than 0 or
     greater than or equal to the length of oDynArray. */

{
  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));
  assert(iIndex >= 0);
  assert(iIndex < oDynArray->iLength);

  return (void*)(oDynArray->ppvArray)[iIndex];
}

/*--------------------------------------------------------------------*/

void *DynArray_set(DynArray_T oDynArray, int iIndex,
    const void *pvElement)

  /* Assign pvElement to the iIndex'th element of oDynArray.  Return the 
     old element.
     It is a checked runtime error for oDynArray to be NULL.
     It is a checked runtime error for iIndex to be less than 0 or
     greater than or equal to the length of oDynArray. */

{
  const void *pvOldElement;

  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));
  assert(iIndex >= 0);
  assert(iIndex < oDynArray->iLength);

  pvOldElement = oDynArray->ppvArray[iIndex];
  oDynArray->ppvArray[iIndex] = pvElement;
  return (void*)pvOldElement;
}

/*--------------------------------------------------------------------*/

static void DynArray_grow(DynArray_T oDynArray)

  /* Double the physical length of oDynArray. */

{
  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));

  oDynArray->iPhysLength *= GROWTH_FACTOR;
  oDynArray->ppvArray =
    (const void**)realloc(oDynArray->ppvArray,
        sizeof(void*) * oDynArray->iPhysLength);
  assert(oDynArray->ppvArray != NULL);
}

/*--------------------------------------------------------------------*/

int DynArray_add(DynArray_T oDynArray, const void *pvElement)

  /* Add pvElement to the end of oDynArray, thus incrementing its length.
     It is a checked runtime error for oDynArray to be NULL. */

{
  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));

  if (oDynArray->iLength == oDynArray->iPhysLength)
    DynArray_grow(oDynArray);

  oDynArray->ppvArray[oDynArray->iLength] = pvElement;
  oDynArray->iLength++;
  return 1;
}

/*--------------------------------------------------------------------*/

void DynArray_addAt(DynArray_T oDynArray, int iIndex,
    const void *pvElement)

  /* Add pvElement to oDynArray such that it is the iIndex'th element.
     It is a checked runtime error for oDynArray to be NULL. 
     It is a checked runtime error for iIndex to be less than 0 or
     greater than the length of oDynArray. */

{
  int i;

  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));
  assert(iIndex >= 0);
  assert(iIndex <= oDynArray->iLength);

  if (oDynArray->iLength == oDynArray->iPhysLength)
    DynArray_grow(oDynArray);

  for (i = oDynArray->iLength; i > iIndex; i--)
    oDynArray->ppvArray[i] = oDynArray->ppvArray[i-1];

  oDynArray->ppvArray[iIndex] = pvElement;
  oDynArray->iLength++;
}

/*--------------------------------------------------------------------*/

void *DynArray_removeAt(DynArray_T oDynArray, int iIndex)

  /* Remove and return the iIndex'th element of oDynArray.
     It is a checked runtime error for oDynArray to be NULL. 
     It is a checked runtime error for iIndex to be less than 0 or
     greater than or equal to the length of oDynArray. */

{
  const void *pvOldElement;
  int i;

  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));
  assert(iIndex >= 0);
  assert(iIndex < oDynArray->iLength);

  pvOldElement = oDynArray->ppvArray[iIndex];

  oDynArray->iLength--;

  for (i = iIndex; i < oDynArray->iLength; i++)
    oDynArray->ppvArray[i] = oDynArray->ppvArray[i+1];

  return (void*)pvOldElement;
}

/*--------------------------------------------------------------------*/

void DynArray_toArray(DynArray_T oDynArray, void **ppvArray)

  /* Fill ppvArray with the elements of oDynArray.
     It is a checked runtime error for oDynArray or ppvArray to be NULL.
     It is an unchecked runtime error for *ppvArray to be too small
     to hold all elements of oDynArray.  */

{
  int i;

  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));
  assert(ppvArray != NULL);

  for (i = 0; i < oDynArray->iLength; i++)
    ppvArray[i] = (void*)oDynArray->ppvArray[i];
}

/*--------------------------------------------------------------------*/

void DynArray_map(DynArray_T oDynArray,
    void (*pfApply)(void *pvElement, void *pvExtra),
    const void *pvExtra)

  /* Apply function *pfApply to each element of oDynArray, passing
     pvExtra as an extra argument.  That is, for each element pvElement
     of oDynArray, call (*pfApply)(pvElement, pvExtra).
     It is a checked runtime error for oDynArray or pfApply to be
     NULL. */

{
  int i;

  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));
  assert(pfApply != NULL);

  for (i = 0; i < oDynArray->iLength; i++)
    (*pfApply)((void*)oDynArray->ppvArray[i], (void*)pvExtra);
}

/*--------------------------------------------------------------------*/

static void DynArray_swap(const void *ppvArray[], int iOne, int iTwo)

  /* Swap ppvArray[iOne] and ppvArray[iTwo]. */

{
  const void *pvTemp;
  pvTemp = ppvArray[iOne];
  ppvArray[iOne] = ppvArray[iTwo];
  ppvArray[iTwo] = pvTemp;
}

/*--------------------------------------------------------------------*/

static int DynArray_partition(const void *ppvArray[],
    int iLeft, int iRight,
    int (*pfCompare)(const void *pvElement1, const void *pvElement2))

  /* Divide ppvArray[iLeft...iRight] into two partitions so elements
     in the first partition are <= elements in the second partition.
     Return the index of the element that marks the partition
     boundary. The sort order is determined by *pfCompare. */

  /* This function is a variation of the partition() function shown in
     the book "Algorithms in C" by Robert Sedgewick. */

{
  int iFirst = iLeft-1;
  int iLast = iRight;

  while (1)
  {
    while ((*pfCompare)(ppvArray[++iFirst], ppvArray[iRight]) < 0)
      ;
    while ((*pfCompare)(ppvArray[iRight], ppvArray[--iLast]) < 0)
      if (iLast == iLeft)
        break;
    if (iFirst >= iLast)
      break;
    DynArray_swap(ppvArray, iFirst, iLast);
  }
  DynArray_swap(ppvArray, iFirst, iRight);
  return iFirst;
}

/*--------------------------------------------------------------------*/

static void DynArray_quicksort(const void *ppvArray[],
    int iLeft, int iRight,
    int (*pfCompare)(const void *pvElement1, const void *pvElement2))

  /* Sort ppvArray[iLeft...iRight] in ascending order, as determined
     by *pfCompare. */

  /* This function is a variation of the quicksort() function shown in
     the book "Algorithms in C" by Robert Sedgewick. */

{
  int iMid;
  if (iRight > iLeft)
  {
    iMid = DynArray_partition(ppvArray, iLeft, iRight, pfCompare);
    DynArray_quicksort(ppvArray, iLeft, iMid - 1, pfCompare);
    DynArray_quicksort(ppvArray, iMid + 1, iRight, pfCompare);
  }
}

/*--------------------------------------------------------------------*/

void DynArray_sort(DynArray_T oDynArray,
    int (*pfCompare)(const void *pvElement1, const void *pvElement2))

  /* Sort oDynArray in the order determined by *pfCompare.
   *pfCompare should return <0, 0, or >0 depending upon whether
   *pvElement1 is less than, equal to, or greater than *pvElement2,
   respectively.
   It is a checked runtime error for oDynArray or pfCompare to be
   NULL. */

{
  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));
  assert(pfCompare != NULL);

  DynArray_quicksort(oDynArray->ppvArray, 0, oDynArray->iLength-1,
      pfCompare);
}

/*--------------------------------------------------------------------*/

int DynArray_search(DynArray_T oDynArray, void *pvSoughtElement,
    int (*pfCompare)(const void *pvElement1, const void *pvElement2))

  /* Linear search oDynArray for *pvSoughtElement using *pfCompare to
     determine equality.  Return the index at which *pvSoughtElement
     is found, or -1 if there is no such index.
   *pfCompare should return 0 if *pvElement1 is equal to pvElement2,
   and non-0 otherwise.
   It is a checked runtime error for oDynArray or pfCompare to be
   NULL. */

{
  int i;

  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));
  assert(pfCompare != NULL);

  for (i = 0; i < oDynArray->iLength; i++)
    if ((*pfCompare)(oDynArray->ppvArray[i], pvSoughtElement) == 0)
      return i;
  return -1;
}

/*--------------------------------------------------------------------*/

int DynArray_bsearch(DynArray_T oDynArray, void *pvSoughtElement,
    int (*pfCompare)(const void *pvElement1, const void *pvElement2))

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

{
  int iLeft;
  int iRight;
  int iMid;
  int iCompare;

  assert(oDynArray != NULL);
  assert(DynArray_isValid(oDynArray));
  assert(pfCompare != NULL);

  iLeft = 0;
  iRight = oDynArray->iLength - 1;

  while (iLeft <= iRight)
  {
    iMid = (iLeft + iRight) / 2;
    iCompare =
      (*pfCompare)(pvSoughtElement, oDynArray->ppvArray[iMid]);
    if (iCompare == 0)
      return iMid;
    if (iCompare < 0)
      iRight = iMid - 1;
    else
      iLeft = iMid + 1;
  }

  return -1;
}
