# Define variables
CC = g++
CFLAGS = -Wall -Wextra -pedantic -Iinclude -std=c++17 -lboost_serialization -g
SRC_DIR = src
HELPER_DIR = helper
OBJ_DIR = obj
BIN_DIR = bin


ifdef DEBUG_MODE
CXXFLAGS += -DDEBUG_MODE
endif

# Find all source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
HELPER_SRCS := $(wildcard $(HELPER_DIR)/*.cpp)

# Define object files
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
HELPER_OBJS := $(patsubst $(HELPER_DIR)/%.cpp,$(OBJ_DIR)/helper/%.o,$(HELPER_SRCS))

# Define binary files
BINS := $(patsubst $(OBJ_DIR)/%.o,$(BIN_DIR)/%,$(OBJS))

# Define rule to compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Define rule to compile object files for helper files
$(OBJ_DIR)/helper/%.o: $(HELPER_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Define rule to link binary files
$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(HELPER_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Define all rule
all: $(BINS)

# Create directories
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR) $(OBJ_DIR)/helper)

# Define phony targets
.PHONY: clean

# Define clean rule
clean:
	rm -f $(OBJS) $(HELPER_OBJS) $(BINS)