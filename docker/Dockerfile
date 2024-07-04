FROM alpine:latest

RUN apk add --no-cache openssh make ncurses ncurses-dev build-base

RUN adduser -D ttt
RUN passwd -d ttt

COPY docker/rootfs /

COPY src /usr/local/src/tictactoe/src
COPY include /usr/local/src/tictactoe/include
COPY Makefile /usr/local/src/tictactoe/

WORKDIR /usr/local/src/tictactoe
RUN make install

WORKDIR /

ENTRYPOINT ["run-ssh"]

EXPOSE 22
VOLUME /etc/ssh/keys
