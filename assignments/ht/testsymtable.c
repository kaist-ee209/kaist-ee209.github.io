/*------------------------------------------------------------------*/
/* testsymtable.c                                                   */
/* Original Author: Bob Dondero                                     */
/*------------------------------------------------------------------*/

#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum {FALSE, TRUE};

/*------------------------------------------------------------------*/

#define ASSURE(i) assure(i, __LINE__)

static void assure(int iSuccessful, int iLineNum)

/* If !iSuccessful, print an error message indicating that the test
   at line iLineNum failed. */

{
   if (! iSuccessful)
      printf("Test at line %d failed.\n", iLineNum);
}

/*------------------------------------------------------------------*/

static void printBinding(const char *pcKey, void *pvValue,
   void *pvExtra)

/* Print the binding whose key is pcKey and whose string value is
   pvValue using format string pvExtra. */

{
   char *pcValue;
   char *pcExtra;

   ASSURE(pcKey != NULL);
   ASSURE(pvValue != NULL);

   pcValue = (char*)pvValue;
   pcExtra = (char*)pvExtra;

   printf(pcExtra, pcKey, pcValue);
}

/*------------------------------------------------------------------*/

static void testSmallTable(void)

/* Test a SymTable that contains a few bindings. */

{
   SymTable_T oSymTable;
   char acRuth[] = "Babe Ruth";
   char acGehrig[] = "Lou Gehrig";
   char acMantle[] = "Mickey Mantle";
   char acJeter[] = "Derek Jeter";
   char acRuth2[] = "Babe Ruth";
   char acPitcher[] = "Pitcher";
   char acRightField[] = "Right Field";
   char acRightField2[] = "Right Field";
   char acFirstBase[] = "First Base";
   char acCenterField[] = "Center Field";
   char acShortstop[] = "Shortstop";
   char *pcValue;
   int iFound;
   int iLength;
   int iResult;

   printf("------------------------------------------------------\n");
   printf("Testing a small SymTable.\n");

   /* Test SymTable_new. */

   oSymTable = SymTable_new();
   ASSURE(oSymTable != NULL);

   /* Test SymTable_put and SymTable_getLength. */

   iResult = SymTable_put(oSymTable, acRuth, acPitcher);
   ASSURE(iResult == TRUE);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 1);

   iResult = SymTable_put(oSymTable, acGehrig, acFirstBase);
   ASSURE(iResult == TRUE);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 2);

   iResult = SymTable_put(oSymTable, acMantle, acCenterField);
   ASSURE(iResult == TRUE);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 3);

   iResult = SymTable_put(oSymTable, acJeter, acShortstop);
   ASSURE(iResult == TRUE);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 4);

   iResult = SymTable_put(oSymTable, acRuth, acRightField);
   ASSURE(iResult == FALSE);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 4);

   iResult = SymTable_put(oSymTable, acRuth2, acRightField2);
   ASSURE(iResult == FALSE);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 4);

   iResult = SymTable_put(oSymTable, acRuth2, acRightField);
   ASSURE(iResult == FALSE);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 4);

   /* Test SymTable_contains. */

   iFound = SymTable_contains(oSymTable, acRuth);
   ASSURE(iFound);

   iFound = SymTable_contains(oSymTable, acGehrig);
   ASSURE(iFound);

   iFound = SymTable_contains(oSymTable, acMantle);
   ASSURE(iFound);

   iFound = SymTable_contains(oSymTable, acJeter);
   ASSURE(iFound);

   iFound = SymTable_contains(oSymTable, acRuth2);
   ASSURE(iFound);

   iFound = SymTable_contains(oSymTable, "Yogi Berra");
   ASSURE(! iFound);

   /* Test SymTable_get. */

   pcValue = (char*)SymTable_get(oSymTable, acRuth);
   ASSURE(pcValue == acPitcher);

   pcValue = (char*)SymTable_get(oSymTable, acGehrig);
   ASSURE(pcValue == acFirstBase);

   pcValue = (char*)SymTable_get(oSymTable, acMantle);
   ASSURE(pcValue == acCenterField);

   pcValue = (char*)SymTable_get(oSymTable, acJeter);
   ASSURE(pcValue == acShortstop);

   pcValue = (char*)SymTable_get(oSymTable, acRuth2);
   ASSURE(pcValue == acPitcher);

   pcValue = (char*)SymTable_get(oSymTable, "Yogi Berra");
   ASSURE(pcValue == NULL);

   /* Test SymTable_map. */

   printf("Should print 4 players and their positions.\n");
   SymTable_map(oSymTable, printBinding, "%s\t%s\n");

   /* Test SymTable_remove. */

   pcValue = (char*)SymTable_remove(oSymTable, acRuth);
   ASSURE(pcValue == acPitcher);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 3);

   iFound = SymTable_contains(oSymTable, acRuth);
   ASSURE(! iFound);

   pcValue = (char*)SymTable_remove(oSymTable, acJeter);
   ASSURE(pcValue == acShortstop);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 2);

   iFound = SymTable_contains(oSymTable, acJeter);
   ASSURE(! iFound);

   pcValue = (char*)SymTable_remove(oSymTable, "Yogi Berra");
   ASSURE(pcValue == NULL);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 2);

   iFound = SymTable_contains(oSymTable, "Yogi Berra");
   ASSURE(! iFound);

   pcValue = (char*)SymTable_remove(oSymTable, acJeter);
   ASSURE(pcValue == NULL);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 2);

   iFound = SymTable_contains(oSymTable, acJeter);
   ASSURE(! iFound);

   /* Test SymTable_free. */

   SymTable_free(oSymTable);
}

