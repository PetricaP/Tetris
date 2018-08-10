SRC_PATH=./src/
CC=gcc
WN=-Wall -Wextra --pedantic -g
CFLAGS=-c ${WN}
INC=./include

INCL=-I$(INC)
FLAGS=-lSDL2 -lSDL2_ttf -lSDL2_image -lm
EXE=output

SRC=$(wildcard $(SRC_PATH)*.c)
OBJ=$(subst ./src, ./obj, $(SRC:.c=.o))

all: $(SRC) $(EXE)

$(EXE): $(OBJ)
	    $(CC) $(OBJ) -o $@ ${FLAGS} ${WN}

./obj/%.o: ./src/%.c
	    $(CC) $(CFLAGS) $(INCL) -o $@ $<

.PHONY : clean
clean :
	    -rm output && find ./obj/ -type f -delete
