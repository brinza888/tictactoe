# TicTacToe game
TicTacToe game with AI written in C (terminal interactive interface).

**Only Linux** is currently supported as target platform.

## How to start game
### Method 1 - Use already compiled executable
1. Download latest release in Releases section on GitHub.
2. Extract downloaded archive.
3. Run executable file:
  ```
  ./ttt
  ```

### Method 2 - Download sources and build
1. Install requirements for build
```
sudo apt update && sudo apt install -y git make gcc libncurses-dev
```
2. Clone repo:
```
git clone https://github.com/BrinzaBezrukoff/tictactoe.git
```
3. Navigate inside repository directory:
```
cd tictactoe
```
4. Build with GNU Make and GCC
```
make
```
5. Run executable file
```
./ttt
```
#### Some extra make options
1. For systems with gnome-terminal installed there is option to run game in separated window:
```
make window
```
2. Build and run in one line:
```
make run
```
3. Clean everything related to build
```
make clean
```

## Gameplay
AI is playing as zero, human is playing as cross and always starts.

1. Choose level mode for AI **using arrow buttons** (this feature in develoment, modes are not balanced).

  ![image](https://user-images.githubusercontent.com/29017599/145648147-60493a5a-9297-4e91-ace0-eb3952e61be4.png)

2. When it is your time to make turn, **use arrow buttons** to navigate on map. Selection is marked with yellow crosses. Press enter to place Cross in selected cell.

  ![image](https://user-images.githubusercontent.com/29017599/145648244-b8f91880-a3f5-43aa-b557-976020f0186a.png)
  
  ![image](https://user-images.githubusercontent.com/29017599/145648543-74bd5d3f-54bd-48d5-829e-c30a338879bf.png)

