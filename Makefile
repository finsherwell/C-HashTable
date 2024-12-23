# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Target executable
TARGET = $(BUILD_DIR)/main

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) -o $(TARGET)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)