/*--------------------------------------------------------------------*/

static void testEmptyTable(void)

/* Test a SymTable that contains no bindings. */

{
   SymTable_T oSymTable;
   char *pcValue;
   int iFound;
   int iLength;

   printf("------------------------------------------------------\n");
   printf("Testing an empty SymTable.\n");

   oSymTable = SymTable_new();
   ASSURE(oSymTable != NULL);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 0);

   iFound = SymTable_contains(oSymTable, "Ruth");
   ASSURE(! iFound);

   pcValue = (char*)SymTable_get(oSymTable, "Ruth");
   ASSURE(pcValue == NULL);

   pcValue = (char*)SymTable_remove(oSymTable, "Ruth");
   ASSURE(pcValue == NULL);

   SymTable_map(oSymTable, printBinding, "%s\t%s\n");

   SymTable_free(oSymTable);
}

/*------------------------------------------------------------------*/

static void testEmptyKey(void)

/* Test a SymTable that contains an empty key. */

{
   SymTable_T oSymTable;
   char *pcValue;
   char acRightField[] = "Right Field";
   int iFound;
   int iLength;
   int iResult;

   printf("------------------------------------------------------\n");
   printf("Testing a SymTable that contains an empty key.\n");

   oSymTable = SymTable_new();
   ASSURE(oSymTable != NULL);

   iResult = SymTable_put(oSymTable, "", acRightField);
   ASSURE(iResult == TRUE);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 1);

   iFound = SymTable_contains(oSymTable, "");
   ASSURE(iFound);

   pcValue = (char*)SymTable_get(oSymTable, "");
   ASSURE(pcValue == acRightField);

   printf("Should print 1 player (with empty name) and position.\n");
   SymTable_map(oSymTable, printBinding, "%s\t%s\n");

   pcValue = (char*)SymTable_remove(oSymTable, "");
   ASSURE(pcValue == acRightField);

   SymTable_free(oSymTable);
}

/*------------------------------------------------------------------*/

static void testNullValue(void)

/* Test a SymTable that contains a NULL value. */

