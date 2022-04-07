#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "murmurhash.h"
#include "countable_bloomfilter.h"

#define ___BLANK___ 0

/**
 * Structure of struct BloomFilter
 *
 * arr : must point to the first element of Bloomfilter Array
 * size : must be equal to the length of Bloomfilter Array
 * 
 * +-----------------------------------+
 * |         struct BloomFilter        |
 * +-----------+-----------------------+
 * |    size   |          *arr         |
 * +--+--+--+--+--+--+--+--+--+--+--+--+
 * |  |  |  |  |  |  |  |  |  |  |  |  |
 * +--+--+--+--+--+--+--+--+--+--+--+--+
 *
 * +-----------------------------------+
 * |               Array               |
 * +---+---+---+---+---+---+-----+-----+
 * | 0 | 1 | 2 | 3 | 4 | 5 | ... | N-1 |
 * +---+---+---+---+---+---+-----+-----+
 * |   |   |   |   |   |   |     |     |
 * +---+---+---+---+---+---+-----+-----+
 *
 * */

/**
 * Allocate a bloom filter whose array size is 'size'. Returns the pointer of new BloomFilter structure.
 */
struct BloomFilter* bloomfilter_create (unsigned int size, unsigned num_hash) {
  struct BloomFilter* bf;
  char *array;
  if(!size) {
    fprintf(stderr, "Size of bloom filter must be larger than 0!\n");
    return NULL;
  }

  /* Fill in the Blank */
  /* ============================== */
  bf = calloc(1,    ___BLANK___   );
  array = calloc(1,    ___BLANK___   );
  assert(bf);
  assert(array);
 
  bf->arr = (unsigned int *) (     ___BLANK___    );
  bf->size = size;
  bf->num_hash = num_hash;
  return (     ___BLANK___     );
  /* ============================== */
}

/**
 * Destroy a bloom filter.
 */
void bloomfilter_destroy (struct BloomFilter *bf) {
  assert(bf);
  free(bf->arr);
  free(bf);
}

/**
 * Add a key into the bloom filter.
 */
void bloomfilter_add (struct BloomFilter *bf, const char *key) {
  assert(bf && bf->arr);
  if (!key) {
    fprintf(stderr, "String must not be NULL!\n");
    return;
  }

  uint32_t hash1 = murmurhash(key, (uint32_t) strlen(key), 100);
  uint32_t hash2 = murmurhash(key, (uint32_t) strlen(key), 200);
  // uint32_t hash3 = murmurhash(key, (uint32_t) strlen(key), 300);
  /* Fill in the Blank */
  /* Hint: bf->arr[ .... ] ... */  
  /* ============================== */
  
  
  
  /* ============================== */

}

/**
 * Remove a key from the bloom filter. Returns 0 if failed to remove. Returns 1 if successfully removed.
 */
int bloomfilter_remove (struct BloomFilter *bf, const char *key) {
  assert(bf && bf->arr);
  if(!bloomfilter_check(bf, key)) {
    return 0;
  }

  uint32_t hash1 = murmurhash(key, (uint32_t) strlen(key), 100);
  uint32_t hash2 = murmurhash(key, (uint32_t) strlen(key), 200);
  // uint32_t hash3 = murmurhash(key, (uint32_t) strlen(key), 300);
  /* Fill in the Blank */
  /* Hint: bf->arr[ .... ] ... */  
  /* ============================== */
  
  
  
  /* ============================== */

  return 1; 
}

/**
 * Returns the possible number of keys inside the bloom filter. If not exist, return 0.
 */
int bloomfilter_check (struct BloomFilter *bf, const char *key) {
  assert(bf && bf->arr);
  
  uint32_t hash1 = murmurhash(key, (uint32_t) strlen(key), 100);
  uint32_t hash2 = murmurhash(key, (uint32_t) strlen(key), 200);
  // uint32_t hash3 = murmurhash(key, (uint32_t) strlen(key), 200);
  /* Fill in the Blank */
  /* ============================== */
 
 uint32_t num1 = ___BLANK___;
 uint32_t num2 = ___BLANK___;
//  uint32_t num3 = ___BLANK___;
 uint32_t total_num = ___BLANK___;
  
  return ( total_num );
  /* ============================== */

}
