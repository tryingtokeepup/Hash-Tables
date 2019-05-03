#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable
{
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  (*pair).next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
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

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(ht->capacity, sizeof(LinkedPair *));
  return ht;
}

void hash_table_insert(HashTable *ht, char *key, char *value)
{
  //1) Take key, hash this number to get a num less than capacity.
  unsigned int hashed_index = hash(key, ht->capacity);

  //2) Take this num, use this as the index number/bucket number that we will store the pair.
  if (ht->storage[hashed_index] == NULL)
  {
    // 2.a) If there's nothing in this bucket, put the Pair into the bucket.
    LinkedPair *pair = create_pair(key, value);
    ht->storage[hashed_index] = pair;
    return;
  }
  else
  {
    // 2.b) If there is something in there, I need to iterate through all the nodes in the bucket.
    // 2.c) Okay, so I need to make copies of the pairs inside the bucket.
    LinkedPair *first_node = ht->storage[hashed_index]; // the index number always points to the first node
    LinkedPair *next_node = (*first_node).next;
    // linked lists are not arrays -> you have to traverse all the pointers
    //while ( the next node does not point to a NULL AND currentnode.key is not the key we are looking for )
    // keep going through the while loop until we hit the key
    while (next_node != NULL && strcmp((*first_node).key, key) != 0)
    {

      first_node = next_node;
      next_node = (*first_node).next;
    }
    // if the key is found, then we do what we always do: let's update the value!
    // learn more about direct accessing of addresses
    if (strcmp((*first_node).key, key) == 0)
    {

      (*first_node).value = value;
    }
    // else, if the key is not found, i need to create that key value pair and append to the end of the first_node!
    else
    {
      LinkedPair *new_pair = create_pair(key, value);
      (*first_node).next = new_pair;
    }
  }
  // I need to traverse ... ? the bucket? and make sure all keys don't match the new pair key.
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
// void hash_table_remove(HashTable *ht, char *key)
// {
//   // 1) We need to remove the key value pair at the hashed_index, so first we need to get the hash
//   unsigned int hashed_index = hash(key, ht->capacity);
//   // 2) now, once we have the index, go through the linked listand find the node with the same key. once we do, remove it.
//   LinkedPair *current_node = ht->storage[hashed_index]; // the index number always points to the first node
//   LinkedPair *previous_node = NULL;
//   while (current_node != NULL && (*current_node).key != key)
//   {

//     previous_node = current_node;
//     current_node = (*current_node).next;
//   }
//   // if we exited the above while loop and the current_node is  NULL, it must mean we didn't find the key
//   if (current_node == NULL)
//   {
//     printf("Failed to find the key, check again please. %s\n", key);
//   }
//   else
//   {
//     // if this is the first node in the list, we should check for that by checking its previous_node value.
//     if (previous_node == NULL)
//     {
//       // take whatever is the next node after current_node, and put that value into the space that is going to
//       // be vacated by the current_node after it is removed
//       ht->storage[hashed_index] = (*current_node).next;
//     }
//     else
//     {
//       // tricky bugger, but you need to connect the empty gap between the prev node and the next node by
//       // bridging the two this way
//       (*previous_node).next = (*current_node).next;
//     }
//     printf("successfully destroyed pair\n");
//     destroy_pair(current_node);
//     (*current_node).key = NULL;
//     (*current_node).value = NULL;
//   }
// }
void hash_table_remove(HashTable *ht, char *key)
{
  // hash the key to get an array index
  unsigned int index = hash(key, ht->capacity);

  // If element of array index is NULL, return null
  //    exit function
  if (ht->storage[index] == NULL)
  {
    printf("Key \"%s\" was not found in the hash table\n", key);
    return;
  }

  // If the head node in the link list is a match...
  //    make the array index equal next node as the
  //    new head (or NULL)
  //    exit function
  LinkedPair *curr_node = ht->storage[index];
  if (strcmp(curr_node->key, ht->storage[index]->key) == 0)
  {
    ht->storage[index] = curr_node->next;
    destroy_pair(curr_node);
    return;
  }

  // Search through the link list for the node key matching given key...
  LinkedPair *prev_node = curr_node;
  curr_node = prev_node->next;

  while (curr_node != NULL && strcmp(curr_node->key, key) != 0)
  {
    prev_node = curr_node;
    curr_node = prev_node->next;
  }

  // If key was found in the current node of the link list
  if (strcmp(curr_node->key, key) == 0)
  {
    // remove the current node from the link list
    prev_node->next = curr_node->next;
    // free the memory of the current node
    destroy_pair(curr_node);
    curr_node = NULL;
  }
  else
  {
    printf("Key \"%s\" was not found in the Link List\n", key);
  }
}
/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  // take the key, hash it, then take that array, traverse the linked_list for that key
  unsigned int hashed_index = hash(key, ht->capacity);
  LinkedPair *current_node = ht->storage[hashed_index]; // the index number always points to the first node

  while (current_node != NULL) // if we find the key OR the next_node is NULL, we exit
  {

    if (strcmp(current_node->key, key) == 0)
    {
      return current_node->value;
    }
    current_node = (*current_node).next;
  }
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      LinkedPair *current_node = ht->storage[i]; // the index number always points to the first node
      LinkedPair *next_node = (*current_node).next;
      // loop through linked_list, destroying and freeing each pair
      while (next_node != NULL)
      {
        destroy_pair(current_node);
        current_node = next_node;
        next_node = (*current_node).next;
      }
      // free that last pair
      destroy_pair(current_node);
    }
  }
  free(ht->storage);
  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *resized_ht = create_hash_table(2 * ht->capacity);
  LinkedPair *current_pair;

  for (int i = 0; i < ht->capacity; i++)
  {

    current_pair = ht->storage[i];
    while (current_pair != NULL)
    {
      hash_table_insert(resized_ht, (*current_pair).key, (*current_pair).value);
      current_pair = (*current_pair).next;
    }
  }

  return resized_ht;
}

#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);
  printf("sucessfully destroyed the table.\n");
  return 0;
}
#endif
