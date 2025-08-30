# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall
LDFLAGS = -lreadline

# Directory variables
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Find all .cpp source files in SRC_DIR
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Replace src/xxx.cpp → obj/xxx.o
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# Final target executable
TARGET = $(BIN_DIR)/myshell

# Phony targets
.PHONY: all clean

# Default target
all: $(TARGET)

# Link object files into final binary
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .cpp → .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)