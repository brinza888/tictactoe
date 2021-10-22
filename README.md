# TicTacToe written in C
TicTacToe game with AI written in C (console interface)

## Installation and run
1. Clone repo:
```
git clone https://github.com/BrinzaBezrukoff/tictactoe.git
```
2. Navigate inside repo folder:
```
cd tictactoe
```
3. Compile source with make:
  ```
  make
  ```
  or with gcc:
  ```
  gcc main.c tictactoe.c minimax.c -o tictactoe
  ```
4. Run binary:
```
./tictactoe
```
## Play


1. Choose who will be make turn first. Type X or O and press Enter key.
  ![image](https://user-images.githubusercontent.com/29017599/138512021-00aa33b2-c18c-4c30-8938-d2b4b989de8e.png)
  * Human playing for X
  * AI playing for O
2. Choose level mode for AI (this feature in develoment, some modes work incorrect):
  ![image](https://user-images.githubusercontent.com/29017599/138511830-b7aa34fc-f540-44e7-8246-3402b464d4f4.png)
  * Easy (E)
  * Medium (M)
  * Hard (H)
  * eXpert (X)
3. When it is your time to make turn, type 2 numbers: row and column indices of cell, where you want to place X.
  ![image](https://user-images.githubusercontent.com/29017599/138511117-e12dba88-b23c-44d6-87a8-3afce50d9b97.png)
