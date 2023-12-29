CC = gcc
CFLAGS = -Wall

BIN = bin/
EXE = ttt

SOURCES = tictactoe.c ai.c tgui.c main.c utils.c
OBJECTS = $(addprefix $(BIN), $(SOURCES:.c=.o))

.PHONY: all
all: $(EXE)

.PHONY: clean
clean:
	rm -rf $(BIN) $(EXE)

.PHONY: run
run: $(EXE)
	./$(EXE)

.PHONY: window
window: $(EXE)
	gnome-terminal -- ./$(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -lncurses -o $(EXE)

$(BIN)%.o: %.c $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN):
	mkdir -p $(BIN)

