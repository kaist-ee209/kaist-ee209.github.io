/**********************
 * EE209 Assignment 3 *
 **********************/
/* testclient.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

#include "customer_manager.h"

/*--------------------------------------------------------------------*/
int
TestRegisterCustomer(DB_T d, const char* id, const char* name,
					 int purchase, int expected_result)
{
	int test_result;

	printf("RegisterCustomer(d, \"%s\", \"%s\", %d);\n",
		   id, name, purchase);
	test_result = RegisterCustomer(d, id, name, purchase);

	if (expected_result == test_result)
		printf("[PASSED] ");
	else
		printf("[FAILED] ");
	printf("test result: %d / expected result: %d\n",
		   test_result, expected_result);

	return (expected_result == test_result)? 0 : -1;
}
/*--------------------------------------------------------------------*/
int
TestUnregisterCustomerByID(DB_T d, const char* id, int expected_result)
{
	int test_result;

	printf("UnregisterCustomerByID(d, \"%s\");\n", id);
	test_result = UnregisterCustomerByID(d, id);

	if (expected_result == test_result)
		printf("[PASSED] ");
	else
		printf("[FAILED] ");
	printf("test result: %d / expected result: %d\n",
		   test_result, expected_result);

	return (expected_result == test_result)? 0 : -1;
}
/*--------------------------------------------------------------------*/
int
TestUnregisterCustomerByName(DB_T d, const char* name,
							 int expected_result)
{
	int test_result;

	printf("UnregisterCustomerByName(d, \"%s\");\n", name);
	test_result = UnregisterCustomerByName(d, name);

	if (expected_result == test_result)
		printf("[PASSED] ");
	else
		printf("[FAILED] ");
	printf("test result: %d / expected result: %d\n",
		   test_result, expected_result);

	return (expected_result == test_result)? 0 : -1;
}
/*--------------------------------------------------------------------*/
int
TestGetPurchaseByID(DB_T d, const char* id, int expected_result)
{
	int test_result;

	printf("GetPurchaseByID(d, \"%s\");\n", id);
	test_result = GetPurchaseByID(d, id);

	if (expected_result == test_result)
		printf("[PASSED] ");
	else
		printf("[FAILED] ");
	printf("test result: %d / expected result: %d\n",
		   test_result, expected_result);

	return (expected_result == test_result)? 0 : -1;
}
/*--------------------------------------------------------------------*/
int
TestGetPurchaseByName(DB_T d, const char* name, int expected_result)
{
	int test_result;

	printf("GetPurchaseByName(d, \"%s\");\n", name);
	test_result = GetPurchaseByName(d, name);

	if (expected_result == test_result)
		printf("[PASSED] ");
	else
		printf("[FAILED] ");
	printf("test result: %d / expected result: %d\n",
		   test_result, expected_result);

	return (expected_result == test_result)? 0 : -1;
}
/*--------------------------------------------------------------------*/
int
NameStartsWithA(const char* id, const char* name, int purchase)
{
	if (*name == 'A')
		return purchase;
	else
		return 0;
}
/*--------------------------------------------------------------------*/
int
IDStartsWithA(const char* id, const char* name, int purchase)
{
	if (*id == 'A')
		return purchase;
	else
		return 0;
}
/*--------------------------------------------------------------------*/
int
PurchaseLargerThan100(const char* id, const char* name, int purchase)
{
	if (purchase > 100)
		return purchase;
	else
		return 0;
}
/*--------------------------------------------------------------------*/
int
TestGetSumCustomerPurchase(DB_T d, FUNCPTR_T fp, const char* fname,
						   int expected_result)
{
	int test_result;

	printf("GetSumCustomerPurchase(d, %s);\n", fname);
	test_result = GetSumCustomerPurchase(d, fp);

	if (expected_result == test_result)
		printf("[PASSED] ");
	else
		printf("[FAILED] ");
	printf("test result: %d / expected result: %d\n",
		   test_result, expected_result);

	return (expected_result == test_result)? 0 : -1;
}
/*--------------------------------------------------------------------*/
/* Correctness Test 1: RegisterCustomer only */
int
CorrectnessTest1() {

	DB_T d;
	int result;

	result = 0;
	printf("------------------------------------------------------\n" \
		   "  Correctness Test 1: RegisterCustomer only\n" \
		   "------------------------------------------------------\n");

	d = CreateCustomerDB();
	if (d == NULL) {
		printf("CreateCustomerDB() failed, cannot perform the test\n");
		return -1;
	}

	result += TestRegisterCustomer(d, "id1", "name1", 10, 0);
	result += TestRegisterCustomer(d, "id2", "name2", 10, 0);
	result += TestRegisterCustomer(d, "id3", "name3", 10, 0);
	result += TestRegisterCustomer(d, "id3", "name3", 10, -1);
	result += TestRegisterCustomer(d, "id2", "name2", 10, -1);
	result += TestRegisterCustomer(d, "id1", "name1", 10, -1);
	result += TestRegisterCustomer(d, "id4", "name1", 10, -1);
	result += TestRegisterCustomer(d, "id2", "name5", 10, -1);
	result += TestRegisterCustomer(d, "name3", "id3", 10, 0);
	result += TestRegisterCustomer(d, "name3", "id3", 10, -1);

	DestroyCustomerDB(d);

	printf("\nCorrectness Test 1 %s\n\n",
		   (result >= 0)? "PASSED" : "FAILED!");

	return (result >= 0)? 0 : -1;
}
/*--------------------------------------------------------------------*/
/* Correctness Test 2: RegisterCustomer and UnregisterCustomer */
int
CorrectnessTest2() {

	DB_T d;
	int result;

	result = 0;
	printf("------------------------------------------------------\n" \
		   "  Correctness Test 2: Register/UnregisterCustomer\n" \
		   "------------------------------------------------------\n");

	d = CreateCustomerDB();
	if (d == NULL) {
		printf("CreateCustomerDB() failed, cannot perform the test\n");
		return -1;
	}

	result += TestRegisterCustomer(d, "id1", "name1", 10, 0);
	result += TestRegisterCustomer(d, "id2", "name2", 10, 0);
	result += TestUnregisterCustomerByID(d, "id2", 0);
	result += TestUnregisterCustomerByName(d, "name1", 0);
	result += TestUnregisterCustomerByName(d, "name2", -1);
	result += TestUnregisterCustomerByID(d, "id1", -1);
	result += TestRegisterCustomer(d, "id3", "name3", 10, 0);
	result += TestRegisterCustomer(d, "id3", "name3", 10, -1);
	result += TestUnregisterCustomerByID(d, "id3", 0);
	result += TestUnregisterCustomerByID(d, "id3", -1);

	DestroyCustomerDB(d);

	printf("\nCorrectness Test 2 %s\n\n",
		   (result >= 0)? "PASSED" : "FAILED!");

	return (result >= 0)? 0 : -1;
}
/*--------------------------------------------------------------------*/
/* Correctness Test 3: RegisterCustomer and GetPurchaseByID/Name */
int
CorrectnessTest3() {

	DB_T d;
	int result;

	result = 0;
	printf("------------------------------------------------------\n" \
		   "  Correctness Test 3:\n" \
		   "  RegisterCustomer and GetPurchaseByID/Name\n" \
		   "------------------------------------------------------\n");

	d = CreateCustomerDB();
	if (d == NULL) {
		printf("CreateCustomerDB() failed, cannot perform the test\n");
		return -1;
	}

	result += TestRegisterCustomer(d, "id1", "name1", 100, 0);
	result += TestRegisterCustomer(d, "id2", "name2", 200, 0);
	result += TestGetPurchaseByName(d, "name1", 100);
	result += TestGetPurchaseByID(d, "id2", 200);
	result += TestGetPurchaseByName(d, "name3", -1);
	result += TestRegisterCustomer(d, "id3", "name3", 300, 0);
	result += TestGetPurchaseByID(d, "id3", 300);

	DestroyCustomerDB(d);

	printf("\nCorrectness Test 3 %s\n\n",
		   (result >= 0)? "PASSED" : "FAILED!");

	return (result >= 0)? 0 : -1;
}
/*--------------------------------------------------------------------*/
/* Correctness Test 4: Register/UnregisterCustomer and
   GetPurchaseByID/Name */
