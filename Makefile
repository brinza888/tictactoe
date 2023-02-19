.PHONY: all clean run outdir

OUTFILE = ttt

all: $(OUTFILE)

clean:
	rm -rf bin $(OUTFILE)

run: $(OUTFILE)
	./$(OUTFILE)

runsep: $(OUTFILE)
	gnome-terminal -- ./ttt

ttt: bin/main.o bin/tictactoe.o bin/ai.o bin/tgui.o
	gcc bin/main.o bin/tictactoe.o bin/ai.o bin/tgui.o -lncurses -o $(OUTFILE)

bin/main.o: main.c outdir
	gcc -c main.c -o bin/main.o

bin/tictactoe.o: tictactoe.c
	gcc -c tictactoe.c -o bin/tictactoe.o

bin/ai.o: ai.c
	gcc -c ai.c -o bin/ai.o

bin/tgui.o: tgui.c
	gcc -c tgui.c -o bin/tgui.o


outdir: bin

bin:
	mkdir -p bin

