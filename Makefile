all: outdir tictactoe

tictactoe: bin/main.o bin/tictactoe.o bin/ai.o
	gcc -o tictactoe bin/main.o bin/tictactoe.o bin/ai.o

bin/main.o: main.c
	gcc -c main.c -o bin/main.o

bin/tictactoe.o: tictactoe.c
	gcc -c tictactoe.c -o bin/tictactoe.o

bin/ai.o: ai.c
	gcc -c ai.c -o bin/ai.o

clean:
	rm -rf bin tictactoe

outdir: bin

bin:
	mkdir -p bin