{
   SymTable_T oSymTable;
   char *pcValue;
   int iFound;
   int iLength;
   int iResult;

   printf("------------------------------------------------------\n");
   printf("Testing a SymTable that contains a NULL value.\n");

   oSymTable = SymTable_new();
   ASSURE(oSymTable != NULL);

   iResult = SymTable_put(oSymTable, "Ruth", NULL);
   ASSURE(iResult == TRUE);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 1);

   iFound = SymTable_contains(oSymTable, "Ruth");
   ASSURE(iFound);

   pcValue = (char*)SymTable_get(oSymTable, "Ruth");
   ASSURE(pcValue == NULL);

   pcValue = (char*)SymTable_remove(oSymTable, "Ruth");
   ASSURE(pcValue == NULL);

   SymTable_free(oSymTable);
}

/*------------------------------------------------------------------*/

static void testLargeKey(void)

/* Test a SymTable that contains a large key. */

{
   enum {KEY_SIZE = 1000};

   SymTable_T oSymTable;
   char acKeyA[KEY_SIZE];
   char acKeyB[KEY_SIZE];
   char *pcValue;
   char acRightField[] = "Right Field";
   int iFound;
   int i;
   int iLength;
   int iResult;

   printf("------------------------------------------------------\n");
   printf("Testing a SymTable that contains a long key.\n");

   for (i = 0; i < KEY_SIZE - 1; i++)
      acKeyA[i] = 'a';
   acKeyA[999] = '\0';

   for (i = 0; i < KEY_SIZE - 1; i++)
      acKeyB[i] = 'b';
   acKeyB[KEY_SIZE - 1] = '\0';

   oSymTable = SymTable_new();
   ASSURE(oSymTable != NULL);

   iResult = SymTable_put(oSymTable, acKeyA, acRightField);
   ASSURE(iResult == TRUE);
   iResult = SymTable_put(oSymTable, acKeyB, acRightField);
   ASSURE(iResult == TRUE);
   iResult = SymTable_put(oSymTable, acKeyB, acRightField);
   ASSURE(iResult == FALSE);

   iLength = SymTable_getLength(oSymTable);
   ASSURE(iLength == 2);

   iFound = SymTable_contains(oSymTable, acKeyA);
   ASSURE(iFound);
   iFound = SymTable_contains(oSymTable, acKeyB);
   ASSURE(iFound);

   pcValue = (char*)SymTable_get(oSymTable, acKeyA);
   ASSURE(pcValue == acRightField);
   pcValue = (char*)SymTable_get(oSymTable, acKeyB);
   ASSURE(pcValue == acRightField);

   pcValue = (char*)SymTable_remove(oSymTable, acKeyB);
   ASSURE(pcValue == acRightField);
   pcValue = (char*)SymTable_remove(oSymTable, acKeyB);
   ASSURE(pcValue == NULL);
   pcValue = (char*)SymTable_remove(oSymTable, acKeyA);
   ASSURE(pcValue == acRightField);

   SymTable_free(oSymTable);
}

/*------------------------------------------------------------------*/

static void testTableOfTables(void)

/* Test a SymTable whose values are other SymTables. */

{
   SymTable_T oSymTable;
   SymTable_T oSymTable1;
   SymTable_T oSymTable2;
   SymTable_T oSymTableRet;
   char acRightField[] = "Right Field";
   char acFirstBase[] = "First Base";
   char *pcValue;
   int iResult;

   printf("------------------------------------------------------\n");
   printf("Testing a SymTable that contains other SymTables.\n");

   oSymTable1 = SymTable_new();
   ASSURE(oSymTable1 != NULL);

   iResult = SymTable_put(oSymTable1, "Ruth", acRightField);
   ASSURE(iResult == TRUE);

   oSymTable2 = SymTable_new();
   ASSURE(oSymTable2 != NULL);

   iResult = SymTable_put(oSymTable2, "Gehrig", acFirstBase);
   ASSURE(iResult == TRUE);

   oSymTable = SymTable_new();
   ASSURE(oSymTable != NULL);

   iResult = SymTable_put(oSymTable, "first table", oSymTable1);
   ASSURE(iResult == TRUE);

   iResult = SymTable_put(oSymTable, "second table", oSymTable2);
   ASSURE(iResult == TRUE);

   oSymTableRet = (SymTable_T)SymTable_get(oSymTable, "first table");
   ASSURE(oSymTableRet == oSymTable1);

   pcValue = (char*)SymTable_get(oSymTableRet, "Ruth");
   ASSURE(pcValue == acRightField);

   oSymTableRet = SymTable_get(oSymTable, "second table");
   ASSURE(oSymTableRet == oSymTable2);

   pcValue = (char*)SymTable_get(oSymTableRet, "Gehrig");
   ASSURE(pcValue == acFirstBase);

   SymTable_free(oSymTable2);
   SymTable_free(oSymTable1);
   SymTable_free(oSymTable);
}

