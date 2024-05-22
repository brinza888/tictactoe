CC := gcc
CFLAGS := -Wall -pthread
LDFLAGS := -lncurses

PREFIX ?= /usr/local
DESTDIR = $(PREFIX)/bin

EXENAME := tictactoe

SRC := src
OBJ := obj
BIN := bin

SOURCES := tictactoe.c ai.c tgui.c game.c main.c loopback.c utils.c
OBJECTS := $(addprefix $(OBJ)/, $(SOURCES:.c=.o))
TARGET := $(BIN)/$(EXENAME)

all: $(TARGET)

install: $(TARGET)
	@mkdir -p $(DESTDIR)
	@install -m 755 $(TARGET) $(DESTDIR)

uninstall:
	@rm -f $(DESTDIR)/$(EXENAME)

$(TARGET): $(OBJECTS) | $(BIN)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN):
	@mkdir -p $(BIN)

$(OBJ):
	@mkdir -p $(OBJ)

clean:
	@rm -rf $(BIN) $(OBJ)

.PHONY: all install uninstall clean
