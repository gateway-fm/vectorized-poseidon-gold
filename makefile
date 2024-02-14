# Directories
SRC_DIR=./src
CMD_DIR=./cmd
BUILD_DIR=./build

CMD_C_DIR=$(CMD_DIR)/c
BUILD_C_DIR=$(BUILD_DIR)/c
BIN_C_DIR=$(BUILD_C_DIR)

CMD_GO_DIR=$(CMD_DIR)/go
BUILD_GO_DIR=$(BUILD_DIR)/go

# Define the compiler
CC=gcc

# Define any compile-time flags
CFLAGS=-Wall -g -MMD -MP -O2 -march=native -std=c17

# Define any directories containing header files
INCLUDES=

# Define library paths in addition to /usr/lib
LDFLAGS=

# Define any libraries to link into executable
LIBS=

# Define the C source files
MAIN_SRC=$(shell find $(CMD_C_DIR) -name '*.c')
SRCS=$(shell find $(SRC_DIR) -name '*.c')

# Define the C object files
MAIN_OBJ=$(patsubst $(CMD_C_DIR)/%.c, $(BUILD_C_DIR)/%.o, $(MAIN_SRC))
OBJS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_C_DIR)/%.o, $(SRCS))

# Dependency files
DEPS=$(OBJS:.o=.d)

# Define the executable file
EXECUTABLE=$(BIN_C_DIR)/main

.PHONY: clean

c: $(BUILD_C_DIR) $(EXECUTABLE)
	@echo  Compiling $(EXECUTABLE)

$(BUILD_C_DIR):
	mkdir -p $(BUILD_C_DIR)

$(EXECUTABLE): $(OBJS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LIBS)

$(BUILD_C_DIR)/main.o: $(CMD_C_DIR)/main.c
	@mkdir -p $(dir $@) # Ensure the directory for the .o file exists
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_C_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@) # Ensure the directory for the .o file exists
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Include the dependency files
-include $(DEPS)

go:
	@go build -mod=readonly -o $(BUILD_GO_DIR)/main -ldflags="-s -w" $(CMD_GO_DIR)/main.go

clean:
	rm -rf $(BUILD_DIR)
