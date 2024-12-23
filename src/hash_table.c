#include "hash_table.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// MurmurHash3 Implementation (Hash function for strings)
uint64_t murmurhash3_64(const void *key, size_t len, uint64_t seed) {
    const uint8_t *data = (const uint8_t*)key;  // Convert key to bytes
    uint64_t h = seed ^ len;  // Initialize hash with the seed and length
    const uint64_t m = 0xc6a4a7935bd1e995;  // Constant multiplier
    const int r = 47;  // Shift amount for mixing

    // Process the key in 8-byte chunks
    while (len >= 8) {
        uint64_t k = *(uint64_t*)data;  // Load 8 bytes
        k *= m;
        k ^= k >> r;  // Mix bits
        k *= m;
        
        h ^= k;
        h *= m;
        
        data += 8;
        len -= 8;
    }
    
    // Process remaining 4 bytes if any
    if (len >= 4) {
        uint32_t k = *(uint32_t*)data;
        k *= m;
        k ^= k >> r;
        k *= m;
        
        h ^= k;
        data += 4;
        len -= 4;
    }
    
    // Process the last byte if necessary
    if (len) {
        uint8_t k = *data;
        h ^= k;
        h *= m;
    }
    
    // Finalization step
    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    
    return h;
}

// Hash Table Capacity
# define INITIAL_CAPACITY 16  // Initial size of the hash table (number of slots)

// Structure for each entry in the hash table (key-value pair)
typedef struct {
    const char* key; // The key is a string (or NULL if empty)
    void* value; // The value can be any type of data (void pointer)
} ht_entry;

// Hash table structure that holds entries and table size information
struct ht {
    ht_entry* entries; // Array of hash table entries
    size_t capacity; // Number of slots in the table
    size_t length; // Number of items currently stored in the table
};

// Second hash function for double hashing (step size calculation)
static uint64_t second_hash(uint64_t hash, size_t capacity) {
    return 1 + (hash % (capacity - 1)); // Ensure step size is non-zero
}

// Double hash probing (used for collision resolution)
static size_t double_hash_probe(uint64_t hash, size_t capacity, size_t i) {
    uint64_t h2 = second_hash(hash, capacity); // Get the secondary hash (step size)
    return (hash + i * h2) % capacity; // Return the new index using the secondary hash
}

// Hash Table Operations (functions that manipulate the hash table)
ht* ht_create(void) {
    ht* table = malloc(sizeof(ht));  // Allocate memory for the hash table structure
    if (table == NULL) {
        return NULL;  // Return NULL if memory allocation fails
    }
    table->length = 0;  // Initialize length to 0 (empty table)
    table->capacity = INITIAL_CAPACITY;  // Set initial capacity

    table->entries = calloc(table->capacity, sizeof(ht_entry));  // Allocate memory for entries (initialized to NULL)
    if (table->entries == NULL) {
        free(table);  // Free the table structure before returning NULL
        return NULL;
    }

    return table;
}

// Function to destroy the hash table and free all allocated memory
void ht_destroy(ht* table) {
    for (size_t i = 0; i < table->capacity; i++) {
        free((void*)table->entries[i].key);  // Free memory allocated for keys
    }
    free(table->entries);  // Free the entries array
    free(table);  // Free the table structure
}

// Function to retrieve a value associated with a key from the table
void* ht_get(ht* table, const char* key) {
    uint64_t hash = murmurhash3_64(key, strlen(key), 42);  // Generate hash for the key using MurmurHash3
    size_t index = hash % table->capacity;  // Calculate the initial index (modulo capacity)
    size_t i = 0;  // Initialize probe count

    // Probe for the key using double hashing (resolve collisions)
    while (table->entries[index].key != NULL) {
        if (strcmp(key, table->entries[index].key) == 0) {
            return table->entries[index].value;  // Return value if key is found
        }

        i++;  // Increment probe count
        index = double_hash_probe(hash, table->capacity, i);  // Calculate next index using double hashing
    }

    return NULL;  // Return NULL if key is not found
}

// Internal function to set a key-value entry into the table (handles collisions)
static const char* ht_set_entry(ht_entry* entries, size_t capacity, const char* key, void* value, size_t* plength) {
    uint64_t hash = murmurhash3_64(key, strlen(key), 42);  // Generate hash for the key
    size_t index = hash % capacity;  // Calculate the initial index
    size_t i = 0;  // Initialize probe count

    // Probe for an empty slot using double hashing (resolve collisions)
    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            entries[index].value = value;  // Update value if key is already in table
            return entries[index].key;  // Return existing key
        }

        i++;  // Increment probe count
        index = double_hash_probe(hash, capacity, i);  // Calculate next index
    }

    // Allocate memory for the new key and insert the new entry
    if (plength != NULL) {
        key = strdup(key);  // Duplicate the key (allocate new memory)
        if (key == NULL) {
            return NULL;  // Return NULL if memory allocation fails
        }
        (*plength)++;  // Increment the length of the table
    }

    entries[index].key = (char*)key;  // Set the key
    entries[index].value = value;  // Set the value

    return key;  // Return the key
}

// Function to expand the hash table by doubling its capacity
static bool ht_expand(ht* table) {
    size_t new_capacity = table->capacity * 2;  // Double the capacity
    ht_entry* new_entries = calloc(new_capacity, sizeof(ht_entry));  // Allocate new entries array
    if (new_entries == NULL) {
        return false;  // Return false if memory allocation fails
    }

    // Rehash existing entries and move them to the new array
    for (size_t i = 0; i < table->capacity; i++) {
        ht_entry entry = table->entries[i];
        if (entry.key != NULL) {
            ht_set_entry(new_entries, new_capacity, entry.key, entry.value, NULL);  // Re-insert entry into new array
        }
    }

    // Free the old entries array and update the table with the new array and new capacity
    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

// Function to set a key-value pair in the hash table
const char* ht_set(ht* table, const char* key, void* value) {
    assert(value != NULL);  // Ensure value is not NULL
    if (value == NULL) {
        return NULL;  // Return NULL if value is NULL
    }

    // If the table is half full, expand it
    if (table->length >= table->capacity / 2) {
        if (!ht_expand(table)) {
            return NULL;  // Return NULL if expansion fails
        }
    }

    // Insert the entry into the table and return the key
    return ht_set_entry(table->entries, table->capacity, key, value, &table->length);
}

// Function to get the number of elements in the hash table
size_t ht_length(ht* table) {
    return table->length;  // Return the current length of the table
}

// Iterator to loop through the hash table
ht_it ht_iterator(ht* table) {
    ht_it it;
    it._table = table;  // Set reference to the hash table
    it._index = 0;  // Start iterator at the beginning of the table
    return it;
}

// Function to move the iterator to the next item in the hash table
bool ht_next(ht_it* it) {
    ht* table = it->_table;
    while (it->_index < table->capacity) {
        size_t i = it->_index++;  // Increment the index
        if (table->entries[i].key != NULL) {
            it->key = table->entries[i].key;  // Set current key
            it->value = table->entries[i].value;  // Set current value
            return true;  // Return true if item is found
        }
    }
    return false;  // Return false if no more items
}