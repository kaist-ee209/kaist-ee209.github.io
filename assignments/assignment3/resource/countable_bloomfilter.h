#ifndef COUNTABLE_BLOOMFILTER_H
#define COUNTABLE_BLOOMFILTER_H
#define BLOOMFILTER_SIZE 1024
#define NUM_HASH 3

struct BloomFilter {
  int size;
  unsigned int *arr;
  int num_hash;
};

struct BloomFilter* bloomfilter_create (unsigned int size, unsigned int num_hash);

void bloomfilter_destroy (struct BloomFilter *bf);

void bloomfilter_add (struct BloomFilter *bf, const char *key);

int bloomfilter_remove (struct BloomFilter *bf, const char *key);

int bloomfilter_check (struct BloomFilter *bf, const char *key);

#endif
