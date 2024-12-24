#include "hash_table.h"

ht_entry** hash_table;
int TABLE_SIZE = INITIAL_TABLE_SIZE;
int entry_count = 0;

// FNV-1a Hashing Function
int fnv1a_hash(char* key) {
    int hash = 2166136261U;  // FNV-1a offset basis
    while (*key) {
        hash ^= (char)(*key++);
        hash *= 16777619U;  // FNV-1a prime
    }
    return hash;
}

// Hash Function (Simplified Name)
int hash(char* key) {
    return fnv1a_hash(key);
}

// Initiate Hash Table
void init_hash_table() {
    hash_table = (ht_entry**)malloc(INITIAL_TABLE_SIZE * sizeof(ht_entry*));
    if (hash_table == NULL) {
        printf("Error: Could not allocate memory for hash table\n");
        exit(1);
    }
    for (int i = 0; i < INITIAL_TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }
}

// Resizes Table (Double)
bool resize_table() {
    int old_size = TABLE_SIZE;
    ht_entry** old_table = hash_table;

    // Double the table size
    TABLE_SIZE *= 2;
    hash_table = (ht_entry**)malloc(TABLE_SIZE * sizeof(ht_entry*));
    if (!hash_table) {
        printf("Error: Could not allocate memory for resized hash table!\n");
        return false;
    }
    for (int i = 0; i < old_size; i++) {
        ht_entry* entry = old_table[i];
        while (entry) {
            ht_entry* next = entry->next;
            int index = hash(entry->key) % TABLE_SIZE;
            entry->next = hash_table[index];
            hash_table[index] = entry;
            entry = next;
        }
    }
    free(old_table);
    return true;
}

// Insert Entry
bool ht_insert(ht_entry* entry) {
    if (!entry) return false;
    if (entry_count / (float)TABLE_SIZE >= MAX_LOAD_FACTOR) {
        if (!resize_table()) {
            return false;
        }
    }
    int index = hash(entry->key) % TABLE_SIZE;
    entry->next = hash_table[index];
    hash_table[index] = entry;
    entry_count++;
    return true;
}

// Search Item from Key
ht_entry* ht_search(char* key) {
    int index = hash(key) % TABLE_SIZE;
    ht_entry* entry = hash_table[index];
    while (entry) {
        if (strncmp(entry->key, key, MAX_SIZE_ENTRY) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

// Delete Item from Key
ht_entry* ht_delete(char* key) {
    int index = hash(key) % TABLE_SIZE;
    ht_entry* entry = hash_table[index];
    ht_entry* prev = NULL;
    while (entry) {
        if (strncmp(entry->key, key, MAX_SIZE_ENTRY) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                hash_table[index] = entry->next;
            }
            entry_count--;
            return entry;
        }
        prev = entry;
        entry = entry->next;
    }
    return NULL;
}

// Print Table
void print_table() {
    printf("Start:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i] == NULL) {
            printf("%u: ---\n", i);
        } else {
            ht_entry* tmp = hash_table[i];
            printf("%u: ", i);
            while (tmp) {
                printf("%s -> ", tmp->key);
                tmp = tmp->next;
            }
            printf("NULL\n");
        }
    }
    printf("End\n");
}