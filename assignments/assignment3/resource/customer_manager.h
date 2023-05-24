#ifndef CUSTOMER_MANAGER_H
#define CUSTOMER_MANAGER_H

/**********************
 * EE209 Assignment 3 *
 **********************/
/* customer_manager.h */

/* forward type definition for DB_T */
/* "struct DB" should be defined in customer_manager1.c or
   customer_manger2.c */
typedef struct DB* DB_T;

/* function pointer type definition */
typedef int (*FUNCPTR_T)(const char* id, const char* name,
						 const int purchase);

/* create and return a db structure */
DB_T CreateCustomerDB(void);

/* destory db and its associated memory */
void DestroyCustomerDB(DB_T d);

/* register a customer with (name, id, purchase) */
int  RegisterCustomer(DB_T d, const char *id,
		      const char *name, const int purchase);

/* unregister a customer with 'id' */
int UnregisterCustomerByID(DB_T d, const char *id);

/* unregister a customer with 'name' */
int UnregisterCustomerByName(DB_T d, const char *name);

/* get the purchase amount of a user whose ID is 'id' */
int GetPurchaseByID(DB_T d, const char* id);

/* get the purchase amount of a user whose name is 'name' */
int GetPurchaseByName(DB_T d, const char* name);

/* iterate all valid user items once, evaluate fp for each valid user
   and return the sum of all fp function calls */
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp);

#endif /* end of CUSTOMER_MANAGER_H */
