# TicTacToe written in C
TicTacToe game with AI written in C (terminal interactive interface).

**Only Linux** is supported as target platform.

## Simplest way to start game
1. Download latest release in Releases section on GitHub.
2. Run binary file (tictactoe.exe or tictactoe). For Linux use following command:
  ```
  ./tictactoe
  ```

## Start game from project sources
Skip this if you have successfully run game in previous step.

### Getting sources
1. Clone repo (or you can download latest release sources):
```
git clone https://github.com/BrinzaBezrukoff/tictactoe.git
```
2. Navigate inside repo folder:
```
cd tictactoe
```

### One-command build and run
**You need to have ncurses library installed!**
```
make run
```


## Play
AI is playing as zero, human is playing as cross and always starts.

1. Choose level mode for AI **using arrow buttons** (this feature in develoment, modes are not balanced).

  ![image](https://user-images.githubusercontent.com/29017599/145648147-60493a5a-9297-4e91-ace0-eb3952e61be4.png)

2. When it is your time to make turn, **use arrow buttons** to navigate on map. Selection is marked with yellow crosses. Press enter to place Cross in selected cell.

  ![image](https://user-images.githubusercontent.com/29017599/145648244-b8f91880-a3f5-43aa-b557-976020f0186a.png)
  
  ![image](https://user-images.githubusercontent.com/29017599/145648543-74bd5d3f-54bd-48d5-829e-c30a338879bf.png)

