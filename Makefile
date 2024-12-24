# Define Variables
CC = gcc
CFLAGS = -Wall -Wextra

# Define Directories
SRC_DIR = src
BUILD_DIR = build

# Create Main Object File (Test)
OBJ_FILE = $(BUILD_DIR)/main.o

# Create Test Executable File
EXEC = $(BUILD_DIR)/test_hash_table

# Define Target to Compile All
all: $(BUILD_DIR) $(EXEC)

# Rule to Create Executable
$(EXEC): $(OBJ_FILE) $(BUILD_DIR)/hash_table.o
	$(CC) $(OBJ_FILE) $(BUILD_DIR)/hash_table.o -o $(EXEC)

# Rule to Create Main Object File (Test)
$(OBJ_FILE): $(SRC_DIR)/main.c $(SRC_DIR)/hash_table.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(OBJ_FILE)

# Rule to Compile Source Files (Excluding Main)
$(BUILD_DIR)/hash_table.o: $(SRC_DIR)/hash_table.c $(SRC_DIR)/hash_table.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/hash_table.c -o $(BUILD_DIR)/hash_table.o

# Clean Generated Files
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/test_hash_table