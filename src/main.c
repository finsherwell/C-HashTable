#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

int main() {
    // Create a new hash table
    ht* table = ht_create();
    if (table == NULL) {
        fprintf(stderr, "Error: Unable to create hash table\n");
        return 1;
    }

    // Test inserting key-value pairs
    printf("Inserting key-value pairs:\n");
    ht_set(table, "name", "Alice");
    ht_set(table, "age", "30");
    ht_set(table, "city", "New York");

    // Test retrieving values
    printf("\nRetrieving values by key:\n");
    char* name = ht_get(table, "name");
    char* age = ht_get(table, "age");
    char* city = ht_get(table, "city");

    printf("name: %s\n", name ? name : "Not found");
    printf("age: %s\n", age ? age : "Not found");
    printf("city: %s\n", city ? city : "Not found");

    // Test handling non-existent key
    char* country = ht_get(table, "country");
    printf("\ncountry: %s\n", country ? country : "Not found");

    // Test updating an existing key
    printf("\nUpdating 'age' key:\n");
    ht_set(table, "age", "31");

    age = ht_get(table, "age");
    printf("Updated age: %s\n", age ? age : "Not found");

    // Iterate over the hash table
    printf("\nIterating over the hash table:\n");
    ht_it it = ht_iterator(table);
    while (ht_next(&it)) {
        printf("Key: %s, Value: %s\n", it.key, (char*)it.value);
    }

    // Test hash table length
    size_t length = ht_length(table);
    printf("\nHash table length: %zu\n", length);

    // Destroy the hash table and free memory
    ht_destroy(table);
    printf("Hash table destroyed successfully.\n");

    return 0;
}