int
CorrectnessTest4() {

	DB_T d;
	int result;

	result = 0;
	printf("------------------------------------------------------\n" \
		   "  Correctness Test 4:\n" \
		   "  Register/UnregisterCustomer and GetPurchaseByID/Name\n" \
		   "------------------------------------------------------\n");

	d = CreateCustomerDB();
	if (d == NULL) {
		printf("CreateCustomerDB() failed, cannot perform the test\n");
		return -1;
	}

	result += TestRegisterCustomer(d, "id1", "name1", 100, 0);
	result += TestRegisterCustomer(d, "id2", "name2", 200, 0);
	result += TestUnregisterCustomerByName(d, "name2", 0);
	result += TestGetPurchaseByID(d, "id1", 100);
	result += TestGetPurchaseByName(d, "name2", -1);
	result += TestRegisterCustomer(d, "id1", "name2", 200, -1);
	result += TestGetPurchaseByID(d, "id1", 100);
	result += TestGetPurchaseByName(d, "name2", -1);
	result += TestRegisterCustomer(d, "id2", "name2", 400, 0);
	result += TestGetPurchaseByID(d, "id1", 100);
	result += TestGetPurchaseByName(d, "name2", 400);

	DestroyCustomerDB(d);

	printf("\nCorrectness Test 4 %s\n\n",
		   (result >= 0)? "PASSED" : "FAILED!");

	return (result >= 0)? 0 : -1;
}
/*--------------------------------------------------------------------*/
/* Correctness Test 5: Register/UnregisterCustomer and
   GetSumCustomerPurchase */
