#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
  Basic hash table key/value pair
 ****/
typedef struct Pair
{
  char *key; // i guess both of these are a complex data type, like a string
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable
{
  // capacity is the number of pairs we can have, or the number of key:value pairs we can... store?
  int capacity;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL)
  {
    //free(pair->key);
    //free(pair->value);
    free(pair);
    //pair = NULL;
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable)); //do we multiply by capacity?
  // so, i need to use Calloc here to intialize everything to NULL
  //void* calloc (size_t num, size_t size);
  ht->capacity = capacity;
  ht->storage = calloc(ht->capacity, sizeof(Pair *));

  return ht;
}
// BasicHashTable *create_hash_table(int capacity)
// {
//   if (capacity < 1)
//   {
//     return NULL;
//   }

//   BasicHashTable *ht = (BasicHashTable *)malloc(sizeof(BasicHashTable));
//   ht->capacity = capacity;
//   ht->storage = (Pair **)calloc(capacity, sizeof(Pair *));

//   return ht;
// }

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{

  // the reason we hash this pair is so we get an index value that is lower than the capacity
  unsigned int hashed_index = hash(key, ht->capacity);
  if (ht->storage[hashed_index] == NULL)
  {
    // if the bucket is empty, let's intialize a pair and put it into it
    Pair *pair = create_pair(key, value);
    ht->storage[hashed_index] = pair;
  }
  else if (strcmp(ht->storage[hashed_index]->key, key) == 0)
  {
    // if the key matches the hashed key, update the value of said key

    ht->storage[hashed_index]->value = value;
  }
  else if (strcmp(ht->storage[hashed_index]->key, key) != 0)
  {
    // this is a collision, we somehow ended up with two different keys with the same hash
    printf("This bucket is already filled, we will overwrite the previous key and value \n");
    ht->storage[hashed_index]->value = value;
  }
}
// void hash_table_insert(BasicHashTable *ht, char *key, char *value)
// {
//   // Note, since we are only writing a resize function in hashtables.c
//   //    I'm NOT going to conditionally check if the ht is full and if so resize it.

//   // hash the key to get an array index
//   unsigned int index = hash(key, ht->capacity);

//   // check if the bucket at that index is occupied
//   Pair *curr_pair = ht->storage[index];

//   // If you do, overwrite that value
//   if (curr_pair != NULL)
//   {
//     printf("key indexed into an occupied slot, key and value will be overwritten...\n");
//     curr_pair->key = key;
//     curr_pair->value = value;
//   }
//   // If not, create a new pair and add it to the LinkedList
//   else
//   {
//     // if it's not occupied, add a new Pair to the bucket
//     Pair *new_pair = create_pair(key, value);
//     ht->storage[index] = new_pair;
//   }
// }

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  // ohh, we are just removing the pair
  unsigned int hashed_index = hash(key, ht->capacity);
  // wait, we are trying to nuke whatever is in the bucket, that pair in that bucket. so we better
  // really make sure that the keys match, or we will kill a pair that doesn't need to die
  if (ht->storage[hashed_index] != NULL && strcmp(ht->storage[hashed_index]->key, key) == 0)
  {

    destroy_pair(ht->storage[hashed_index]);
    ht->storage[hashed_index] = NULL;
    return;
  }

  printf("Yo, the key you gave me doesn't exist. Check yourself before you rekt yourself. \n");
  // exit(1); explictely, i want to know what to return in a void function, and can i just use exit(1)
}

// void hash_table_remove(BasicHashTable *ht, char *key)
// {
//   // hash the key to get an array index
//   unsigned int index = hash(key, ht->capacity);

//   // if key_found, destroy pair(free's the address and pointer), and set element of array to null
//   if (ht->storage[index] != NULL)
//   {
//     destroy_pair(ht->storage[index]);
//     ht->storage[index] = NULL;
//   }
// }

char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int hashed_index = hash(key, ht->capacity);

  if (ht->storage[hashed_index] != NULL && strcmp(ht->storage[hashed_index]->key, key) == 0)
  {
    // OMG DOT NOTATION IS A THING
    return (*ht->storage[hashed_index]).value;
  }

  printf("Yo, the key you gave me doesn't exist. Check yourself before you rekt yourself. \n");

  return NULL;
}

void destroy_hash_table(BasicHashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      hash_table_remove(ht, ht->storage[i]->key);
    }
  }
  free(ht->storage);
  free(ht);
  ht->storage = NULL;
  ht = NULL;
}

#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL)
  {
    printf("...gone tomorrow. (success)\n");
  }
  else
  {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
