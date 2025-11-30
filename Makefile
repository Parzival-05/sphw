CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra

all: ping pong

ping: ping.c
	$(CC) $(CFLAGS) -o ping ping.c

pong: pong.c
	$(CC) $(CFLAGS) -o pong pong.c

clean:
	rm -f ping pong data.txt ping.flag pong.flag data.tmp

run: all
	rm -f data.txt ping.flag pong.flag data.tmp && ./pong & ./ping & wait