int
CorrectnessTest5() {

	DB_T d;
	int result;

	result = 0;
	printf("------------------------------------------------------\n" \
		   "  Correctness Test 5:\n" \
		   "  Register/UnregisterCustomer and GetSumCustomerPurchase\n"\
		   "------------------------------------------------------\n");

	d = CreateCustomerDB();
	if (d == NULL) {
		printf("CreateCustomerDB() failed, cannot perform the test\n");
		return -1;
	}

	result += TestRegisterCustomer(d, "ander2003", "Anderson", 50, 0);
	result += TestRegisterCustomer(d, "Adele", "adele", 100, 0);
	result += TestRegisterCustomer(d, "mike3002", "Mike", 200, 0);
	result += TestRegisterCustomer(d, "adrian", "Adrian", 400, 0);

	result += TestGetSumCustomerPurchase(d, &NameStartsWithA,
										 "NameStartsWithA", 450);
	result += TestGetSumCustomerPurchase(d, &IDStartsWithA,
										 "IDStartsWithA", 100);
	result += TestGetSumCustomerPurchase(d, &PurchaseLargerThan100,
										 "PurchaseLargerThan100", 600);

	result += TestUnregisterCustomerByName(d, "Adrian", 0);
	result += TestRegisterCustomer(d, "Adriano", "Ardriano", 800, 0);

	result += TestGetSumCustomerPurchase(d, &NameStartsWithA,
										 "NameStartsWithA", 850);
	result += TestGetSumCustomerPurchase(d, &IDStartsWithA,
										 "IDStartsWithA", 900);
	result += TestGetSumCustomerPurchase(d, &PurchaseLargerThan100,
										 "PurchaseLargerThan100", 1000);

	DestroyCustomerDB(d);

	printf("\nCorrectness Test 5 %s\n\n",
		   (result >= 0)? "PASSED" : "FAILED!");

	return (result >= 0)? 0 : -1;
}
/*--------------------------------------------------------------------*/
float
timedifference_msec(struct timeval* t0, struct timeval* t1)
{
    return (t1->tv_sec - t0->tv_sec) * 1000.0f
		+ (t1->tv_usec - t0->tv_usec) / 1000.0f;
}
/*--------------------------------------------------------------------*/
int
OddNumber(const char *id, const char* name, const int purchase)
{
	const char *p = name + 4;

	if ((atoi(p) % 2) == 1)
		return purchase;

	return 0;
}
/*--------------------------------------------------------------------*/
/* Performance Test */
void
PerformanceTest(int num) {

	DB_T d;
	int sum, i, res;
	char name[100];
	char id[100];
	struct timeval start, end;
	double elapsed;

	printf("---------------------------------------------------\n" \
		   "  Performance Test\n" \
		   "---------------------------------------------------\n\n");

	d = CreateCustomerDB();
	if (d == NULL) {
		printf("CreateCustomerDB() failed, cannot perform the test\n");
		return;
	}

	/*----------------------- Test 1 ----------------------*/
	printf("[Test 1] Register %d users with RegisterCustomer()\n", num);
	/* start timer */
	gettimeofday(&start, NULL);
	/* run test */
	for (i = 0; i < num; i++) {
		sprintf(name, "name%d", i);
		sprintf(id, "id%d", i);
		if (RegisterCustomer(d, id, name, 10) < 0) {
			printf("RegisterCustomer returns error\n");
			return;
		}
	}
	/* stop timer and calulate elapsed time*/
	gettimeofday(&end, NULL);
	elapsed = timedifference_msec(&start, &end);
	printf("Finished registering %d users\n", num);
	printf("[elapsed time: %f ms]\n\n", elapsed);

	/*----------------------- Test 2 ----------------------*/
	printf("[Test 2] Total sum of purchase of %d users\n"\
		   "         with GetPurchaseByName()\n", num);
	/* start timer */
	gettimeofday(&start, NULL);
	/* run test */
	sum = 0;
	for (i = 0; i < num; i++) {
		sprintf(name, "name%d", i);
		if ((res = GetPurchaseByName(d, name)) > 0)
			sum += res;
	}
	/* stop timer and calulate elapsed time*/
	gettimeofday(&end, NULL);
	elapsed = timedifference_msec(&start, &end);
	printf("Finished calculating the total sum = %d\n", sum);
	printf("[elapsed time: %f ms]\n\n", elapsed);

	/*----------------------- Test 3 ----------------------*/
	printf("[Test 3] Total sum of purchase of %d users\n"\
		   "         with GetPurchaseByID()\n", num);
	/* start timer */
	gettimeofday(&start, NULL);
	/* run test */
	sum = 0;
	for (i = 0; i < num; i++) {
		sprintf(id, "id%d", i);
		if ((res = GetPurchaseByID(d, id)) > 0)
			sum += res;
	}
	/* stop timer and calulate elapsed time*/
	gettimeofday(&end, NULL);
	elapsed = timedifference_msec(&start, &end);
	printf("Finished calculating the total sum = %d\n", sum);
	printf("[elapsed time: %f ms]\n\n", elapsed);

	/*----------------------- Test 4 ----------------------*/
	printf("[Test 4] Total sum of purchase of odd number users\n"\
		   "         with GetSumCustomerPurchase()\n");
	/* start timer */
	gettimeofday(&start, NULL);
	/* run test */
	sum = GetSumCustomerPurchase(d, OddNumber);
	/* stop timer and calulate elapsed time*/
	gettimeofday(&end, NULL);
	elapsed = timedifference_msec(&start, &end);
	printf("Finished calculating the odd number user sum = %d\n", sum);
	printf("[elapsed time: %f ms]\n\n", elapsed);

	/*----------------------- Test 5 ----------------------*/
	printf("[Test 5] Unregister all the %d users\n"\
		   "         with UnregisterCustomerByName()\n", num);
	/* start timer */
	gettimeofday(&start, NULL);
	/* run test */
	for (i = 0; i < num; i++) {
		sprintf(name, "name%d", i);
		assert(UnregisterCustomerByName(d, name) == 0);
	}
	/* stop timer and calulate elapsed time*/
	gettimeofday(&end, NULL);
	elapsed = timedifference_msec(&start, &end);
	printf("Finished unregistering %d users\n", num);
	printf("[elapsed time: %f ms]\n\n", elapsed);


	DestroyCustomerDB(d);
}

