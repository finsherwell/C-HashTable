#ifndef HASH_T_HEADER
#define HASH_T_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_SIZE_ENTRY 256
#define INITIAL_TABLE_SIZE 8
#define MAX_TABLE_SIZE 128
#define MAX_LOAD_FACTOR 0.75

typedef struct ht_entry ht_entry;

struct ht_entry {
    char* key;
    void* value;
    ht_entry* next; // External Chain
};

int fnv1a_hash(char* key);
void init_hash_table();
bool resize_table();
bool ht_insert(ht_entry* entry);
ht_entry* ht_search(char* key);
ht_entry* ht_delete(char* key);
void print_table();

#endif // HASH_T_HEADER