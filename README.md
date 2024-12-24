# C-HashTable
This project demonstrates a basic hash table implementation in C with support for operations such as insertion, deletion, and search. The code uses open addressing with external chaining for collision handling and supports dynamic resizing based on the load factor.

## Features
- **Custom Hashing:** Implements the FNV-1a hashing algorithm for hashing string keys.
- **Dynamic Resizing:** The hash table automatically doubles its size when the load factor exceeds 75%.
- **Collision Handling:** Uses external chaining with linked lists to handle collisions.
- **Operations Supported:**
  - Insert
  - Search
  - Delete
  - Print

## Files
```src/main.c```
Where tests can be written to verify the functionality of the hash table. Tests could include:
- Insertion of key-value pairs
- Searching for existing and non-existing keys
- Deletion of keys
- Printing the hash table

```hash_table.c```
Implements the core functionality of the hash table:
- Hashing with the FNV-1a algorithm
- Insert, search, and delete operations
- Table resizing for performance

```hash_table.h```
Header file that defines the structures and functions for the hash table.

```build/info.txt```
A placeholder file to ensure the build folder is committed to version control. This directory stores compiled object files and the final executable.

## Build Automation
The building of this project has been automated using Makefile for ease of use.

### Makefile Overview
The Makefile automates the compilation process:
- ```make```: Builds the ```test_hash_table``` executable.
- ```make clean```: Removes object files and the executable, except ```info.txt```.
- ```./build/test_hash_table```: Runs the executable so you can see output.

### Example Makefile Usage
- Build the project:
```make```
- Run the executable:
```./build/test_hash_table```
- Clean the build folder:
```make clean```

## Code Overview
### Core Functions
1. **Initialisation:**
```c
void init_hash_table();
```
Allocates memory for the hash table and initialises all entries to ```NULL```.

2. **Insertion:**
```c
bool ht_insert(ht_entry* entry);
```
Inserts a key-value pair into the hash table. Resizes the table if the load factor exceeds 0.75.

3. **Search:**
```c
ht_entry* ht_search(char* key);
```
Searches for a key in the hash table and returns the associated value, or ```NULL``` if not found.

4. **Deletion:**
```c
ht_entry* ht_delete(char* key);
```
Removes a key-value pair from the hash table and returns the deleted entry.

5. **Print:**
```c
void print_table();
```
Prints the current contents of the hash table for debugging purposes.

### Hashing
Uses the ***FNV-1a hashing algorithm***, which is efficient and has a low collision rate for string keys:
```c
int fnv1a_hash(char* key);
```

### Resizing
The hash table doubles its size dynamically when the load factor exceeds 0.75:
```c
bool resize_table();
```

## Example Output
Running the test file in its current state would produce:
```
Initializing hash table...
Start:
0: ---
1: ---
2: ---
3: ---
4: ---
5: ---
6: ---
7: ---
End
```
In order to test the hash table for yourself, be sure to add tests which can explore each operation.

## Future Enhancements
There are many enhancements I could make to this hash table to improve its functionality, so I will list a few below:
- **Double Hashing:** - This is an open addressing technique which uses a second hash function to resolve collisions by calculating step sizes for probing.
- **Resizing Down:** - Currently, the hash table grows when the load factor exceeds a threshold. I could add support for shrinking the table when the load factor drops below a certain threshold.
- **Thread-Safety** - I could make the hash table thread-safe to allow concurrent access.
- **Persistent Storage** - Allowing the hash table to save its state to a file and reload it later.