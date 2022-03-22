#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/**********************
 * EE209 Assignment 3 *
 **********************/
/* hash_table.h */

/* forward type definition for HashTable */
/* "struct HashTable" should be defined in hash_table1.c or
   hash_table2.c */
typedef struct HashTable* HashTable_T;

/* create and return a hash table */
HashTable_T HashTable_new(void);

/* destory the hash table and its associated memory */
void HashTable_free(HashTable_T ht);

/* get the number of key-value pairs in the hash table */
int HashTable_getLength(HashTable_T ht);

/* put a key-value pair into the hash table if the key does not exists */
int HashTable_put(HashTable_T ht, const char *key, const void *value);

/* replace a value of a given key in the hash table */
void *HashTable_replace(HashTable_T ht, const char *key,
   const void *newValue);

/* check whether the key exists in the hash table */
int HashTable_contains(HashTable_T ht, const char *key);

/* returns a value of given key in the hash table */
void *HashTable_get(HashTable_T ht, const char *key);

/* remove a key-value pair from the hash table */
void *HashTable_remove(HashTable_T ht, const char *key);

/* iterates all entries in the hash table and call func */
void HashTable_map(HashTable_T ht,
   void (*func)(const char *key, void *value, void *extra),
   const void *extra);

/* returns the number of buckets of the hash table */
void HashTable_getBucketCount(HashTable_T ht);

/* returns the number of entries in the nth bucket */
void HashTable_getBucketSize(HashTable_T ht, size_t n);

#endif /* end of HASH_TABLE_H */
