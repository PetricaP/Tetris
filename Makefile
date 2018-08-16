SRC_PATH=src/
CC=gcc
WN=-Wall -Wextra --pedantic -g
CFLAGS=-c ${WN}

INCL=-Iinclude/ -Iinclude/framework/ -Iinclude/game/ -Iinclude/tests/
FLAGS=-lSDL2 -lSDL2_ttf -lSDL2_image -lm
EXE=output

SRC=$(wildcard $(SRC_PATH)framework/*.c $(SRC_PATH)game/*.c)
OBJ=$(subst src, obj, $(SRC:.c=.o))
TESTSRC=$(wildcard $(SRC_PATH)tests/*.c)
TESTS=$(subst src/, , $(TESTSRC:.c= ))

all: $(EXE) $(TESTS)

tests/test%: obj/tests/test%.o $(OBJ)
	    $(CC) -o $@ $^ $(FLAGS) $(WN)

$(EXE): $(OBJ) obj/main.o
	    $(CC) -o $@ $^ $(FLAGS) $(WN)

obj/main.o: $(SRC_PATH)main.c
	    $(CC) $(CFLAGS) $(INCL) -o $@ $<

obj/%.o: $(SRC_PATH)%.c
	    $(CC) $(CFLAGS) $(INCL) -o $@ $<

obj/tests/%.o: $(SRC_PATH)tests/%.c
	    $(CC) $(CFLAGS) $(INCL) -o $@ $<

obj/framework/%.o: $(SRC_PATH)framework/%.c
	    $(CC) $(CFLAGS) $(INCL) -o $@ $<

obj/game/%.o: $(SRC_PATH)game/%.c
	    $(CC) $(CFLAGS) $(INCL) -o $@ $<


.PHONY : clean
clean :
	    -rm output && find obj/ -type f -delete