/*------------------------------------------------------------------*/

static void testLargeTable(int iBindingCount)

/* Test a SymTable that contains iBindingCount bindings. */

{
   enum {MAX_KEY_LENGTH = 10};

   SymTable_T oSymTable;
   char acKey[MAX_KEY_LENGTH];
   char *pcValue;
   int i;
   int iSmall;
   int iLarge;
   clock_t iInitialClock;
   clock_t iFinalClock;
   int iResult;

   printf("------------------------------------------------------\n");
   printf("Testing a potentially large SymTable.\n");

   iInitialClock = clock();

   oSymTable = SymTable_new();

   /* Put iBindingCount new binding into oSymTable.  Each binding's
      key and value contain the same characters. */

   for (i = 0; i < iBindingCount; i++)
   {
      sprintf(acKey, "%d", i);
      pcValue = (char*)malloc(sizeof(char) * (strlen(acKey) + 1));
      ASSURE(pcValue != NULL);
      strcpy(pcValue, acKey);
      iResult = SymTable_put(oSymTable, acKey, pcValue);
      ASSURE(iResult == TRUE);
   }

   /* Get each binding's value, and make sure that it contains
      the same characters as its key. */

   iSmall = 0;
   iLarge = iBindingCount - 1;
   while (iSmall < iLarge)
   {
      /* Get the smallest of the remaining bindings. */
      sprintf(acKey, "%d", iSmall);
      pcValue = (char*)SymTable_get(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
      iSmall++;
      /* Get the largest of the remaining bindings. */
      sprintf(acKey, "%d", iLarge);
      pcValue = (char*)SymTable_get(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
      iLarge--;
   }
   /* Get the middle binding -- if there is one. */
   if (iSmall == iLarge)
   {
      sprintf(acKey, "%d", iSmall);
      pcValue = (char*)SymTable_get(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
   }

   /* Remove each binding. Also free each binding's value. */

   iSmall = 0;
   iLarge = iBindingCount - 1;
   while (iSmall < iLarge)
   {
      /* Remove the smallest of the remaining bindings. */
      sprintf(acKey, "%d", iSmall);
      pcValue = (char*)SymTable_remove(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
      free(pcValue);
      iSmall++;
      /* Remove the largest of the remaining bindings. */
      sprintf(acKey, "%d", iLarge);
      pcValue = (char*)SymTable_remove(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
      free(pcValue);
      iLarge--;
   }
   /* Remove the middle binding -- if there is one. */
   if (iSmall == iLarge)
   {
      sprintf(acKey, "%d", iSmall);
      pcValue = (char*)SymTable_remove(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
      free(pcValue);
   }

   SymTable_free(oSymTable);

   iFinalClock = clock();
   printf("CPU time (%d bindings):  %f seconds\n", iBindingCount,
      ((double)(iFinalClock - iInitialClock)) / CLOCKS_PER_SEC);
}

/*------------------------------------------------------------------*/

static void testMultipleTables(void)

/* Test multiple simultaneous SymTables, one containing many bindings
   and one containing few bindings. */

{
   enum {BINDING_COUNT = 1000};
   SymTable_T oSymTable1;
   SymTable_T oSymTable2;
   char *pcValue;
   int iLength;
   int iFound;
   int i;
   char acKey[10];
   int iResult;

   printf("------------------------------------------------------\n");
   printf("Testing multiple simultaneous SymTables.\n");

   oSymTable1 = SymTable_new();
   iResult = SymTable_put(oSymTable1, "x", "x");
   ASSURE(iResult == TRUE);
   iResult = SymTable_put(oSymTable1, "y", "y");
   ASSURE(iResult == TRUE);

   oSymTable2 = SymTable_new();
   for (i = 0; i < BINDING_COUNT; i++)
   {
      sprintf(acKey, "%d", i);
      pcValue = (char*)malloc(sizeof(char) * (strlen(acKey) + 1));
      ASSURE(pcValue != NULL);
      strcpy(pcValue, acKey);
      iResult = SymTable_put(oSymTable2, acKey, pcValue);
      ASSURE(iResult == TRUE);
   }

   iLength = SymTable_getLength(oSymTable1);
   ASSURE(iLength == 2);
   iFound = SymTable_contains(oSymTable1, "x");
   ASSURE(iFound);
   iFound = SymTable_contains(oSymTable1, "y");
   ASSURE(iFound);
   pcValue = (char*)SymTable_get(oSymTable1, "x");
   ASSURE((pcValue != NULL) && (strcmp(pcValue, "x") == 0));
   pcValue = (char*)SymTable_get(oSymTable1, "y");
   ASSURE((pcValue != NULL) && (strcmp(pcValue, "y") == 0));
   iFound = SymTable_contains(oSymTable1, "0");
   ASSURE(! iFound);
   iFound = SymTable_contains(oSymTable1, "99");
   ASSURE(! iFound);
   iFound = SymTable_contains(oSymTable1, "999");
   ASSURE(! iFound);

   iLength = SymTable_getLength(oSymTable2);
   ASSURE(iLength == BINDING_COUNT);
   iFound = SymTable_contains(oSymTable2, "0");
   ASSURE(iFound);
   iFound = SymTable_contains(oSymTable2, "99");
   ASSURE(iFound);
   iFound = SymTable_contains(oSymTable2, "999");
   ASSURE(iFound);
   pcValue = (char*)SymTable_get(oSymTable2, "0");
   ASSURE((pcValue != NULL) && (strcmp(pcValue, "0") == 0));
   pcValue = (char*)SymTable_get(oSymTable2, "99");
   ASSURE((pcValue != NULL) && (strcmp(pcValue, "99") == 0));
   pcValue = (char*)SymTable_get(oSymTable2, "999");
   ASSURE((pcValue != NULL) && (strcmp(pcValue, "999") == 0));
   iFound = SymTable_contains(oSymTable2, "x");
   ASSURE(! iFound);
   iFound = SymTable_contains(oSymTable2, "y");
   ASSURE(! iFound);

   for (i = 0; i < BINDING_COUNT; i++)
   {
      sprintf(acKey, "%d", i);
      pcValue = (char*)SymTable_get(oSymTable2, acKey);
      ASSURE(pcValue != NULL);
      free(pcValue);
   }

   SymTable_free(oSymTable2);
   SymTable_free(oSymTable1);
}

/*------------------------------------------------------------------*/

int main(int argc, char *argv[])

/* Test a SymTable_T whose values are strings.  No output indicates
   success.  argv[1] contains the number of bindings to create
   within a potentially large SymTable_T.  Return 0. */

{
   int iBindingCount;

   if (argc != 2)
   {
      fprintf(stderr, "Usage:  %s bindingcount\n", argv[0]);
      return EXIT_FAILURE;
   }

   if (sscanf(argv[1], "%d", &iBindingCount) != 1)
   {
      fprintf(stderr, "bindingcount must be numeric\n");
      return EXIT_FAILURE;
   }

   testSmallTable();
   testEmptyTable();
   testEmptyKey();
   testNullValue();
   testLargeKey();
   testTableOfTables();
   testLargeTable(iBindingCount);
   testMultipleTables();

   printf("------------------------------------------------------\n");
   printf("End of %s.\n", argv[0]);
   return 0;
}
