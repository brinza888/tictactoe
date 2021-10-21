all: tictactoe

tictactoe: main.o tictactoe.o minimax.o
	gcc -o tictactoe main.o tictactoe.o minimax.o

main.o: main.c
	gcc -c main.c

tictactoe.o: tictactoe.c
	gcc -c tictactoe.c

minimax.o: minimax.c
	gcc -c minimax.c

clean:
	rm -rf *.o tictactoe

