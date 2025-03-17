CXX := g++
CXXFLAGS := -Iinclude -Wall -Wextra -std=c++17 -g
SRC_DIR := src
TEST_DIR := tests

SRC := $(SRC_DIR)/expression.cpp $(SRC_DIR)/differentiator.cpp
OBJ := $(SRC:.cpp=.o)

TEST_SRC := $(TEST_DIR)/test.cpp
TEST_OBJ := $(TEST_SRC:.cpp=.o)
TEST_DEP := $(filter-out $(SRC_DIR)/differentiator.o, $(OBJ))

EXEC := differentiator
TEST_EXEC := test_runner

.PHONY: all clean test

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $^ -o $@

$(TEST_EXEC): $(TEST_OBJ) $(TEST_DEP)
	$(CXX) $^ -o $@

test: $(TEST_EXEC)
	./$(TEST_EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(EXEC) $(TEST_EXEC)