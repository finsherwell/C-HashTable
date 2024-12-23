#ifndef HASH_T_HEADER
#define HASH_T_HEADER

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// The hash table structure definition (forward declaration)
typedef struct ht ht;

// Create a new hash table and return a pointer to it
ht* ht_create(void);

// Destroy the hash table and free allocated memory
void ht_destroy(ht* table);

// Retrieve the value associated with a key in the hash table
void* ht_get(ht* table, const char* key);

// Set (insert or update) a key-value pair in the hash table
const char* ht_set(ht* table, const char* key, void* value);

// Return the number of elements in the hash table
size_t ht_length(ht* table);

// Iterator structure for looping through the hash table
typedef struct {
    const char* key; // Current key in the iteration
    void* value; // Current value in the iteration

    ht* _table; // Reference to the hash table
    size_t _index; // Current index in the iteration
} ht_it;

// Return an iterator to start iterating through the hash table
ht_it ht_iterator(ht* table);

// Move to the next item in the hash table and update the iterator
bool ht_next(ht_it* it);

#endif // HASH_T_HEADER