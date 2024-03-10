FROM alpine:latest

RUN apk add --no-cache openssh make ncurses ncurses-dev build-base

COPY sshd.conf /etc/ssh/sshd_config.d/

RUN mkdir -p /home/ttt
RUN adduser -h /home/ttt -s /usr/bin/ttt -D ttt
RUN passwd -d ttt

WORKDIR /home/ttt/
COPY Makefile ai.c ai.h game.c game.h main.c tgui.c tgui.h tictactoe.c tictactoe.h utils.c utils.h ./
RUN make && cp bin/ttt /usr/bin/

COPY run.sh /
WORKDIR /

ENTRYPOINT ["/bin/sh"]
CMD ["run.sh"]

EXPOSE 22
VOLUME /etc/ssh/keys