/*--------------------------------------------------------------------*/
int
main(int argc, const char *argv[])
{
	int res[5], i;

	/* ./testclient -c : run all the correctness tests */
	if (argc == 2 && strcmp("-c", argv[1]) == 0) {
		res[0] = CorrectnessTest1();
		res[1] = CorrectnessTest2();
		res[2] = CorrectnessTest3();
		res[3] = CorrectnessTest4();
		res[4] = CorrectnessTest5();

		for (i = 0; i < 5; i++)
			printf("Test %d %s\n", i + 1,
				   (res[i] == 0)? "PASSED" : "FAILED");

		return 0;
	}
	/* ./testclient -c case : run the certain correctness test*/
	else if (argc == 3 && strcmp("-c", argv[1]) == 0) {
		if (atoi(argv[2]) == 1)
			CorrectnessTest1();
		else if (atoi(argv[2]) == 2)
			CorrectnessTest2();
		else if (atoi(argv[2]) == 3)
			CorrectnessTest3();
		else if (atoi(argv[2]) == 4)
			CorrectnessTest4();
		else if (atoi(argv[2]) == 5)
			CorrectnessTest5();
		else
			goto error;
		return 0;
	}
	/* ./testclient -p num : run the performance test */
	else if (argc == 3 && strcmp("-p", argv[1]) == 0) {
		int n = atoi(argv[2]);
		if (n > 0)
			PerformanceTest(n);

		return 0;
	}

 error:
	printf("Usage:  %s -c      run all the correctness tests\n"  	\
		   "        %s -c 3    run the correctness test 3 (1~5)\n"	\
		   "        %s -p 2000 run performance test with data set"	\
		   " of 2000 users", argv[0], argv[0], argv[0]);

	return 0;
}


