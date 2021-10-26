all: tictactoe

tictactoe: main.o tictactoe.o ai.o
	gcc -o tictactoe main.o tictactoe.o ai.o

main.o: main.c
	gcc -c main.c

tictactoe.o: tictactoe.c
	gcc -c tictactoe.c

ai.o: ai.c
	gcc -c ai.c

clean:
	rm -rf *.o tictactoe

