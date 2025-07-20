# Compiler and Flags

CC = gcc

LDFLAGS =  # Add linker flags if needed

# Source and Object Files
SRC_DIR = 
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = test/lexer_tc

SRC = $(wildcard *.c)  # Finds all .c files in src/
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))  # Converts .c to .o
BIN = stage1exe  # Output executable


# Create binary executable
$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean
