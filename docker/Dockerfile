FROM alpine:latest

RUN apk add --no-cache openssh make ncurses ncurses-dev build-base

RUN mkdir -p /home/ttt
RUN adduser -h /home/ttt -s /usr/bin/ttt -D ttt
RUN passwd -d ttt

COPY docker/rootfs/ /
COPY src/ /home/ttt/src/
COPY Makefile /home/ttt/

WORKDIR /home/ttt

RUN make install

ENTRYPOINT ["/bin/sh"]
CMD ["run.sh"]

EXPOSE 22
VOLUME /etc/ssh/keys

