# Variables
SRC_PATH = src/
CC = gcc
WN = -Werror -Wall -Wextra --pedantic -g -std=c11
CFLAGS = -c ${WN} 
INCL = -Iinclude/ -Iinclude/framework/ -Iinclude/game/ -Iinclude/tests/ -I/opt/homebrew/include
FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -lm -L/opt/homebrew/lib
EXE = output

# Source files
SRC = $(wildcard $(SRC_PATH)framework/*.c $(SRC_PATH)game/*.c)
OBJ = $(subst src, obj, $(SRC:.c=.o))
TESTSRC = $(wildcard $(SRC_PATH)tests/*.c)
TESTS = $(patsubst src/tests/test_%.c, tests/%, $(TESTSRC))

# Default rule
all: setup $(EXE) $(TESTS)

# Create required directories
setup:
	mkdir -p obj/framework obj/game obj/tests tests

# Build the main executable
$(EXE): $(OBJ) obj/main.o
	$(CC) -o $@ $^ $(FLAGS) $(WN)

# Rule for tests
tests/%: obj/tests/test_%.o $(OBJ)
	$(CC) -o $@ $^ $(FLAGS) $(WN)

# Compile source files
obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCL) -o $@ $<

# Clean rule
clean:
	rm -rf obj $(EXE) tests/*

# PHONY targets
.PHONY: all clean setup
