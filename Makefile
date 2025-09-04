# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall \
    -I/opt/homebrew/opt/readline/include \
    -Ilib \
    -DREADLINE_LIBRARY
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline -lncurses

# Directory variables
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
HDR_DIR = lib

# Find all .cpp source files in SRC_DIR
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Replace src/xxx.cpp → obj/xxx.o
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# Final target executable
TARGET = $(BIN_DIR)/myshell

# Phony targets
.PHONY: all clean dirs organize checksrc run

# Default target
all: dirs organize checksrc $(TARGET)

# Move stray .cpp files (from project root) into src/
# Move stray libs (*.h, *.hpp) into lib/
organize:
	@mkdir -p $(SRC_DIR) $(HDR_DIR)
	@for f in *.cpp; do \
		if [ -f "$$f" ]; then \
			echo "📂 Moving $$f → $(SRC_DIR)/"; \
			mv "$$f" $(SRC_DIR)/; \
		fi \
	done
	@for h in *.h *.hpp; do \
		if [ -f "$$h" ]; then \
			echo "📂 Moving $$h → $(HDR_DIR)/"; \
			mv "$$h" $(HDR_DIR)/; \
		fi \
	done

# Check if we have any source files
checksrc:
	@if [ -z "$(SRC)" ]; then \
		echo "❌ No source files found in $(SRC_DIR)/"; \
		exit 1; \
	fi

# Link object files into final binary
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .cpp → .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure directories exist
dirs: | $(SRC_DIR) $(OBJ_DIR) $(BIN_DIR) $(HDR_DIR)

$(SRC_DIR) $(OBJ_DIR) $(BIN_DIR) $(HDR_DIR):
	mkdir -p $@

# Run target: build first, then run
run: $(TARGET)
	@echo "🚀 Running $(TARGET)..."
	@./$(TARGET)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)