CC := gcc
CFLAGS := -Wall
LDFLAGS := -lncurses

PREFIX = /usr
DESTDIR = $(PREFIX)/bin/

SRC := src/
BIN := bin/
EXE := ttt

SOURCES = tictactoe.c ai.c tgui.c game.c main.c utils.c
OBJECTS = $(addprefix $(BIN), $(SOURCES:.c=.o))
EXE := $(addprefix $(BIN), $(EXE))

.PHONY: all
all: $(EXE)

.PHONY: install
install: $(EXE)
	install -m 755 $(EXE) $(DESTDIR)

$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXE) $(LDFLAGS)

$(BIN)%.o: $(SRC)%.c $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN):
	@mkdir -p $(BIN)

.PHONY: clean
clean:
	rm -rf $(BIN)

