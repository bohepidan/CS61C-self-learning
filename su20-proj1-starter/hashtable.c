#include "hashtable.h"
#include <stdlib.h>

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *))
{
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  newTable->size = size;
  newTable->data = malloc(sizeof(struct HashBucket *) * size);
  for (i = 0; i < size; ++i)
  {
    newTable->data[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/*
 * This inserts a key/data pair into a hash table.  To use this
 * to store strings, simply cast the char * to a void * (e.g., to store
 * the string referred to by the declaration char *string, you would
 * call insertData(someHashTable, (void *) string, (void *) string).
 * Because we only need a set data structure for this spell checker,
 * we can use the string as both the key and data.
 */
void insertData(HashTable *table, void *key, void *data)
{
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket location with table->hashFunction.
  unsigned int location;
  location = table->hashFunction(key);
  // 2. Allocate a new hash bucket struct.
  struct HashBucket *newBucket = malloc(sizeof(struct HashBucket));
  newBucket->key = key;
  newBucket->data = data;
  newBucket->next = NULL;
  // 3. Append to the linked list or create it if it does not yet exist.
  if (table->data[location] == NULL)
  {
    table->data[location] = newBucket;
  }
  else
  {
    struct HashBucket *cur = table->data[location];
    while (cur->next)
    {
      cur = cur->next;
    }
    cur->next = newBucket;
  }
}

/*
 * This returns the corresponding data for a given key.
 * It returns NULL if the key is not found.
 */
void *findData(HashTable *table, void *key)
{
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket with table->hashFunction.
  unsigned int location = table->hashFunction(key);
  // 2. Walk the linked list and check for equality with table->equalFunction.
  for (struct HashBucket *cur = table->data[location]; cur; cur = cur->next)
  {
    if (table->equalFunction(cur->key, key))
    {
      return cur->data;
    }
  }
  return NULL;
}
