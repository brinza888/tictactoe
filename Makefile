CC := gcc
CFLAGS := -Wall
LDFLAGS := -lncurses

BIN := bin/
EXE := ttt

SOURCES = tictactoe.c ai.c tgui.c main.c utils.c
OBJECTS = $(addprefix $(BIN), $(SOURCES:.c=.o))
EXE := $(addprefix $(BIN), $(EXE))

.PHONY: all
all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXE) $(LDFLAGS)

$(BIN)%.o: %.c $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN):
	@mkdir -p $(BIN)

.PHONY: clean
clean:
	rm -rf $(BIN)
