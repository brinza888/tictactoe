# TicTacToe game
TicTacToe game with AI written in C (terminal interactive interface).

**Only Linux** is currently supported as main platform, but there is CMakeLists.txt, which can be useful to compile project on Windows.

## How to start game
### Method 1 - Use already compiled executable
Download latest release, extract archive and run already compiled executable file.

### Method 2 - Download sources and compile yourself
Install requirements for build (apt as example)
```bash
sudo apt update && sudo apt install -y git make gcc libncurses-dev
```
Clone repo and navigate inside directory
```bash
git clone https://github.com/brinza888/tictactoe.git && cd tictactoe
```
Build with GNU Make and GCC
```bash
make
```
Run executable file
```bash
./bin/ttt
```
Also you can simply install tictactoe to /usr/bin location (by default), using `install` target
```bash
make install
```

### Method 3 - Docker (self-hosted OpenSSH server)
This asuume you have Docker installed.

Clone repo and navigate inside directory
```bash
git clone https://github.com/brinza888/tictactoe.git && cd tictactoe
```
Build Docker image
```bash
docker build -t tictactoe -f docker/Dockerfile .
```
Run in background
```bash
docker run -d -v tictactoe:/etc/ssh/keys -p 9999:22 tictactoe
```
Now connect using user `ttt` (without password)
```bash
ssh -p 9999 ttt@localhost
```
This method can be used wherever you are since after deployment on server you will be need only SSH client on machine you want!
