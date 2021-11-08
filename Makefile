.PHONY: all clean run run_tgui outdir

all: ttt_console ttt_tgui


clean:
	rm -rf bin ttt_tgui ttt_console

run: ttt_console
	./ttt_console

run_tgui: ttt_tgui
	./ttt_tgui


ttt_console: bin/main.o bin/tictactoe.o bin/ai.o
	gcc -o ttt_console bin/main.o bin/tictactoe.o bin/ai.o

ttt_tgui: bin/tgui.o bin/tictactoe.o bin/ai.o
	gcc -o ttt_tgui bin/tgui.o bin/tictactoe.o bin/ai.o -lncurses


bin/main.o: main.c
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

